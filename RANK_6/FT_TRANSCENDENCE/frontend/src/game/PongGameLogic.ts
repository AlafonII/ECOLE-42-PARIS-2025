import { createAiController, AiDifficulty } from "../services/AI";

type Vector = {
	x: number;
	y: number;
};

type Ball = {
	pos: Vector;
	v: Vector;
	size: number;
};

type Racket = {
	y: number;
	x: number;
	speedY: number;
	width: number;
	height: number;
};

type Side = "left" | "right";

type Player = {
	racket: Racket;
	side: Side;
	score: number;
};

type State = "pending" | "in pause" | "in_progress" | "finished";

type GameEngine = {
	player1: Player;
	player2: Player;
	ball: Ball;
	state: State;
	points: number;
};

export function initPongGame() {
	var canvasEl = document.getElementById("gameCanvas");
	if (!canvasEl) throw new Error("Canvas element not found");
	var canvas = canvasEl as HTMLCanvasElement;
	const ctx = canvas.getContext("2d");
	if (!ctx) throw new Error("Failed to get 2D rendering context");

	const rootStyle = getComputedStyle(document.documentElement);
	const headerFont =
		rootStyle.getPropertyValue("--font-header").trim() || "sans-serif";
	const gameYellow =
		rootStyle.getPropertyValue("--game-yellow").trim() || "#ffeb3b";
	const basePlayer1Color =
		rootStyle.getPropertyValue("--player-1").trim() || "#ff5252";
	const basePlayer2Color =
		rootStyle.getPropertyValue("--player-2").trim() || "#448aff";
	const aiEasyColor =
		rootStyle.getPropertyValue("--easy-color").trim() || "#10ec02";
	const aiMediumColor =
		rootStyle.getPropertyValue("--medium-color").trim() || "#ecb802";
	const aiHardColor =
		rootStyle.getPropertyValue("--hard-color").trim() || "#ec0202";
	const aiColors: Record<string, string> = {
		easy: aiEasyColor,
		medium: aiMediumColor,
		hard: aiHardColor,
	};
	const player1NormalColor = basePlayer2Color;
	const player2NormalColor = basePlayer1Color;
	let player1Color = player1NormalColor;
	let player2Color = player2NormalColor;

	const canvasWidth = canvas.width;
	const canvasHeight = canvas.height;
	const borderGap = 5;
	const borderThickness = 5;
	const ballRestartDelayDuration = 2;
	const ballInitialSpeed = 280;
	const ballSize = 10;
	const playBounds = {
		top: borderGap + borderThickness + ballSize / 2,
		bottom: canvasHeight - borderGap - borderThickness - ballSize / 2,
	};
	const racketWidth = 10;
	const racketHeight = 80;
	const racketSpeed = 300;
	const maxSpeed = ballInitialSpeed * 1.85;
	const minSpeed = ballInitialSpeed * 0.9;
	let startTime = 0;
	let endTime = startTime;
	const game = createGame(canvasWidth, canvasHeight);
	let aiEnabled = false;
	let aiDifficulty: AiDifficulty = "easy";

	const keys: { [key: string]: boolean } = {};

	const keydownHandler = (e: KeyboardEvent) => {
		if (e.key === "ArrowUp" || e.key === "ArrowDown" || e.key === " ") {
			e.preventDefault();
		}
		keys[e.key] = true;
	};

	const keyupHandler = (e: KeyboardEvent) => {
		keys[e.key] = false;
	};

	const aiController = createAiController({
		keydown: keydownHandler,
		keyup: keyupHandler,
	});

	const updateAi = (player: Player, nowMs: number) => {
		if (!aiEnabled) return;
		aiController.update(
			{
				ball: {
					x: game.ball.pos.x,
					y: game.ball.pos.y,
					vx: game.ball.v.x,
					vy: game.ball.v.y,
				},
				racket: {
					x: player.racket.x,
					y: player.racket.y,
					height: player.racket.height,
				},
				bounds: playBounds,
				nowMs,
			},
			aiDifficulty,
		);
	};

	const setAiMode = (enabled: boolean, difficulty?: string) => {
		const prevEnabled = aiEnabled;
		const prevDifficulty = aiDifficulty;
		aiEnabled = enabled;
		if (
			difficulty === "easy" ||
			difficulty === "medium" ||
			difficulty === "hard"
		) {
			aiDifficulty = difficulty;
		}
		player1Color = player1NormalColor;
		player2Color = aiEnabled ? aiColors[aiDifficulty] : player2NormalColor;
		if (prevEnabled !== aiEnabled || prevDifficulty !== aiDifficulty) {
			aiController.clear();
			resetMatch();
		}
	};

	const spaceHandler = (e: KeyboardEvent) => {
		if (e.key !== " ") return;
		if (game.state === "finished") {
			game.player1.score = 0;
			game.player2.score = 0;
			ballDelayTimer = 0;
			game.state = "in_progress";
			startTime = Date.now();
			endTime = startTime;
			resetBall(game);
			return;
		}
		if (game.state != "in_progress") {
			game.state = "in_progress";
			startTime = Date.now();
			resetBall(game);
		}
	};

	window.addEventListener("keydown", keydownHandler);
	window.addEventListener("keyup", keyupHandler);
	window.addEventListener("keydown", spaceHandler);

	const forfeitHandler = ((event: Event) => {
		const detail = (event as CustomEvent).detail as
			| { player?: 1 | 2 }
			| undefined;
		const forfeiting = detail?.player;

		if (game.state === "finished") return;
		if (forfeiting !== 1 && forfeiting !== 2) return;

		if (forfeiting === 1) {
			game.player1.score = 0;
			game.player2.score = game.points;
		} else {
			game.player2.score = 0;
			game.player1.score = game.points;
		}

		game.state = "finished";
		endTime = startTime ? (Date.now() - startTime) / 1000 : 0;

		if (!gameOverSent) {
			gameOverSent = true;
			window.dispatchEvent(new CustomEvent("pong:localGameOver"));
		}
	}) as EventListener;

	window.addEventListener("pong:forfeit", forfeitHandler);

	const aiModeHandler = ((event: Event) => {
		const detail = (event as CustomEvent).detail as
			| { enabled?: boolean; difficulty?: string }
			| undefined;
		setAiMode(detail?.enabled ?? true, detail?.difficulty);
	}) as EventListener;
	window.addEventListener("pong:ai", aiModeHandler);

	let lastTime = 0;
	let ballDelayTimer = 0;

	requestAnimationFrame((t) => loop(game, t));

	function createGame(canvasWidth: number, canvasHeight: number): GameEngine {
		let player1 = createPlayer("left", canvasWidth, canvasHeight);
		let player2 = createPlayer("right", canvasWidth, canvasHeight);
		let ball = createBall(canvasWidth, canvasHeight);
		const state = "pending";
		const points = 5;
		return { player1, player2, ball, state, points };
	}

	let gameOverSent = false;

	function randomVector(): Vector {
		const minAngle = 0.2;
		const maxAngle = Math.PI / 3.5;
		let angle = Math.random() * (maxAngle - minAngle) + minAngle;
		if (Math.random() < 0.5) angle = -angle;
		let speed = ballInitialSpeed;
		const direction = Math.random() < 0.5 ? -1 : 1;
		return {
			x: Math.cos(angle) * speed * direction,
			y: Math.sin(angle) * speed,
		};
	}

	function createBall(canvasWidth: number, canvasHeight: number): Ball {
		let pos = { x: canvasWidth / 2, y: canvasHeight / 2 };
		let v = randomVector();
		let size = ballSize;
		return { pos, v, size };
	}

	function resetBall(game: GameEngine) {
		game.ball.pos = { x: canvasWidth / 2, y: canvasHeight / 2 };
		game.ball.v = randomVector();
	}

	function resetMatch() {
		game.player1.score = 0;
		game.player2.score = 0;
		ballDelayTimer = 0;
		game.state = "pending";
		startTime = 0;
		endTime = 0;
		resetBall(game);
	}

	function createPlayer(
		side: Side,
		canvasWidth: number,
		canvasHeight: number,
	): Player {
		const racket = createRacket(side, canvasWidth, canvasHeight);
		let score = 0;
		return { racket, score, side };
	}

	function createRacket(
		side: Side,
		canvasWidth: number,
		canvasHeight: number,
	): Racket {
		const width = racketWidth;
		const height = racketHeight;
		const x = side === "left" ? 25 + width : canvasWidth - 25 - width;
		const y = canvasHeight / 2;
		const speedY = racketSpeed;
		return { width, height, x, y, speedY };
	}

	function speedVariation(game: GameEngine, playerNum: number): void {
		const player = playerNum == 1 ? game.player1 : game.player2;
		const racketTop = player.racket.y - 0.5 * player.racket.height;
		const racketFifth = player.racket.height / 5;
		let hitPosition = 0;
		if (game.ball.v.y < 0)
			hitPosition = game.ball.pos.y - racketTop - 0.5 * game.ball.size;
		else hitPosition = game.ball.pos.y - racketTop + 0.5 * game.ball.size;
		const currentSpeed = Math.sqrt(game.ball.v.x ** 2 + game.ball.v.y ** 2);
		let nextSpeed = currentSpeed;

		const zone = Math.floor(hitPosition / racketFifth) + 1;
		if (zone <= 1 || zone >= 5) {
			nextSpeed *= 1.15;
			const randomAngle = Math.random() * 1.05 - 0.52; // angle between -30° and +30°
			const baseAngle = playerNum === 1 ? 0 : Math.PI;
			game.ball.v.x = Math.cos(baseAngle + randomAngle) * nextSpeed;
			game.ball.v.y = Math.sin(baseAngle + randomAngle) * nextSpeed;
			return;
		} else if (zone === 2 || zone === 4) nextSpeed *= 1.1;
		if (nextSpeed > maxSpeed) nextSpeed = maxSpeed;
		else if (nextSpeed < minSpeed) nextSpeed = minSpeed;

		const ratio = nextSpeed / currentSpeed;
		game.ball.v.x *= ratio;
		game.ball.v.y *= ratio;
	}

	function checkScoreAndResetBall(): void {
		const ball = game.ball;
		const half = ball.size / 2;

		if (ball.pos.x + half < 0) {
			game.player2.score++;
			resolvePointScored();
		} else if (ball.pos.x - half > canvasWidth) {
			game.player1.score++;
			resolvePointScored();
		}
	}

	function resolvePointScored() {
		if (
			game.player1.score >= game.points ||
			game.player2.score >= game.points
		) {
			game.state = "finished";
			endTime = (Date.now() - startTime) / 1000;

			if (!gameOverSent) {
				gameOverSent = true;
				window.dispatchEvent(new CustomEvent("pong:localGameOver"));
			}
		}
		resetBall(game);
		ballDelayTimer = ballRestartDelayDuration;
	}

	function collisionRacket(playerNum: number): void {
		const ball = game.ball;
		const player = playerNum === 1 ? game.player1 : game.player2;
		const half = ball.size / 2;
		const top = player.racket.y - player.racket.height / 2;
		const bottom = player.racket.y + player.racket.height / 2;

		const leftX =
			playerNum === 1
				? player.racket.x - player.racket.width
				: player.racket.x;
		const rightX =
			playerNum === 1
				? player.racket.x
				: player.racket.x + player.racket.width;

		const ballLeft = ball.pos.x - half;
		const ballRight = ball.pos.x + half;
		const ballTop = ball.pos.y - half;
		const ballBottom = ball.pos.y + half;

		const overlapsY = ballBottom >= top && ballTop <= bottom;
		const overlapsX = ballRight >= leftX && ballLeft <= rightX;
		const isApproaching = playerNum === 1 ? ball.v.x < 0 : ball.v.x > 0;

		if (!overlapsY || !overlapsX || !isApproaching) return;

		if (playerNum === 1) {
			ball.pos.x = rightX + half;
			ball.v.x = Math.abs(ball.v.x);
		} else {
			ball.pos.x = leftX - half;
			ball.v.x = -Math.abs(ball.v.x);
		}
		speedVariation(game, playerNum);
	}

	function collisionWall(ball: Ball): void {
		const top = borderGap + borderThickness + ball.size / 2;
		const bottom =
			canvasHeight - borderGap - borderThickness - ball.size / 2;
		if (ball.pos.y <= top) {
			ball.pos.y = top + 0.01;
			ball.v.y = Math.abs(ball.v.y);
		} else if (ball.pos.y >= bottom) {
			ball.pos.y = bottom - 0.01;
			ball.v.y = -Math.abs(ball.v.y);
		}
	}

	function updateBall(game: GameEngine, deltaTime: number): void {
		if (ballDelayTimer > 0) {
			ballDelayTimer -= deltaTime;
			return;
		}
		const ball = game.ball;
		ball.pos.x += ball.v.x * deltaTime;
		ball.pos.y += ball.v.y * deltaTime;
		collisionWall(ball);
		collisionRacket(1);
		collisionRacket(2);
		checkScoreAndResetBall();
	}

	function updateRacket(
		game: GameEngine,
		deltaTime: number,
		playerNum: number,
		nowMs: number,
	) {
		const player = playerNum === 1 ? game.player1 : game.player2;
		if (playerNum === 2) updateAi(player, nowMs);
		if (
			((playerNum === 1 && keys["w"]) ||
				(playerNum === 2 && keys["ArrowUp"])) &&
			player.racket.y -
				player.racket.height / 2 -
				borderThickness -
				borderGap -
				5 >
				0
		)
			player.racket.y -= player.racket.speedY * deltaTime;
		else if (
			((playerNum === 1 && keys["s"]) ||
				(playerNum === 2 && keys["ArrowDown"])) &&
			player.racket.y + player.racket.height / 2 <
				canvasHeight - borderThickness - borderGap - 5
		)
			player.racket.y += player.racket.speedY * deltaTime;
		const minY = borderGap + borderThickness + player.racket.height / 2 + 5;
		const maxY =
			canvasHeight -
			borderGap -
			borderThickness -
			player.racket.height / 2 -
			5;
		player.racket.y = Math.max(minY, Math.min(maxY, player.racket.y));
	}

	function drawRackets(): void {
		ctx!.fillStyle = player1Color;
		ctx?.fillRect(
			game.player1.racket.x - game.player1.racket.width,
			game.player1.racket.y - 0.5 * game.player1.racket.height,
			game.player1.racket.width,
			game.player1.racket.height,
		);
		ctx!.fillStyle = player2Color;
		ctx?.fillRect(
			game.player2.racket.x,
			game.player2.racket.y - 0.5 * game.player2.racket.height,
			game.player2.racket.width,
			game.player2.racket.height,
		);
	}

	function drawBorders(): void {
		ctx!.fillStyle = "white";
		ctx?.fillRect(10, borderGap, canvasWidth - 20, borderThickness);
		ctx?.fillRect(
			10,
			canvasHeight - borderGap - borderThickness,
			canvasWidth - 20,
			borderThickness,
		);
	}

	function drawBall(): void {
		ctx!.fillStyle = gameYellow;
		ctx?.beginPath();
		ctx?.arc(
			game.ball.pos.x,
			game.ball.pos.y,
			game.ball.size / 2,
			0,
			Math.PI * 2,
		);
		ctx!.fill();
	}

	function drawScore(): void {
		ctx!.font = `48px ${headerFont}`;
		ctx!.fillStyle = gameYellow;
		const score: string =
			game.player1.score.toString() +
			" - " +
			game.player2.score.toString();
		const textWidth = ctx?.measureText(score).width;
		const x = (canvasWidth - textWidth!) / 2;
		ctx?.fillText(score, x, 100);
	}

	function drawCountdown() {
		ctx!.font = `72px ${headerFont}`;
		const countdown = Math.ceil(ballDelayTimer).toString();
		const countdownWidth = ctx?.measureText(countdown).width;
		const countdownX = (canvasWidth - countdownWidth!) / 2;
		ctx!.fillStyle = gameYellow;
		ctx?.fillText(countdown, countdownX, canvasHeight / 2 - 50);
	}

	function draw(game: GameEngine) {
		ctx?.clearRect(0, 0, canvasWidth, canvasHeight);
		ctx!.fillStyle = "black";
		ctx?.fillRect(0, 0, canvasWidth, canvasHeight);

		if (game.state === "pending") gameReadyToStart();
		drawBorders();
		drawRackets();
		drawBall();
		drawScore();
		if (ballDelayTimer > 0 && game.state != "finished") drawCountdown();
	}

	function drawFinalScore() {
		const winner: string =
			game.player1.score > game.player2.score
				? "PLAYER 1 WINS"
				: aiEnabled
					? "AI WINS"
					: "PLAYER 2 WINS";
		const winnerFontSize = 64;
		const restartFontSize = 24;
		const winnerY = canvasHeight / 2 + 80;
		const restartY = winnerY + 55;
		ctx!.font = `${winnerFontSize}px ${headerFont}`;
		ctx!.fillStyle = gameYellow;
		const textWidth = ctx?.measureText(winner).width;
		const x = (canvasWidth - textWidth!) / 2;
		ctx?.fillText(winner, x, winnerY);

		const restartMsg = "PRESS SPACEBAR TO RESTART";
		ctx!.font = `${restartFontSize}px ${headerFont}`;
		const restartWidth = ctx?.measureText(restartMsg).width;
		const restartX = (canvasWidth - restartWidth!) / 2;
		ctx?.fillText(restartMsg, restartX, restartY);
	}

	function gameReadyToStart() {
		const startMsg: string = "PRESS SPACE TO START";
		const textWidth = 400;
		const x = (canvasWidth - textWidth!) / 2;
		ctx!.font = `40px ${headerFont}`;
		ctx!.fillStyle = gameYellow;
		ctx?.fillText(startMsg, x, canvasHeight - 100, textWidth);
	}

	function loop(game: GameEngine, timestamp: DOMHighResTimeStamp) {
		let deltaTime = (timestamp - lastTime) / 1000;
		lastTime = timestamp;
		deltaTime = Math.min(deltaTime, 1 / 30);

		if (game.state == "finished") {
			draw(game);
			drawFinalScore();
			requestAnimationFrame((t) => loop(game, t));
			return;
		}
		if (game.state == "in_progress") {
			updateBall(game, deltaTime);
		}
		updateRacket(game, deltaTime, 1, timestamp);
		updateRacket(game, deltaTime, 2, timestamp);
		draw(game);
		requestAnimationFrame((t) => loop(game, t));
	}

	return () => {
		window.removeEventListener("keydown", keydownHandler);
		window.removeEventListener("keyup", keyupHandler);
		window.removeEventListener("keydown", spaceHandler);
		window.removeEventListener("pong:ai", aiModeHandler);
		window.removeEventListener("pong:forfeit", forfeitHandler);
	};
}
