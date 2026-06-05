//- Manage connections (WebSocket)
//- PongEngine with a loop (60 times / second)

import { WebSocket } from "ws";
import { PongEngine, GameSnapshot, PlayerInput, CONFIG } from "./PongEngine";

type PlayerConnection = {
	ws: WebSocket;
	playerNum: 1 | 2;
	userId?: number;
	lastActivity: number;
};

//message sent by the client
type ClientMessage =
	| { type: "keydown"; key: string }
	| { type: "keyup"; key: string }
	| { type: "ready" }
	| { type: "forfeit" };

//message sent by the server
type ServerMessage =
	| { type: "waiting"; message: string }
	| { type: "error"; message: string }
	| { type: "player_assigned"; playerNum: 1 | 2 }
	| { type: "game_start"; countdown: number }
	| { type: "state"; snapshot: GameSnapshot }
	| {
			type: "game_over";
			winner: 1 | 2;
			score: { player1: number; player2: number };
			reason?:
				| "finished"
				| "opponent_left"
				| "opponent_inactive"
				| "you_inactive"
				| "you_forfeit"
				| "opponent_forfeit";
	  };

export class GameRoom {
	id: string; //room id
	private engine: PongEngine;
	private players: Map<WebSocket, PlayerConnection> = new Map(); // one player one websocket
	private gameLoopInterval: NodeJS.Timeout | null = null; //interval to stop the game
	private lastUpdateTime: number = 0;
	private readyPlayers: Set<WebSocket> = new Set(); //to know if the player has pressed SPACE and he is ready

	//CALLBACKS
	//to notify DB when game ends
	onGameEnd?: (
		winner: 1 | 2,
		scores: { player1: number; player2: number },
		durationMs: number,
		reason:
			| "finished"
			| "opponent_left"
			| "opponent_inactive"
			| "you_inactive"
			| "forfeit",
	) => void;

	//when players left the room, to delete the room
	onEmpty?: (roomId: string) => void;

	private gameStartTime: number = 0;

	constructor(id: string) {
		this.id = id;
		this.engine = new PongEngine();
	}

	get playerCount(): number {
		return this.players.size;
	}

	isFull(): boolean {
		return this.players.size >= 2;
	}

	addPlayer(ws: WebSocket, userId?: number): boolean {
		if (this.isFull()) return false;
		if (userId) {
			for (const player of this.players.values()) {
				if (player.userId === userId) return false;
			}
		}

		const playerNum: 1 | 2 = this.players.size === 0 ? 1 : 2;

		this.players.set(ws, {
			ws,
			playerNum,
			userId,
			lastActivity: Date.now(),
		}); //to store player info
		/*console.log(
			"addPlayer: readyState =",
			ws.readyState,
			"hasSend=",
			typeof (ws as any).send,
		);*/
		this.send(ws, { type: "player_assigned", playerNum });

		if (this.isFull()) {
			this.broadcast({
				type: "waiting",
				message: "Both players connected\nPRESS SPACE when ready...",
			});
		} else {
			this.send(ws, {
				type: "waiting",
				message: "Waiting for another player...",
			});
		}

		ws.on("message", (data) => this.handleMessage(ws, data.toString())); //when player sends a message
		ws.on("close", () => this.removePlayer(ws)); //to remove the player (disconnection)

		return true;
	}

	// retrieve the user ids to use them for addPlayers() when you create a remote match
	getPlayerUserIds(): { player1Id?: number; player2Id?: number } {
		let player1Id: number | undefined;
		let player2Id: number | undefined;

		for (const player of this.players.values()) {
			if (player.playerNum === 1) player1Id = player.userId;
			else if (player.playerNum === 2) player2Id = player.userId;
		}
		return { player1Id, player2Id };
	}

	// remove player if disconnection or waiting message if waiting for an opponent
	private removePlayer(ws: WebSocket): void {
		const player = this.players.get(ws);
		if (!player) return;
		this.players.delete(ws);
		this.readyPlayers.delete(ws);

		if (
			this.engine.state === "in_progress" ||
			this.engine.state === "countdown"
		) {
			this.stopGameLoop();

			const winner = player.playerNum === 1 ? 2 : 1;
			const reason = "opponent_left";
			const scores =
				winner === 1
					? { player1: 5, player2: 0 }
					: { player1: 0, player2: 5 };
			this.broadcast({
				type: "game_over",
				winner,
				score: scores,
				reason: reason,
			});

			if (this.onGameEnd) {
				this.onGameEnd(
					winner,
					scores,
					Date.now() - this.gameStartTime,
					reason,
				);
			}
		} else {
			this.broadcast({
				type: "waiting",
				message: "Other player disconnected. Game cancelled.",
			});
		}
		if (this.players.size === 0 && this.onEmpty) {
			this.onEmpty(this.id);
		}
	}

	private handleMessage(ws: WebSocket, rawData: string): void {
		const player = this.players.get(ws); //find the player who has sent a message (= pressed a key)
		if (!player) return;
		player.lastActivity = Date.now(); //saving last activity time to cancel the game in case of timeout
		let message: ClientMessage;
		try {
			message = JSON.parse(rawData);
		} catch {
			console.error("Invalid JSON from client:", rawData);
			return;
		}

		switch (message.type) {
			case "ready":
				this.handleReady(ws);
				break;
			case "keydown":
				this.handleKeydown(player, message.key);
				break;
			case "keyup":
				this.handleKeyup(player, message.key);
				break;
			case "forfeit":
				this.handleForfeit(player);
				break;
		}
	}

	//when player presses space bar and he is ready to play
	private handleReady(ws: WebSocket): void {
		if (this.engine.state !== "pending") return;
		this.readyPlayers.add(ws);
		if (this.readyPlayers.size === 2 && this.isFull()) this.startGame();
	}

	//Press a key (update the input to calculate the new position)
	private handleKeydown(player: PlayerConnection, key: string): void {
		const input = this.keyToInput(player.playerNum, key);
		if (input) {
			this.engine.setPlayerInput(player.playerNum, input);
		}
	}

	//Release a key
	private handleKeyup(player: PlayerConnection, key: string): void {
		const input = this.keyToInput(player.playerNum, key);
		if (input) {
			const currentInput =
				player.playerNum === 1
					? this.engine.player1.input
					: this.engine.player2.input;
			if (currentInput === input) {
				//to be sure for the key, if a player presses 2 keys
				this.engine.setPlayerInput(player.playerNum, "idle");
			}
		}
	}

	private handleForfeit(player: PlayerConnection): void {
		//console.log(`Player ${player.playerNum} forfeited`);
		//console.log(`Player ${player.playerNum} forfeited`);

		if (
			this.engine.state !== "in_progress" &&
			this.engine.state !== "countdown"
		)
			return;

		this.stopGameLoop();

		const winner: 1 | 2 = player.playerNum === 1 ? 2 : 1;
		const scores =
			winner === 1
				? { player1: 5, player2: 0 }
				: { player1: 0, player2: 5 };

		for (const p of this.players.values()) {
			if (p.playerNum === player.playerNum) {
				this.send(p.ws, {
					type: "game_over",
					winner,
					score: scores,
					reason: "you_forfeit" as any,
				});
			} else {
				this.send(p.ws, {
					type: "game_over",
					winner,
					score: scores,
					reason: "opponent_forfeit" as any,
				});
			}
		}

		if (this.onGameEnd) {
			this.onGameEnd(
				winner,
				scores,
				Date.now() - this.gameStartTime,
				"forfeit" as any,
			);
		}

		this.closeAllConnections();
	}

	//convert a key into a string
	private keyToInput(playerNum: 1 | 2, key: string): PlayerInput | null {
		if (playerNum === 1) {
			if (key === "w" || key === "W" || key === "ArrowUp") return "up";
			if (key === "s" || key === "S" || key === "ArrowDown")
				return "down";
		} else {
			if (key === "w" || key === "W" || key === "ArrowUp") return "up";
			if (key === "s" || key === "S" || key === "ArrowDown")
				return "down";
		}
		return null;
	}

	private startGame(): void {
		this.engine.startGame();
		this.gameStartTime = Date.now();
		this.broadcast({
			type: "game_start",
			countdown: CONFIG.countdownDuration,
		});
		this.startGameLoop();
	}

	private endGame(): void {
		this.stopGameLoop();

		const winner: 1 | 2 = this.engine.winner === "left" ? 1 : 2;
		const scores = {
			player1: this.engine.player1.score,
			player2: this.engine.player2.score,
		};

		const durationMs = Date.now() - this.gameStartTime; // game duration

		const reason = "finished";
		this.broadcast({
			type: "game_over",
			winner,
			score: scores,
			reason: reason,
		});

		if (this.onGameEnd) {
			this.onGameEnd(winner, scores, durationMs, reason); //call the callback to store info game in DB
		}
		this.closeAllConnections();
	}

	private readonly INACTIVITY_TIMEOUT = 30000; //30sec

	private startGameLoop(): void {
		const TICK_RATE = 60; //  60 update / sec
		const TICK_INTERVAL = 1000 / TICK_RATE; //  1 update / 16.67ms
		this.lastUpdateTime = Date.now();

		this.gameLoopInterval = setInterval(() => {
			const now = Date.now();
			for (const player of this.players.values()) {
				if (now - player.lastActivity >= this.INACTIVITY_TIMEOUT) {
					this.stopGameLoop();

					const winner = player.playerNum === 1 ? 2 : 1;
					const scores =
						winner === 1
							? { player1: 5, player2: 0 }
							: { player1: 0, player2: 5 };
					for (const p of this.players.values()) {
						if (p.playerNum === player.playerNum) {
							this.send(p.ws, {
								type: "game_over",
								winner,
								score: scores,
								reason: "you_inactive",
							});
						} else {
							this.send(p.ws, {
								type: "game_over",
								winner,
								score: scores,
								reason: "opponent_inactive",
							});
						}
					}
					const dbReason = "opponent_inactive";
					if (this.onGameEnd) {
						this.onGameEnd(
							winner,
							scores,
							Date.now() - this.gameStartTime,
							dbReason,
						);
					}
					this.closeAllConnections();
					return;
				}
			}
			const deltaTime = (now - this.lastUpdateTime) / 1000;
			this.lastUpdateTime = now;
			this.engine.update(deltaTime);
			const snapshot = this.engine.getSnapshot();
			this.broadcast({ type: "state", snapshot }); // send a snapshot of the game (all positions) to the players => SAME GAME for both players
			if (this.engine.state === "finished") this.endGame();
		}, TICK_INTERVAL);
	}

	private stopGameLoop(): void {
		if (this.gameLoopInterval) {
			clearInterval(this.gameLoopInterval);
			this.gameLoopInterval = null;
		}
	}

	private closeAllConnections(): void {
		for (const [ws] of this.players) {
			ws.close();
		}
	}

	private send(ws: WebSocket, message: ServerMessage): void {
		if (!ws) return;
		const payload = JSON.stringify(message);

		if (ws.readyState === WebSocket.OPEN) {
			ws.send(payload);
			return;
		}
		console.warn(
			`[GameRoom ${this.id}] Dropped message because readyState=${ws.readyState}`,
			message.type,
		);
	}

	private broadcast(message: ServerMessage): void {
		for (const [ws] of this.players) {
			this.send(ws, message);
		}
	}
}
