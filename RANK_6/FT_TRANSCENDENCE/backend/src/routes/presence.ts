import { FastifyInstance } from "fastify";
import { getQueries } from "../database/index.ts";
import { WebSocket } from "ws";

/**
 * Interface representing a connected user via WebSocket
 */
interface ConnectedUser {
	userId: number;
	ws: WebSocket;
	connectedAt: Date;
}

/**
 * Interface for tipization incoming messages from the client
 */
interface PresenceMessage {
	type: "auth" | "ping";
}

export const connectedUsers = new Map<string, ConnectedUser>();

/**
 * Check if user is currently online
 */
function isUserOnline(userId: number): boolean {
	for (const conn of connectedUsers.values()) {
		if (conn.userId === userId && conn.ws.readyState === WebSocket.OPEN) {
			return true;
		}
	}
	return false;
}

/**
 * Get set of all online user IDs, optionally excluding a specific user
 */
function getOnlineUserIds(excludeUserId?: number): Set<number> {
	const onlineUserIds = new Set<number>();
	for (const conn of connectedUsers.values()) {
		if (conn.ws.readyState === WebSocket.OPEN) {
			if (!excludeUserId || conn.userId !== excludeUserId) {
				onlineUserIds.add(conn.userId);
			}
		}
	}
	return onlineUserIds;
}

/**
 * Notify specific users about online status
 */
function notifyUserStatus(
	targetUserIds: number[],
	aboutUserId: number,
	status: "online" | "offline"
): void {
	const messageType = status === "online" ? "friend_online" : "friend_offline";
	const message = JSON.stringify({ type: messageType, userId: aboutUserId });
	
	for (const conn of connectedUsers.values()) {
		if (conn.ws.readyState === WebSocket.OPEN && targetUserIds.includes(conn.userId)) {
			conn.ws.send(message);
		}
	}
}

/**
 * Notify both users about new friendship if they are online
 */
export async function notifyNewFriendship(userId1: number, userId2: number) {
	try {
		if (isUserOnline(userId2)) {
			notifyUserStatus([userId1], userId2, "online");
		}
		
		if (isUserOnline(userId1)) {
			notifyUserStatus([userId2], userId1, "online");
		}
	} catch (error) {
		console.error("Failed to notify about new friendship:", error);
	}
}

/**
 * Callback for notifying friends when user goes offline (used by logout)
 */
let onUserLogout: ((userId: number) => Promise<void>) | null = null;

export function setLogoutHandler(handler: (userId: number) => Promise<void>) {
	onUserLogout = handler;
}

/**
 * Disconnect all WebSocket connections for a user
 */
export async function disconnectUser(userId: number): Promise<number> {
	if (onUserLogout) {
		await onUserLogout(userId);
	}
	
	const closedConnections: string[] = [];
	
	for (const [connId, conn] of connectedUsers.entries()) {
		if (conn.userId === userId) {
			conn.ws.close(1000, "User logged out");
			connectedUsers.delete(connId);
			closedConnections.push(connId);
		}
	}
	
	if (closedConnections.length > 0) {
		//console.log(`[Presence] Closed ${closedConnections.length} WebSocket connections for user ${userId}`);
		//console.log(`[Presence] Closed ${closedConnections.length} WebSocket connections for user ${userId}`);
	}
	
	return closedConnections.length;
}

/**
 * Register presence WebSocket routes
 */
export async function presenceRoutes(fastify: FastifyInstance) {
	const queries = getQueries();

	/**
	 * Notify all online friends about user's status change
	 */
	async function notifyFriends(userId: number, status: "online" | "offline") {
		try {
			const friends = await queries.getFriends(userId);
			const friendIds = friends.map(f => f.id);
			notifyUserStatus(friendIds, userId, status);
		} catch (error) {
			console.error("Failed to notify friends:", error);
		}
	}

	// Register logout handler to notify friends when user logs out
	setLogoutHandler((userId) => notifyFriends(userId, "offline"));

	fastify.get("/presence", { websocket: true }, (socket, req) => {
		let userId: number | null = null;
		let uniqConnectionId: string | null = null;

		//console.log("[Presence] New WebSocket connection established");
		//console.log("[Presence] New WebSocket connection established");

		/**
		 * Incoming message handler
		 */
		socket.on("message", async (data: any) => {
			try {
				const message = JSON.parse(data.toString()) as PresenceMessage;
				//console.log("[Presence] Received message:", message.type, userId ? `from user ${userId}` : '(not authenticated)');
				//console.log("[Presence] Received message:", message.type, userId ? `from user ${userId}` : '(not authenticated)');

				switch (message.type) {
					case "auth":
						try {
							const token = req.cookies?.access_token;
							if (!token) {
								//console.log("[Presence] No token provided");
								//console.log("[Presence] No token provided");
								socket.send(
									JSON.stringify({
										type: "auth_error",
										error: "No token provided",
									})
								);
								socket.close();
								return;
							}

							//console.log("[Presence] Token found, verifying...");
							//console.log("[Presence] Token found, verifying...");
							const payload: any = await fastify.jwt.verify(token);
							userId = payload.id as number;
							//console.log("[Presence] User authenticated:", userId);
							//console.log("[Presence] User authenticated:", userId);

							uniqConnectionId = `${userId}_${Date.now()}`;

							connectedUsers.set(uniqConnectionId, {
								userId,
								ws: socket,
								connectedAt: new Date(),
							});
							//console.log(`[Presence] User ${userId} added to connected users. Total connections: ${connectedUsers.size}`);
							//console.log(`[Presence] User ${userId} added to connected users. Total connections: ${connectedUsers.size}`);

							await queries.updateUserLastSeen(userId);

							socket.send(
								JSON.stringify({
									type: "auth_success",
									data: { userId },
								})
							);

							// Get online friends based on actual WebSocket connections
							const onlineUserIds = getOnlineUserIds(userId);
							
							// Filter friends list to only include those actually online
							const allFriends = await queries.getFriends(userId);
							const onlineFriends = allFriends.filter(f => onlineUserIds.has(f.id));
							//console.log(`[Presence] Sending ${onlineFriends.length} online friends to user ${userId}`);

							socket.send(
								JSON.stringify({
									type: "online_friends",
									data: onlineFriends,
								})
							);

							// Notify friends that this user is now online
							//console.log(`[Presence] Notifying friends that user ${userId} is online`);
							//console.log(`[Presence] Notifying friends that user ${userId} is online`);
							await notifyFriends(userId, "online");
						} catch (error) {
							console.error("[Presence] Auth error:", error);
							socket.send(
								JSON.stringify({
									type: "auth_error",
									error: "Invalid token",
								})
							);
							socket.close();
						}
						break;

					case "ping":
						if (userId) {
							//console.log(`[Presence] Ping from user ${userId}`);
							//console.log(`[Presence] Ping from user ${userId}`);
							await queries.updateUserLastSeen(userId);
						} else {
							//console.log("[Presence] Ping from unauthenticated connection");
							//console.log("[Presence] Ping from unauthenticated connection");
						}
						socket.send(JSON.stringify({ type: "pong" }));
					break;

					default:
					console.warn(`[Presence] Unknown message type: ${message.type}`);
						socket.send(
							JSON.stringify({
								type: "error",
								message: `Unknown message type: ${message.type}`,
							})
						);
					break;
				}
			} catch (err) {
				console.error("[Presence] Message parsing error:", err);
				socket.send(
					JSON.stringify({
						type: "error",
						message: "Invalid message",
					})
				);
			}
		});

		socket.on("close", async () => {
			if (userId && uniqConnectionId) {
				/*console.log(
					`[Presence] User ${userId} disconnected (${uniqConnectionId})`
				);*/

				connectedUsers.delete(uniqConnectionId);

				let hasOtherConnections = false;
				for (const conn of connectedUsers.values()) {
					if (conn.userId === userId) {
						hasOtherConnections = true;
						break;
					}
				}
				if (!hasOtherConnections) {
					//console.log(`[Presence] User ${userId} is offline`);
					//console.log(`[Presence] User ${userId} is offline`);
					
					await notifyFriends(userId, "offline");
				} else {
					let activeCount = 0;
					for (const conn of connectedUsers.values()) {
						if (conn.userId === userId) {
							activeCount++;
						}
				}
					/*console.log(
						`[Presence] User ${userId} still has ${activeCount} active ${activeCount === 1 ? 'connection' : 'connections'}`
					);*/
				}
			}
		});

		socket.on("error", (error: any) => {
			console.error("[Presence] Socket error:", error);
		});
	});
	
	const heartbeatInterval = setInterval(() => {
		let cleanedCount = 0;

		connectedUsers.forEach((user, connId) => {
			if (user.ws.readyState === WebSocket.CLOSING || user.ws.readyState === WebSocket.CLOSED) {
				connectedUsers.delete(connId);
				cleanedCount++;
				/*console.log(
					`Cleaned up dead connection for user ${user.userId}`
				);*/
			}
		});

		if (cleanedCount > 0) {
			/*console.log(
				`Heartbeat: Cleaned ${cleanedCount} dead ${cleanedCount === 1 ? 'connection' : 'connections'}`
			);*/
		}
	}, 30000);


	fastify.addHook("onClose", async () => {
		//console.log("Shutting down presence service...");
		//console.log("Shutting down presence service...");

		clearInterval(heartbeatInterval);

		connectedUsers.forEach((user, connId) => {
			if (user.ws.readyState === WebSocket.OPEN || user.ws.readyState === WebSocket.CONNECTING) {
				user.ws.close(1001, "Server shutting down");
			}
		});

		//console.log("Presence service shut down complete");
		//console.log("Presence service shut down complete");
	});

	//console.log("Presence WebSocket service initialized");
	//console.log("Presence WebSocket service initialized");
}
