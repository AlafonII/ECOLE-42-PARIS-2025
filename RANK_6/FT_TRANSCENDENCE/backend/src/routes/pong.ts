import { FastifyInstance } from "fastify";
import { GameRoom } from "../game/GameRoom";
import { getQueries } from "../database";
import { getChatGateway } from "../chat/ChatGateway";

type JwtPayload = {
	id: number;
	username: string;
	email: string;
};

const waitingRooms: Map<number, GameRoom> = new Map();

function generateRoomId(): string {
	return "room_" + Date.now();
}

async function createRoom(matchId: number): Promise<GameRoom> {
	const roomId = generateRoomId();
	const room = new GameRoom(roomId);

	room.onGameEnd = async (winner, scores, durationMs, reason) => {
		//console.log(`Game ${matchId} ended. Winner: Player ${winner}`);
		//console.log(`Game ${matchId} ended. Winner: Player ${winner}`);

		const { player1Id, player2Id } = room.getPlayerUserIds();
		if (!player1Id || !player2Id) {
			/*console.log(
				"At least, one player is not authenticated, skipping DB save",
			);*/
			return;
		}
		const queries = getQueries();

		let finalScores = { ...scores };
		let finalWinner = winner;

		if (
			reason === "opponent_inactive" ||
			reason === "opponent_left" ||
			reason === "you_inactive" ||
			reason === "forfeit"
		) {
			finalScores =
				winner === 1
					? { player1: 5, player2: 0 }
					: { player1: 0, player2: 5 };
			finalWinner = winner;
		}

		await queries.updateMatchScore(matchId, player1Id, finalScores.player1);
		await queries.updateMatchScore(matchId, player2Id, finalScores.player2);

		const winnerId = finalWinner === 1 ? player1Id : player2Id;
		await queries.setMatchWinner(
			matchId,
			winnerId,
			Math.floor(durationMs / 1000),
		);

		//console.log("Game saved to database");
		//console.log("Game saved to database");

		// Send notification to next match players
		const match = await queries.getMatchWithUsers(matchId);
		if (match?.tournament_id) {
			const nextMatch = await queries.nextMatchToBePlayed(match.tournament_id);
			if (nextMatch) {
				const nextMatchWithPlayers = await queries.getMatchWithUsers(nextMatch.id);
				if (nextMatchWithPlayers.players.length >= 2) {
					const player1 = nextMatchWithPlayers.players[0];
					const player2 = nextMatchWithPlayers.players[1];
					if (player1.user_id && player2.user_id) {
						const chatGateway = getChatGateway();
						if (chatGateway) {
							chatGateway.sendTournamentMatchReady(
								player1.user_id,
								player2.user_id,
								player1.username || "Player 1",
								player2.username || "Player 2",
								nextMatch.id,
								match.tournament_id
							);
							//console.log(`[Tournament] Notified players for next match`);
							//console.log(`[Tournament] Notified players for next match`);
						}
					}
				}
			}
		}
	};

	room.onEmpty = () => {
		waitingRooms.delete(matchId);
	};

	return room;
}

export async function pongRoutes(fastify: FastifyInstance) {
	fastify.get("/pong/play", { websocket: true }, (connection, req) => {
		const socket = (connection as any).socket ?? connection;
		if (!socket || typeof (socket as any).send !== "function") {
			fastify.log.error(
				{ connection },
				"Invalid websocket connection object",
			);
			return;
		}
		const matchIdRaw = (req.query as any)?.matchId;
		const matchId = Number(matchIdRaw);
		if (!Number.isFinite(matchId)) {
			socket.send(
				JSON.stringify({
					type: "error",
					message: "Invalid or missing matchId",
				}),
			);
			socket.close();
			return;
		}

		//console.log("New player connected");
		//console.log("New player connected");
		let userId: number | undefined;
		(async () => {
			try {
				const token = req.cookies?.access_token;
				//console.log("WS cookies:", req.cookies);
				//console.log("WS cookies:", req.cookies);
				if (token) {
					const payload = (await req.server.jwt.verify(
						token,
					)) as JwtPayload;
					userId = payload.id;
					//console.log("Authenticated player:", userId);
					//console.log("Authenticated player:", userId);
				}
			} catch (error) {
				socket.send(
					JSON.stringify({
						type: "error",
						message: "Session expired. Please log in again.",
					}),
				);
				socket.close();
				return;
			}

			let room = waitingRooms.get(matchId);
			/*console.log(
			/*console.log(
				"Waiting room:",
				room?.id,
				"playerCount:",
				room?.playerCount,
			);*/

			if (room && !room.isFull()) {
				//console.log("Joining existing room:", room.id);
				//console.log("Joining existing room:", room.id);
			} else {
				room = await createRoom(matchId);
				//console.log("Created new room:", room.id);
				//console.log("Created new room:", room.id);
				waitingRooms.set(matchId, room);
			}

			const added = room.addPlayer(socket, userId);
			//console.log("Joined match", matchId, "userId", userId);
			//console.log("Room userIds after join:", room.getPlayerUserIds());
			//console.log("Joined match", matchId, "userId", userId);
			//console.log("Room userIds after join:", room.getPlayerUserIds());

			if (!added) {
				socket.send(
					JSON.stringify({
						type: "error",
						message: "You are already in the game",
					}),
				);
				socket.close();
				/*console.log(
					"Player tried to connect twice, connection is ending...",
				);*/
				return;
			}

			//console.log("Room player count after join:", room.playerCount);
			//console.log("Room player count after join:", room.playerCount);

			if (room.isFull()) waitingRooms.delete(matchId);
		})();
	});
}
