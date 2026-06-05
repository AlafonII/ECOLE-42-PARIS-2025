import { FastifyInstance } from "fastify";
import WebSocket from "ws";
import { getQueries } from "../database/index.ts";
import { ChatWSMessage } from "./chat.types";
import { timeStamp } from "node:console";

type Client = {
	userId: number;
	socket: WebSocket;
};

export class ChatGateway {
	private clients = new Map<number, WebSocket>();

	constructor(private app: FastifyInstance) {}

	register() {
		this.app.get("/ws/chat", { websocket: true }, (connection, req) => {
			const ws = connection;

			const token = req.query.token as string;

			if (!token) {
				//console.log('[CHAT] No token provided');
				ws.close(1008, "Unauthorized - No token");
				return;
			}

			(async () => {
				try {
					const payload = await req.server.jwt.verify(token);

					const userId = payload.userId || payload.id;

					if (!userId) {
						//console.log('[CHAT] No userId in payload');
						ws.close(1008, "Invalid token - no user ID");
						return;
					}

					this.clients.set(userId, ws);

					ws.on("message", (raw) => {
						this.onMessage(userId, raw.toString());
					});

					ws.on("close", () => {
						this.clients.delete(userId);
					});

					ws.on("error", (err) => {
						console.error(`[CHAT] Socket error for user ${userId}:`, err);
					});

				} catch (err) {
					console.error("[CHAT] Auth failed:", err);
					ws.close(1008, "Unauthorized");
				}
			})();
		});
	}

	async onMessage(fromUserId: number, raw: string) {
		try {
			const msg: ChatWSMessage = JSON.parse(raw);
			const queries = getQueries();

			if (!msg || typeof msg !== "object" || !("type" in msg)) {
				//console.log('[CHAT] Invalid message format');
				return;
			}

			if (msg.type === "DIRECT_MESSAGE") {
				const { toUserId, content } = msg;

				const isBlocked = await queries.isBlockedBidirectional(fromUserId, toUserId);
				if (isBlocked) {
					this.sendErrorToUser(fromUserId, "Cannot send message: blocked relationship");
					return;
				}

				const friendships = await queries.getUserFriendships(fromUserId);
				const friendIds = friendships.map(f =>
					f.user_id === fromUserId ? f.contact_id : f.user_id
				);

				if (!friendIds.includes(toUserId)) return;

				let messageType = "text";
				let metadata = null;
				let actualContent = content;

				try {
					const parsed = JSON.parse(content);
					if (parsed.messageType) {
						messageType = parsed.messageType;
						metadata = parsed.metadata || null;
						actualContent = parsed.content || content;
					}
				} catch {}

				let threadId = await queries.getDirectThread(fromUserId, toUserId);
				if (!threadId) {
					threadId = await queries.createThread([fromUserId, toUserId], 0);
				}

				await queries.createMessage(
					threadId,
					fromUserId,
					actualContent,
					messageType,
					metadata
				);

				if (messageType === "GAME_INVITE" ||
					messageType === "GAME_INVITE_ACCEPTED" ||
					messageType === "GAME_INVITE_DECLINED" ||
					messageType === "GAME_INVITE_CANCELLED") {

					this.sendGameMessage(
						fromUserId,
						toUserId,
						actualContent,
						messageType,
						metadata);
					return;
				}

				this.sendToUser(
					fromUserId,
					toUserId,
					actualContent,
					messageType,
					metadata);
			}

			if (msg.type === "BROADCAST") {
				const friendships = await queries.getUserFriendships(fromUserId);

				const friendIds = friendships.map(f =>
					f.user_id === fromUserId ? f.contact_id : f.user_id
				 );

				for (const friendId of friendIds) {
					const isBlocked = await queries.isBlockedBidirectional(fromUserId, friendId);
					if (!isBlocked) {
						let threadId = await queries.getDirectThread(fromUserId, friendId);
						if (!threadId)
							threadId = await queries.createThread([fromUserId, friendId], 0);

						await queries.createMessage(threadId, fromUserId, msg.content, "text", null);

						this.sendToUser(fromUserId, friendId, msg.content);
					} else {
						//console.log(`[CHAT] Skipping broadcast to ${friendId} due to block`);
					}
				}
			}
		} catch (err) {
			console.error('[CHAT] Error handling message:', err);
		}
	}

	sendToUser(
		from: number,
		to: number,
		content: string,
		messageType?: string,
		metadata?: any) {
		const payload = JSON.stringify({
			type: "RECEIVE_MESSAGE",
			fromUserId: from,
			content,
			timestamp: Date.now(),
			messageType,
			metadata,
		});

		const recipient = this.clients.get(to);
		if (recipient?.readyState === WebSocket.OPEN) {
			recipient.send(payload);
		}

		const sender = this.clients.get(from);
		if (sender?.readyState === WebSocket.OPEN) {
			sender.send(payload);
		}
	}

	sendGameMessage(
		from: number,
		to: number,
		content: string,
		messageType: string,
		metadata?: any) {
		const payload = JSON.stringify({
			type: "RECEIVE_MESSAGE",
			fromUserId: from,
			content,
			timestamp: Date.now(),
			messageType,
			metadata,
		});

		const recipient = this.clients.get(to);
		if (recipient?.readyState === WebSocket.OPEN) {
			recipient.send(payload);
			//console.log(`[CHAT] Sent ${messageType} to user ${to}`);
		} else {
			//console.log(`[CHAT] User ${to} not connected, cannot send ${messageType}`);
		}

		const sender = this.clients.get(from);
		if (sender?.readyState === WebSocket.OPEN) {
			sender.send(payload);
		}
	}

	sendErrorToUser(userId: number, error: string) {
		const payload = JSON.stringify({
			type: "ERROR",
			error,
			timestamp: Date.now(),
		});

		const client = this.clients.get(userId);
		if (client?.readyState === WebSocket.OPEN) {
			client.send(payload);
		}
	}

	sendTournamentMatchReady(
		player1Id: number,
		player2Id: number,
		player1Name: string,
		player2Name: string,
		matchId: number,
		tournamentId: number,
	) {
		const payload = JSON.stringify({
				type: "RECEIVE_MESSAGE",
				fromUserId: 0,
				content: "Tournament match ready",
				timestamp: Date.now(),
				messageType: "TOURNAMENT_MATCH_READY",
				metadata: {
					player1Name,
					player2Name,
					matchId,
					tournamentId,
				},
		});

		const client1 = this.clients.get(player1Id);
		if (client1?.readyState === WebSocket.OPEN) {
			client1.send(payload);
			//console.log(`[CHAT] Sent TOURNAMENT_MATCH_READY to ${player1Name}`);
		}

		const client2 = this.clients.get(player2Id);
		if (client2?.readyState === WebSocket.OPEN) {
          client2.send(payload);
          //console.log(`[CHAT] Sent TOURNAMENT_MATCH_READY to ${player2Name}`);
        }
	}
}

let chatGatewayInstance: ChatGateway | null = null;

export const setChatGateway = (instance: ChatGateway) => {
	chatGatewayInstance = instance;
};

export const getChatGateway = (): ChatGateway | null => {
	return chatGatewayInstance;
};
