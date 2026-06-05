import { FastifyInstance } from "fastify";
import { getQueries } from "../database";
import { parseId } from "../utils/utils";

interface CreateMatchBody {
	gameTypeId: number;
	tournamentId?: number;
	roundNumber?: number;
	matchNumber?: number;
}

export async function matchRoutes(fastify: FastifyInstance) {
	const queries = getQueries();

	fastify.get("/matches", async (request, reply) => {
		try {
			const matches = await queries.getAllMatchesWithUsers();
			return { success: true, data: matches };
		} catch (error) {
			console.error("Error: ", error);
			return reply.code(500).send({
				success: false,
				error: "Failed to fetch matches",
			});
		}
	});

	fastify.get<{ Params: { id: string } }>(
		"/matches/by-user/:id",
		async (request, reply) => {
			try {
				const id = parseId(request.params.id, "user ID");

				const matches = await queries.getAllMatchesOfUser(id);

				return { success: true, data: matches };
			} catch (error) {
				console.error("Error:", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to fetch user matches",
				});
			}
		}
	);

	fastify.get("/matches/finished", async (request, reply) => {
		const matches = await queries.getAllFinishedMatchesWithUsers();
		return { success: true, data: matches };
	});

	fastify.get("/matches/unplayed", async (request, reply) => {
		const matches = await queries.getAllUnplayedMatchesWithUsers();
		return { success: true, data: matches };
	});

	fastify.get<{ Params: { id: string } }>(
		"/matches/:id",
		async (request, reply) => {
			try {
				const id = parseId(request.params.id, "match ID");

				const match = await queries.getMatchWithUsers(id);

				if (!match) {
					return reply.code(404).send({
						success: false,
						error: "Match not found",
					});
				}
				return { success: true, data: match };
			} catch (error) {
				console.error("Error:", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to fetch match",
				});
			}
		}
	);

	fastify.post<{
		Body: CreateMatchBody;
	}>("/matches/new-match", async (request, reply) => {
		try {
			const { gameTypeId, tournamentId, roundNumber, matchNumber } =
				request.body;

			if (!gameTypeId || typeof gameTypeId !== "number") {
				return reply.code(400).send({
					success: false,
					error: "gameTypeId is required and must be a number",
				});
			}

			const createdMatchId = await queries.createMatch(
				gameTypeId,
				tournamentId,
				roundNumber,
				matchNumber
			);

			if (!createdMatchId || createdMatchId === 0) {
				return reply.code(500).send({
					success: false,
					error: "Couldn't create match",
				});
			}
			return reply.code(201).send({
				success: true,
				data: createdMatchId,
			});
		} catch (error) {
			console.error("Error:", error);
			return reply.code(500).send({
				success: false,
				error: "Failed to create match",
			});
		}
	});

	fastify.post<{ Body: { matchId: number; userId?: number } }>(
		"/matches/add-participant",
		async (request, reply) => {
			try {
				const { matchId, userId } = request.body;

				if (!matchId || typeof matchId !== "number") {
					return reply.code(400).send({
						success: false,
						error: "matchId is required and must be a number",
					});
				}

				if (userId !== undefined && typeof userId !== "number") {
					return reply.code(400).send({
						success: false,
						error: "userId must be a number",
					});
				}

				await queries.addMatchParticipant(matchId, userId);

				const matchWithParticipants = await queries.getMatchWithUsers(
					matchId
				);

				return reply.code(201).send({
					success: true,
					data: matchWithParticipants,
				});
			} catch (error) {
				console.error("Error:", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to add match participant",
				});
			}
		}
	);

	fastify.put<{
		Body: { matchId: number; userId: number | null; score: number };
	}>("/matches/update-score", async (request, reply) => {
		try {
			const { matchId, userId, score } = request.body;

			if (!matchId || typeof matchId !== "number") {
				return reply.code(400).send({
					success: false,
					error: "matchId is required and must be a number",
				});
			}

			if (
				score === undefined ||
				score === null ||
				typeof score !== "number"
			) {
				return reply.code(400).send({
					success: false,
					error: "score is required and must be a number",
				});
			}

			await queries.updateMatchScore(matchId, userId, score);
			const matchWithParticipants = await queries.getMatchWithUsers(
				matchId
			);

			return reply.code(200).send({
				success: true,
				data: matchWithParticipants,
			});
		} catch (error) {
			console.error("Error:", error);
			return reply.code(500).send({
				success: false,
				error: "Failed to update match score",
			});
		}
	});

	fastify.put<{
		Body: {
			matchId: number;
			winnerId: number | null;
			durationSeconds: number;
		};
	}>("/matches/set-winner", async (request, reply) => {
		try {
			const { matchId, winnerId, durationSeconds } = request.body;

			if (typeof matchId !== "number") {
				return reply.code(400).send({
					success: false,
					error: "matchId is required and must be a number",
				});
			}

			if (typeof durationSeconds !== "number") {
				return reply.code(400).send({
					success: false,
					error: "durationSeconds is required and must be a number",
				});
			}

			await queries.setMatchWinner(matchId, winnerId, durationSeconds);
			const matchWithParticipants = await queries.getMatchWithUsers(
				matchId
			);

			return reply.code(200).send({
				success: true,
				data: matchWithParticipants,
			});
		} catch (error) {
			console.error("Error:", error);
			return reply.code(500).send({
				success: false,
				error: "Failed to set match winner",
			});
		}
	});
}
