import { getMatchIdFromPath } from "../pages/PongGame";
import { GameSnapshot, CONFIG } from "./types";

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

let activeWebSocket: WebSocket | null = null;
let gameOver = false;
let renderLoopActive = false;
let keydownHandler: ((e: KeyboardEvent) => void) | null = null;
let keyupHandler: ((e: KeyboardEvent) => void) | null = null;

export function initPongClient() {
	closePongConnection();
	if (
		activeWebSocket &&
		(activeWebSocket.readyState === WebSocket.OPEN ||
			activeWebSocket.readyState === WebSocket.CONNECTING)
	) {
		//console.log("Already connected or connecting");
		return;
	}

	//console.log("Creating new WebSocket");

	const canvas = document.getElementById("gameCanvas") as HTMLCanvasElement;
	const ctx = canvas.getContext("2d")!;

	gameOver = false;
	renderLoopActive = true;
	let currentSnapshot: GameSnapshot | null = null;
	let playerNum: 1 | 2 | null = null;
	let statusMessage: string = "Connecting...";
	const rootStyle = getComputedStyle(document.documentElement);
	const headerFont =
		rootStyle.getPropertyValue("--font-header").trim() || "sans-serif";
	const gameYellow =
		rootStyle.getPropertyValue("--game-yellow").trim() || "#ffeb3b";
	const player1Color =
		rootStyle.getPropertyValue("--player-2").trim() || "#448aff";
	const player2Color =
		rootStyle.getPropertyValue("--player-1").trim() || "#ff5252";
	const protocol = window.location.protocol === "https:" ? "wss:" : "ws:";
	const baseUrl = `${protocol}//${window.location.host}/pong/play`;
	const matchId = getMatchIdFromPath();
	const wsUrl = matchId !== null ? `${baseUrl}?matchId=${matchId}` : baseUrl;
	const ws = new WebSocket(wsUrl);
	activeWebSocket = ws;
	ws.onopen = () => {
		statusMessage = "Connected...";
	};
	ws.onmessage = (event) => {
		try {
			const text = typeof event.data === "string" ? event.data : "";
			if (!text) {
				console.warn("WS message not string:", event.data);
				return;
			}
			const message = JSON.parse(text);
			handleServerMessage(message);
		} catch (err) {
			console.error("Failed to parse WS message:", err, event.data);
		}
	};
	ws.onclose = () => {
		if (!gameOver) statusMessage = "Disconnected";
		activeWebSocket = null;
		if (keydownHandler)
			window.removeEventListener("keydown", keydownHandler);
		if (keyupHandler) window.removeEventListener("keyup", keyupHandler);
	};
	ws.onerror = (e) => {
		console.error("WS error", e);
		statusMessage = "WebSocket error (check URL / server / proxy)";
	};

	function handleServerMessage(message: ServerMessage): void {
		switch (message.type) {
			case "waiting":
				statusMessage = message.message;
				break;
			case "error":
				statusMessage = message.message;
				break;
			case "player_assigned":
				playerNum = message.playerNum;
				break;
			case "game_start":
				statusMessage = "Game starting...";
				break;
			case "state":
				currentSnapshot = message.snapshot;
				break;
			case "game_over":
				gameOver = true;
				//console.log("Game over received:", message);
				if (message.reason === "opponent_left")
					statusMessage = "Opponent disconnected - You win!";
				else if (message.reason === "opponent_inactive")
					statusMessage = "Opponent inactive - You win!";
				else if (message.reason === "you_inactive")
					statusMessage = "You were inactive too long - You lose!";
				else if (message.reason === "you_forfeit")
					statusMessage = "You forfeited - You lose!";
				else if (message.reason === "opponent_forfeit")
					statusMessage = "Opponent forfeited - You win!";
				else {
					const myScore =
						playerNum === 1
							? message.score.player1
							: message.score.player2;
					const opponentScore =
						playerNum === 1
							? message.score.player2
							: message.score.player1;
					if (playerNum === message.winner)
						statusMessage = `YOU WIN!\nFINAL SCORE: ${myScore} - ${opponentScore}`;
					else
						statusMessage = `YOU LOSE!\nFINAL SCORE: ${myScore} - ${opponentScore}`;
				}
				currentSnapshot = null;
				ws.close();
				activeWebSocket = null;
				window.dispatchEvent(new CustomEvent("pong:remoteGameOver"));
				break;
		}
	}

	keydownHandler = (e: KeyboardEvent) => {
		if (ws.readyState === WebSocket.OPEN) {
			if (e.key === " ") ws.send(JSON.stringify({ type: "ready" }));
			else ws.send(JSON.stringify({ type: "keydown", key: e.key }));
		}
	};

	keyupHandler = (e: KeyboardEvent) => {
		if (ws.readyState === WebSocket.OPEN)
			ws.send(JSON.stringify({ type: "keyup", key: e.key }));
	};

	window.addEventListener("keydown", keydownHandler);
	window.addEventListener("keyup", keyupHandler);

	function renderLoop(): void {
		if (!renderLoopActive) return;
		draw();
		requestAnimationFrame(renderLoop);
	}

	function drawRackets(): void {
		if (!currentSnapshot) return;
		ctx!.fillStyle = player1Color;
		ctx?.fillRect(
			currentSnapshot.player1.racket.x -
				currentSnapshot.player1.racket.width,
			currentSnapshot.player1.racket.y -
				0.5 * currentSnapshot.player1.racket.height,
			currentSnapshot.player1.racket.width,
			currentSnapshot.player1.racket.height
		);
		ctx!.fillStyle = player2Color;
		ctx?.fillRect(
			currentSnapshot.player2.racket.x,
			currentSnapshot.player2.racket.y -
				0.5 * currentSnapshot.player2.racket.height,
			currentSnapshot.player2.racket.width,
			currentSnapshot.player2.racket.height
		);
	}

	function drawBorders(): void {
		ctx!.fillStyle = "white";
		ctx?.fillRect(
			5,
			CONFIG.borderGap,
			CONFIG.canvasWidth - 20,
			CONFIG.borderThickness
		);
		ctx?.fillRect(
			5,
			CONFIG.canvasHeight - CONFIG.borderGap - CONFIG.borderThickness,
			CONFIG.canvasWidth - 20,
			CONFIG.borderThickness
		);
	}

	function drawBall(): void {
		if (!currentSnapshot) return;
		ctx!.fillStyle = gameYellow;
		ctx?.beginPath();
		ctx?.arc(
			currentSnapshot.ball.pos.x,
			currentSnapshot.ball.pos.y,
			currentSnapshot.ball.size / 2,
			0,
			Math.PI * 2
		);
		ctx!.fill();
	}

	function drawScore(): void {
		if (!currentSnapshot) return;
		ctx!.font = `48px ${headerFont}`;
		ctx!.fillStyle = gameYellow;
		const score: string =
			currentSnapshot.player1.score.toString() +
			" - " +
			currentSnapshot.player2.score.toString();
		const textWidth = ctx?.measureText(score).width;
		const x = (CONFIG.canvasWidth - textWidth!) / 2;
		ctx?.fillText(score, x, 100);
	}

	function drawCountdown() {
		if (!currentSnapshot) return;
		ctx!.font = `72px ${headerFont}`;
		const countdown = Math.ceil(currentSnapshot.countdown).toString();
		const countdownWidth = ctx?.measureText(countdown).width;
		const countdownX = (CONFIG.canvasWidth - countdownWidth!) / 2;
		ctx!.fillStyle = gameYellow;
		ctx?.fillText(countdown, countdownX, CONFIG.canvasHeight / 2 - 50);
	}

	function draw() {
		if (currentSnapshot === null) {
			drawStatusMessage();
			return;
		}
		ctx?.clearRect(0, 0, CONFIG.canvasWidth, CONFIG.canvasHeight);
		ctx!.fillStyle = "black";
		ctx?.fillRect(0, 0, CONFIG.canvasWidth, CONFIG.canvasHeight);

		drawBorders();
		drawRackets();
		drawBall();
		drawScore();
		if (
			currentSnapshot.state === "countdown" ||
			currentSnapshot.countdown > 0
		)
			drawCountdown();
		if (currentSnapshot.state === "finished") drawFinalScore();
	}

	function drawFinalScore() {
		if (currentSnapshot === null) {
			//console.log(statusMessage);
			return;
		}
		const winner: string =
			currentSnapshot.player1.score > currentSnapshot.player2.score
				? "PLAYER 1 WINS"
				: "PLAYER 2 WINS";
		ctx!.font = `64px ${headerFont}`;
		ctx!.fillStyle = gameYellow;
		const textWidth = ctx?.measureText(winner).width;
		const x = (CONFIG.canvasWidth - textWidth!) / 2;
		ctx?.fillText(winner, x, CONFIG.canvasHeight / 2 + 100);
	}

	function drawStatusMessage(): void {
		ctx?.clearRect(0, 0, CONFIG.canvasWidth, CONFIG.canvasHeight);
		ctx!.fillStyle = "black";
		ctx?.fillRect(0, 0, CONFIG.canvasWidth, CONFIG.canvasHeight);
		ctx!.fillStyle = gameYellow;
		ctx!.font = `24px ${headerFont}`;

		let display = statusMessage;
		if (!gameOver && display.toLowerCase().includes("waiting")) {
			display += "\nSelect a game mode";
		}

		const lines = display.split("\n");
		const lineHeight = 35;
		const startY =
			CONFIG.canvasHeight / 2 - ((lines.length - 1) * lineHeight) / 2;

		lines.forEach((line, index) => {
			const textWidth = ctx.measureText(line).width;
			const x = (CONFIG.canvasWidth - textWidth!) / 2;
			const y = startY + index * lineHeight;
			ctx.fillText(line, x, y);
		});
	}

	requestAnimationFrame(renderLoop);
}

export function closePongConnection(): void {
	renderLoopActive = false;
	if (activeWebSocket) {
		activeWebSocket.close();
		activeWebSocket = null;
	}
	if (keydownHandler) {
		window.removeEventListener("keydown", keydownHandler);
		keydownHandler = null;
	}
	if (keyupHandler) {
		window.removeEventListener("keyup", keyupHandler);
		keyupHandler = null;
	}
}

export function sendPongForfeit() {
	if (activeWebSocket && activeWebSocket.readyState === WebSocket.OPEN) {
		try {
			activeWebSocket.send(JSON.stringify({ type: "forfeit" }));
		} catch (e) {
			console.warn("Failed to send forfeit:", e);
		}
	}
}
