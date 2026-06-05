import { ChatWSMessage, ChatReceiveMessage } from "./chat.types";

let socket: WebSocket | null = null;
let reconnectTimer: number | null = null;
let messageHandler: ((msg: ChatReceiveMessage) => void) | null = null;
let isIntentionallyClosed = false;
let reconnectAttempts = 0;
const MAX_RECONNECT_ATTEMPTS = 5;
const RECONNECT_DELAY = 3000;

export const connectChat = async (
	onMessage: (msg: ChatReceiveMessage) => void
): Promise<WebSocket> => {
	messageHandler = onMessage;

	if (socket && socket.readyState === WebSocket.OPEN) return socket;

	if (socket && socket.readyState === WebSocket.CONNECTING) {
		return new Promise((resolve, reject) => {
			const checkConnection = setInterval(() => {
				if (socket && socket.readyState === WebSocket.OPEN) {
					clearInterval(checkConnection);
					resolve(socket);
				} else if (socket && socket.readyState === WebSocket.CLOSED) {
					clearInterval(checkConnection);
					reject(new Error("Connection failed"));
				}
			}, 100);
		});
	}

	isIntentionallyClosed = false;

	const response = await fetch("/api/auth/ws-token", {
		credentials: "include",
		headers: { "Content-Type": "application/json" },
	});

	if (!response.ok) {
		throw new Error("Failed to authenticate for WebSocket");
	}

	const { token } = await response.json();

	const wsProtocol = window.location.protocol === "https:" ? "wss:" : "ws:";
	const wsHost = window.location.host;

	socket = new WebSocket(`${wsProtocol}//${wsHost}/ws/chat?token=${token}`);

	socket.onopen = () => {
		reconnectAttempts = 0;
	};

	socket.onmessage = (event) => {
		try {
			const msg: ChatWSMessage = JSON.parse(event.data);
			if (msg.type === "RECEIVE_MESSAGE" && messageHandler) {
				messageHandler(msg);
			}
		} catch (err) {
			console.error("[CHAT] Invalid message", err);
		}
	};

	socket.onerror = (error) => {
		console.error("[CHAT] WebSocket error:", error);
	};

	socket.onclose = (event) => {
		socket = null;
		console.warn(`[CHAT] Socket closed`, event.code, event.reason);

		if (isIntentionallyClosed) {
			return;
		}

		if (reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) {
			console.error("[CHAT] Max reconnection attempts reached");
			return;
		}

		reconnectAttempts++;

		if (reconnectTimer) {
			clearTimeout(reconnectTimer);
		}

		reconnectTimer = window.setTimeout(async () => {
			try {
				if (messageHandler) {
					await connectChat(messageHandler);
				}
			} catch (err) {
				console.error("[CHAT] Reconnection failed:", err);
			}
		}, RECONNECT_DELAY);
	};

	return socket;
};

export const disconnectChat = () => {
	isIntentionallyClosed = true;

	if (reconnectTimer) {
		clearTimeout(reconnectTimer);
		reconnectTimer = null;
	}

	if (socket) {
		socket.close(1000, "User disconnected");
		socket = null;
	}

	messageHandler = null;
	reconnectAttempts = 0;
};

export const sendDirectMessage = (toUserId: number, content: string) => {
	if (!socket || socket.readyState !== WebSocket.OPEN) {
		console.error("[CHAT] Cannot send message: socket not connected");
		return;
	}

	socket.send(
		JSON.stringify({
			type: "DIRECT_MESSAGE",
			toUserId,
			content,
		})
	);
};

export const sendBroadcast = (content: string) => {
	if (!socket || socket.readyState !== WebSocket.OPEN) {
		console.error("[CHAT] Cannot send broadcast: socket not connected");
		return;
	}

	socket.send(
		JSON.stringify({
			type: "BROADCAST",
			content,
		})
	);
};

export const isConnected = (): boolean => {
	return socket !== null && socket.readyState === WebSocket.OPEN;
};
