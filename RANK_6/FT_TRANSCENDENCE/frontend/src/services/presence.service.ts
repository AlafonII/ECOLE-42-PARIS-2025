/**
 * Presence Service - manages WebSocket connection for real-time friend status
 * Singleton pattern ensures single WS connection across the app
 */

type PresenceListener = (userId: number, isOnline: boolean) => void;

class PresenceService {
	private static instance: PresenceService;
	private ws: WebSocket | null = null;
	private onlineUserIds: Set<number> = new Set();
	private listeners: Set<PresenceListener> = new Set();
	private reconnectTimeout: number | null = null;
	private isConnecting: boolean = false;
	private isAuthenticated: boolean = false;
	private shouldReconnect: boolean = true;
	private heartbeatInterval: number | null = null;
	private heartbeatTimeout: number | null = null;
	private readonly HEARTBEAT_INTERVAL = 30000; // 30 seconds
	private readonly HEARTBEAT_TIMEOUT = 10000; // 10 seconds

	private constructor() {}

	/**
	 * Get singleton instance
	 */
	static getInstance(): PresenceService {
		if (!PresenceService.instance) {
			PresenceService.instance = new PresenceService();
		}
		return PresenceService.instance;
	}

	/**
	 * Connect to WebSocket presence service
	 */
	connect(): void {
		if (this.ws?.readyState === WebSocket.OPEN || this.isConnecting) {
			//console.log('[PresenceService] Already connected or connecting');
			//console.log('[PresenceService] Already connected or connecting');
			return;
		}

		// Cancel any pending reconnect before connecting
		if (this.reconnectTimeout) {
			clearTimeout(this.reconnectTimeout);
			this.reconnectTimeout = null;
		}

		this.shouldReconnect = true;
		this.isConnecting = true;
		const protocol = window.location.protocol === "https:" ? "wss:" : "ws:";
		const wsUrl = `${protocol}//${window.location.host}/presence`;

		//console.log('[PresenceService] Connecting to:', wsUrl);
		//console.log('[PresenceService] Connecting to:', wsUrl);
		this.ws = new WebSocket(wsUrl);

		this.ws.onopen = () => {
			//console.log('[PresenceService] Connected');
			//console.log('[PresenceService] Connected');
			this.isConnecting = false;
			const authMsg = JSON.stringify({ type: "auth" });
			this.ws?.send(authMsg);
			this.startHeartbeat();
		};

		this.ws.onmessage = (event) => {
			this.handleMessage(event.data);
		};

		this.ws.onclose = (event) => {
			//console.log('[PresenceService] Disconnected - code:', event.code);
			//console.log('[PresenceService] Disconnected - code:', event.code);
			this.isConnecting = false;
			this.isAuthenticated = false;
			this.ws = null;
			this.onlineUserIds.clear();
			this.stopHeartbeat();

			if (this.shouldReconnect) {
				this.scheduleReconnect();
			}
		};

		this.ws.onerror = (error) => {
			console.error("[PresenceService] Error:", error);
			this.isConnecting = false;
		};
	}

	/**
	 * Disconnect from WebSocket
	 */
	disconnect(): void {
		this.shouldReconnect = false;

		if (this.reconnectTimeout) {
			clearTimeout(this.reconnectTimeout);
			this.reconnectTimeout = null;
		}

		this.stopHeartbeat();

		if (this.ws) {
			this.ws.close();
			this.ws = null;
		}

		this.onlineUserIds.clear();
		this.isConnecting = false;
		this.isAuthenticated = false;
		//console.log('[PresenceService] Disconnected');
		//console.log('[PresenceService] Disconnected');
	}

	/**
	 * Schedule reconnection attempt
	 */
	private scheduleReconnect(): void {
		if (this.reconnectTimeout) return;

		this.reconnectTimeout = window.setTimeout(() => {
			this.reconnectTimeout = null;
			//console.log('[PresenceService] Attempting to reconnect...');
			//console.log('[PresenceService] Attempting to reconnect...');
			this.connect();
		}, 5000);
	}

	/**
	 * Start heartbeat mechanism to keep connection alive
	 */
	private startHeartbeat(): void {
		if (this.heartbeatInterval) return;

		this.heartbeatInterval = window.setInterval(() => {
			if (this.ws?.readyState === WebSocket.OPEN) {
				this.ws.send(JSON.stringify({ type: "ping" }));

				// Set timeout for heartbeat response
				if (this.heartbeatTimeout) {
					clearTimeout(this.heartbeatTimeout);
				}
				this.heartbeatTimeout = window.setTimeout(() => {
					console.warn(
						"[PresenceService] Heartbeat timeout - connection may be dead"
					);
					this.ws?.close();
				}, this.HEARTBEAT_TIMEOUT);
			} else {
				console.warn(
					"[PresenceService] WebSocket not open during heartbeat"
				);
				this.stopHeartbeat();
			}
		}, this.HEARTBEAT_INTERVAL);
	}

	/**
	 * Stop heartbeat mechanism
	 */
	private stopHeartbeat(): void {
		if (this.heartbeatInterval) {
			clearInterval(this.heartbeatInterval);
			this.heartbeatInterval = null;
		}

		if (this.heartbeatTimeout) {
			clearTimeout(this.heartbeatTimeout);
			this.heartbeatTimeout = null;
		}

		//console.log('[PresenceService] Heartbeat stopped');
		//console.log('[PresenceService] Heartbeat stopped');
	}

	/**
	 * Handle incoming WebSocket messages
	 */
	private handleMessage(data: string): void {
		try {
			const message = JSON.parse(data);

			switch (message.type) {
				case 'auth_success':
					//console.log('[PresenceService] Authenticated');
					this.isAuthenticated = true;
					break;

				case "auth_error":
					console.error(
						"[PresenceService] Authentication failed:",
						message.error
					);
					this.isAuthenticated = false;
					this.shouldReconnect = false;
					if (this.reconnectTimeout) {
						clearTimeout(this.reconnectTimeout);
						this.reconnectTimeout = null;
					}
					this.ws?.close();
					break;

				case "online_friends":
					// Initial list of online friends
					if (message.data && Array.isArray(message.data)) {
						this.onlineUserIds.clear();
						message.data.forEach((friend: any) => {
							this.onlineUserIds.add(friend.id);
						});
						//console.log('[PresenceService] Online friends:', this.onlineUserIds.size);
						//console.log('[PresenceService] Online friends:', this.onlineUserIds.size);
						// Notify all listeners about initial state
						this.notifyListeners();
					}
					break;

				case "friend_online":
					this.onlineUserIds.add(message.userId);
					//console.log('[PresenceService] Friend online:', message.userId);
					//console.log('[PresenceService] Friend online:', message.userId);
					this.notifyListeners(message.userId, true);
					break;

				case "friend_offline":
					this.onlineUserIds.delete(message.userId);
					//console.log('[PresenceService] Friend offline:', message.userId);
					//console.log('[PresenceService] Friend offline:', message.userId);
					this.notifyListeners(message.userId, false);
					break;

				case "pong":
					if (this.heartbeatTimeout) {
						clearTimeout(this.heartbeatTimeout);
						this.heartbeatTimeout = null;
					}
					break;

				case "error":
					console.error(
						"[PresenceService] Server error:",
						message.message
					);
					break;

				default:
					console.warn(
						"[PresenceService] Unknown message type:",
						message.type
					);
			}
		} catch (error) {
			console.error("[PresenceService] Failed to parse message:", error);
		}
	}

	/**
	 * Notify all listeners about status change
	 */
	private notifyListeners(userId?: number, isOnline?: boolean): void {
		if (userId !== undefined && isOnline !== undefined) {
			// Specific user status changed
			this.listeners.forEach((listener) => listener(userId, isOnline));
		} else {
			// Initial load - notify about all online users
			this.onlineUserIds.forEach((id) => {
				this.listeners.forEach((listener) => listener(id, true));
			});
		}
	}

	/**
	 * Subscribe to presence updates
	 * @returns Unsubscribe function
	 */
	subscribe(listener: PresenceListener): () => void {
		this.listeners.add(listener);

		// Immediately notify about current online users
		this.onlineUserIds.forEach((userId) => {
			listener(userId, true);
		});

		// Return unsubscribe function
		return () => {
			this.listeners.delete(listener);
		};
	}

	/**
	 * Check if user is online
	 */
	isOnline(userId: number): boolean {
		return this.onlineUserIds.has(userId);
	}

	/**
	 * Get all online user IDs
	 */
	getOnlineUsers(): number[] {
		return Array.from(this.onlineUserIds);
	}

	/**
	 * Send ping to keep connection alive
	 */
	ping(): void {
		if (this.ws?.readyState === WebSocket.OPEN) {
			this.ws.send(JSON.stringify({ type: "ping" }));
		}
	}

	/**
	 * Get connection status
	 */
	getConnectionStatus(): {
		isConnected: boolean;
		isAuthenticated: boolean;
		isConnecting: boolean;
	} {
		return {
			isConnected: this.ws?.readyState === WebSocket.OPEN,
			isAuthenticated: this.isAuthenticated,
			isConnecting: this.isConnecting,
		};
	}

	/**
	 * Check if authenticated
	 */
	authenticated(): boolean {
		return this.isAuthenticated;
	}

	/**
	 * Check if connected
	 */
	connected(): boolean {
		return this.ws?.readyState === WebSocket.OPEN || false;
	}
}

// Export singleton instance
export const presenceService = PresenceService.getInstance();
