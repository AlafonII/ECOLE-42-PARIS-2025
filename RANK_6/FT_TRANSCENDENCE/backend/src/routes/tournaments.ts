import { FastifyInstance } from "fastify";
import { getQueries } from "../database";
import { parseId } from "../utils/utils";
import { request } from "node:http";

interface TournamentCreation {
  name: string;
  gameTypeId: number;
  participantsNb: number;
  organiserId: number;
}
interface AddTournamentParticipant {
  tournamentId: number;
  userId: number;
  alias: string;
}

interface MatchForTournament {
  player1Id: number;
  player2Id: number;
  roundNumber: number;
  matchNumber: number;
}

async function generateMatchesForTournament(
  tournamentId: number,
): Promise<MatchForTournament[]> {
  const queries = getQueries();

  const participants = await queries.getTournamentParticipants(tournamentId);

  if (participants.length < 2) {
    throw new Error("Need at least 2 participants for a tournament");
  }

  const userIds = participants.map((p) => p.user_id);

  const matchSchedule: MatchForTournament[] = [];
  const n = userIds.length;
  const players = n % 2 === 0 ? [...userIds] : [...userIds, -1];
  const numPlayers = players.length;
  let matchCount = 1;

  for (let round = 0; round < numPlayers - 1; round++) {
    for (let i = 0; i < numPlayers / 2; i++) {
      const player1 = players[i];
      const player2 = players[numPlayers - 1 - i];
      if (player1 !== -1 && player2 !== -1) {
        matchSchedule.push({
          player1Id: player1,
          player2Id: player2,
          roundNumber: round + 1,
          matchNumber: matchCount++,
        });
      }
    }
    players.splice(1, 0, players.pop()!);
  }
  return matchSchedule;
}

export async function tournamentRoutes(fastify: FastifyInstance) {
  const queries = getQueries();

  fastify.get("/tournaments", async (request, reply) => {
    try {
      const tournaments = await queries.getAllTournaments();
      return { success: true, data: tournaments };
    } catch (error) {
      console.error("Error: ", error);
      return reply.code(500).send({
        success: false,
        error: "Failed to fetch tournaments",
      });
    }
  });

  fastify.get<{ Params: { id: string } }>(
    "/tournaments/by-id/:id",
    async (request, reply) => {
      try {
        const id = parseId(request.params.id, "tournament ID");
        const tournament = await queries.getTournamentById(id);

        if (!tournament) {
          return reply.code(404).send({
            success: false,
            error: "Tournament not found",
          });
        }
        return { success: true, data: tournament };
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to fetch tournament",
        });
      }
    },
  );

  fastify.get<{ Params: { id: string } }>(
    "/tournaments/tournament-matches/:id",
    async (request, reply) => {
      try {
        const id = parseId(request.params.id, "tournament ID");

        const result = await queries.getMatchesForTournament(id);

        return { success: true, data: result };
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to fetch tournament matches",
        });
      }
    },
  );

  fastify.get<{ Params: { id: string } }>(
    "/tournaments/tournament-participants/:id",
    async (request, reply) => {
      try {
        const id = parseId(request.params.id, "tournament ID");

        const result = await queries.getTournamentParticipants(id);

        return { success: true, data: result };
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to fetch tournament participants",
        });
      }
    },
  );

  fastify.get<{ Params: { userId: string } }>(
    "/tournaments/user-tournaments/:userId",
    async (request, reply) => {
      try {
        const userId = parseId(request.params.userId, "user ID");

        const result = await queries.allTournamentsByUser(userId);
        return { success: true, data: result };
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to fetch tournaments of user",
        });
      }
    },
  );

  fastify.get<{ Params: { tournamentId: string } }>(
    "/tournaments/tournament-table/:tournamentId",
    async (request, reply) => {
      try {
        const tournamentId = parseId(
          request.params.tournamentId,
          "tournament ID",
        );

        const result = await queries.getTournamentRanking(tournamentId);
        return { success: true, data: result };
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to fetch tournament ranking",
        });
      }
    },
  );

  fastify.get<{ Params: { tournamentId: string; userId: string } }>(
    "/tournaments/is-user-participant/:tournamentId/:userId",
    async (request, reply) => {
      try {
        const tournamentId = parseId(
          request.params.tournamentId,
          "tournament ID",
        );
        const userId = parseId(request.params.userId, "user ID");

        const isPresent = await queries.isUserInTournament(
          tournamentId,
          userId,
        );
        return { success: true, data: isPresent };
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to verify if user is part of tournament",
        });
      }
    },
  );

  fastify.get<{ Params: { tournamentId: string } }>(
    "/tournaments/is-tournament-full/:tournamentId",
    async (request, reply) => {
      try {
        const tournamentId = parseId(
          request.params.tournamentId,
          "tournament ID",
        );

        const isFull = await queries.isTournamentFull(tournamentId);
        return { success: true, data: isFull };
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to verify if tournament is full",
        });
      }
    },
  );

  fastify.post<{ Body: TournamentCreation }>(
    "/tournaments",
    async (request, reply) => {
      try {
        const { name, gameTypeId, participantsNb, organiserId } = request.body;

        if (!name || typeof name !== "string") {
          return reply.code(400).send({
            success: false,
            error: "tournament name is required and must be a string",
          });
        }

        if (!gameTypeId || typeof gameTypeId !== "number") {
          return reply.code(400).send({
            success: false,
            error: "gameTypeId is required and must be a number",
          });
        }

        if (!participantsNb || typeof participantsNb !== "number") {
          return reply.code(400).send({
            success: false,
            error: "participantsNb is required and must be a number",
          });
        }

        if (participantsNb < 2) {
          return reply.code(400).send({
            success: false,
            error:
              "There must be at least 2 participants to create a tournament",
          });
        }

        const createdTournamentId = await queries.createTournament(
          name,
          gameTypeId,
          participantsNb,
          organiserId,
        );
        if (!createdTournamentId || createdTournamentId === 0) {
          return reply.code(500).send({
            success: false,
            error: "Couldn't create tournament",
          });
        }
        return reply.code(201).send({
          success: true,
          data: createdTournamentId,
        });
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to create tournament",
        });
      }
    },
  );

  fastify.put<{ Params: { id: string } }>(
    "/tournaments/start-tournament/:id",
    async (request, reply) => {
      try {
        const id = parseId(request.params.id, "tournament ID");

        const tournament = await queries.getTournamentById(id);
        if (!tournament) {
          return reply.code(404).send({
            success: false,
            error: "Tournament not found",
          });
        }

        if (tournament.started_at) {
          return reply.code(409).send({
            success: false,
            error: "Tournament has already started",
          });
        }

        const participants = await queries.getTournamentParticipants(id);
        if (participants.length < 2) {
          return reply.code(400).send({
            success: false,
            error: "Tournament needs at least 2 participants to start",
          });
        }

        await queries.begin();

        const matches = await generateMatchesForTournament(id);

        if (!matches.length) {
          return reply.code(500).send({
            success: false,
            error: "Failed to generate matches for tournament",
          });
        }

        await queries.startTournament(id);

        for (const match of matches) {
          const matchId = await queries.createMatch(
            tournament?.game_type_id,
            tournament.id,
            match.roundNumber,
            match.matchNumber,
          );
          await queries.addMatchParticipant(matchId, match.player1Id);
          await queries.addMatchParticipant(matchId, match.player2Id);
        }
        await queries.commit();

        // Notify the first tournament match to players
        const firstMatch = await queries.nextMatchToBePlayed(id);
        if (firstMatch) {
          const matchWithPlayers = await queries.getMatchWithUsers(
            firstMatch.id,
          );
          if (matchWithPlayers.players.length >= 2) {
            const player1 = matchWithPlayers.players[0];
            const player2 = matchWithPlayers.players[1];
            if (player1.user_id && player2.user_id) {
              const { getChatGateway } = await import("../chat/ChatGateway");
              const chatGateway = getChatGateway();
              if (chatGateway) {
                chatGateway.sendTournamentMatchReady(
                  player1.user_id,
                  player2.user_id,
                  player1?.username || "Player 1",
                  player2?.username || "Player 2",
                  firstMatch.id,
                  id,
                );
                //console.log(`[Tournament] Notified players for first match`);
              }
            }
          }
        }

        return reply.code(200).send({
          success: true,
          data: tournament.id,
        });
      } catch (error) {
        console.error("Error: ", error);
        try {
          await queries.rollback();
        } catch {}
        return reply.code(500).send({
          success: false,
          error: "Failed to start tournament",
        });
      }
    },
  );

  fastify.put<{ Params: { id: string } }>(
    "/tournaments/finish-tournament/:id",
    async (request, reply) => {
      try {
        const id = parseId(request.params.id, "tournament ID");

        await queries.finishTournament(id);
        return reply.code(200).send({
          success: true,
        });
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to end tournament",
        });
      }
    },
  );

  fastify.post<{ Body: AddTournamentParticipant }>(
    "/tournaments/add-participant",
    async (request, reply) => {
      try {
        const { tournamentId, userId, alias } = request.body;

        if (!tournamentId || typeof tournamentId !== "number") {
          return reply.code(400).send({
            success: false,
            error: "tournamentId is required and must be a number",
          });
        }
        if (!userId || typeof userId !== "number") {
          return reply.code(400).send({
            success: false,
            error: "userId is required and must be a number",
          });
        }
        if (!alias || typeof alias !== "string") {
          return reply.code(400).send({
            success: false,
            error: "alias is required and must be a string",
          });
        }

        const tournament = await queries.getTournamentById(tournamentId);
        if (!tournament) {
          return reply.code(404).send({
            success: false,
            error: "Tournament not found",
          });
        }

        if (tournament.started_at) {
          return reply.code(400).send({
            success: false,
            error: "Tournament has already started",
          });
        }

        const isFull = await queries.isTournamentFull(tournamentId);
        if (isFull) {
          return reply.code(400).send({
            success: false,
            error: "Tournament is full",
          });
        }

        const isAlreadyIn = await queries.isUserInTournament(
          tournamentId,
          userId,
        );
        if (isAlreadyIn) {
          return reply.code(400).send({
            success: false,
            error: "User is already in tournament",
          });
        }

        const isAliasAlreadyIn = await queries.isAliasInTournament(
          tournamentId,
          alias,
        );
        if (isAliasAlreadyIn) {
          return reply.code(400).send({
            success: false,
            error: "This alias is already taken",
          });
        }

        await queries.addTournamentParticipant(tournamentId, userId, alias);
        return reply.code(201).send({
          success: true,
        });
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to add tournament participant",
        });
      }
    },
  );

  fastify.delete<{ Params: { tournamentId: string; userId: string } }>(
    "/tournaments/delete-participant/:tournamentId/:userId",
    async (request, reply) => {
      try {
        const tournamentId = parseId(
          request.params.tournamentId,
          "tournament ID",
        );
        const userId = parseId(request.params.userId, "user ID");

        const tournament = await queries.getTournamentById(tournamentId);
        if (!tournament) {
          return reply.code(404).send({
            success: false,
            error: "Can't find tournament",
          });
        }

        if (tournament.started_at) {
          return reply.code(400).send({
            success: false,
            error:
              "Can't delete participant from a tournament that has already started",
          });
        }

        await queries.removeTournamentParticipant(tournamentId, userId);
        return reply.code(200).send({
          success: true,
        });
      } catch (error) {
        console.error("Error: ", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to remove tournament participant",
        });
      }
    },
  );

  fastify.get<{ Params: { id: string } }>(
    "/tournaments/next-match/:id",
    async (request, reply) => {
      try {
        const tournamentId = parseId(request.params.id, "Tournament ID");

        const nextMatch = await queries.nextMatchToBePlayed(tournamentId);
        if (!nextMatch) {
          return reply.code(404).send({
            success: false,
            error: "No more matches to be played",
          });
        }
        return { success: true, data: nextMatch };
      } catch (error) {
        return reply.code(500).send({
          success: false,
          error: "Failed to fetch next match",
        });
      }
    },
  );

  fastify.get<{ Params: { userId: string } }>(
    "/tournaments/pending-match/:userId",
    async (request, reply) => {
      try {
        const userId = parseInt(request.params.userId);

        const userTournaments = await queries.allTournamentsByUser(userId);
        const activeTournaments = userTournaments.filter(
          (t) => t.status === "in_progress",
        );

        for (const tournament of activeTournaments) {
          const nextMatch = await queries.nextMatchToBePlayed(tournament.id);
          if (nextMatch) {
            const matchWithPlayers = await queries.getMatchWithUsers(
              nextMatch.id,
            );
            const isPlayerInMatch = matchWithPlayers.players.some(
              (p) => p.user_id === userId,
            );
            if (isPlayerInMatch && matchWithPlayers.players.length >= 2) {
              const player1 = matchWithPlayers.players[0];
              const player2 = matchWithPlayers.players[1];
              return {
                success: true,
                data: {
                  matchId: nextMatch.id,
                  tournamentId: tournament.id,
                  player1Name: player1?.username || "Player 1",
                  player2Name: player2?.username || "Player 2",
                  roundNumber: nextMatch.round_number,
                },
              };
            }
          }
        }
        return { success: true, data: null };
      } catch (error) {
        console.error("[Tournaments] Error in pending-match:", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to get pending match",
        });
      }
    },
  );
}
