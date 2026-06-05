export type Vector = {
  x: number;
  y: number;
};

export type Ball = {
  pos: Vector;
  v: Vector;
  size: number;
};

export type Racket = {
  y: number;
  x: number;
  width: number;
  height: number;
};

export type Side = "left" | "right";

export type PlayerInput = "up" | "down" | "idle";

export type Player = {
  racket: Racket;
  side: Side;
  score: number;
  input: PlayerInput;
};

export type GameState = "pending" | "countdown" | "in_progress" | "finished";

export type GameSnapshot = {
  player1: Player;
  player2: Player;
  ball: Ball;
  state: GameState;
  countdown: number;
  winner: Side | null;
};

export const CONFIG = {
  canvasWidth: 800,
  canvasHeight: 500,
  borderGap: 5,
  borderThickness: 15,
  ballInitialSpeed: 250,
  ballSize: 10,
  racketWidth: 10,
  racketHeight: 80,
  racketSpeed: 300,
  maxSpeed: 250 * 1.85,
  minSpeed: 250 * 0.9,
  pointsToWin: 5,
  countdownDuration: 3,
  ballRestartDelay: 2,
} as const;

export class PongEngine {
  ball: Ball;
  player1: Player;
  player2: Player;
  state: GameState;
  countdown: number;
  winner: Side | null;

  constructor() {
    this.ball = this.createBall();
    this.player1 = this.createPlayer("left");
    this.player2 = this.createPlayer("right");
    this.state = "pending";
    this.countdown = 0;
    this.winner = null;
  }

  private createBall(): Ball {
    return {
      pos: { x: CONFIG.canvasWidth / 2, y: CONFIG.canvasHeight / 2 },
      v: { x: 0, y: 0 },
      size: CONFIG.ballSize,
    };
  }

  private createPlayer(side: Side): Player {
    const x =
      side === "left"
        ? 25 + CONFIG.racketWidth
        : CONFIG.canvasWidth - 25 - CONFIG.racketWidth;

    return {
      racket: {
        x,
        y: CONFIG.canvasHeight / 2,
        width: CONFIG.racketWidth,
        height: CONFIG.racketHeight,
      },
      side,
      score: 0,
      input: "idle",
    };
  }

  private randomVelocity(): Vector {
    const minAngle = 0.2;
    const maxAngle = Math.PI / 4;
    let angle = Math.random() * (maxAngle - minAngle) + minAngle;
    if (Math.random() < 0.5) angle = -angle;
    const direction = Math.random() < 0.5 ? -1 : 1;
    return {
      x: Math.cos(angle) * CONFIG.ballInitialSpeed * direction,
      y: Math.sin(angle) * CONFIG.ballInitialSpeed,
    };
  }

  startGame(): void {
    if (this.state !== "pending") return;
    this.state = "countdown";
    this.countdown = CONFIG.countdownDuration;
  }

  setPlayerInput(playerNum: 1 | 2, input: PlayerInput): void {
    const player = playerNum === 1 ? this.player1 : this.player2;
    player.input = input;
  }

  update(deltaTime: number): void {
    if (this.state === "pending") return;
    if (this.state === "countdown") {
      this.countdown -= deltaTime;
      if (this.countdown <= 0) {
        this.state = "in_progress";
        this.ball.v = this.randomVelocity();
      }
      return;
    }
    if (this.state === "finished") return;

    this.updateRackets(deltaTime);
    this.updateBall(deltaTime);
  }

  private updateRackets(deltaTime: number): void {
    this.updateRacket(this.player1, deltaTime);
    this.updateRacket(this.player2, deltaTime);
  }

  private updateRacket(player: Player, deltaTime: number): void {
    const minY =
      CONFIG.borderGap + CONFIG.borderThickness + player.racket.height / 2;
    const maxY =
      CONFIG.canvasHeight -
      (CONFIG.borderGap + CONFIG.borderThickness) -
      player.racket.height / 2;

    if (player.input === "up") {
      player.racket.y = Math.max(
        minY,
        player.racket.y - CONFIG.racketSpeed * deltaTime,
      );
    } else if (player.input === "down") {
      player.racket.y = Math.min(
        maxY,
        player.racket.y + CONFIG.racketSpeed * deltaTime,
      );
    }
  }

  private updateBall(deltaTime: number): void {
    if (this.countdown > 0) {
      this.countdown -= deltaTime;
      if (this.countdown <= 0) this.ball.v = this.randomVelocity();
      return;
    }
    this.ball.pos.x += this.ball.v.x * deltaTime;
    this.ball.pos.y += this.ball.v.y * deltaTime;

    this.collisionWalls();
    this.collisionRacket(this.player1);
    this.collisionRacket(this.player2);
    this.checkScore();
  }

  private applySpeedVariation(player: Player): void {
    const ball = this.ball;
    const racket = player.racket;

    const racketTop = racket.y - 0.5 * racket.height;
    const racketFifth = racket.height / 5;
    const hitPosition = ball.pos.y - racketTop;

    const currentSpeed = Math.sqrt(ball.v.x ** 2 + ball.v.y ** 2);
    let nextSpeed = currentSpeed;

    const zone = Math.floor(hitPosition / racketFifth) + 1;
    if (zone <= 1 || zone >= 5) {
      nextSpeed *= 1.15;
	  if (nextSpeed > CONFIG.maxSpeed) nextSpeed = CONFIG.maxSpeed;
      const randomAngle = Math.random() * 1.05 - 0.52; // angle between -30° and +30°
      const baseAngle = player.side === "left" ? 0 : Math.PI;
      ball.v.x = Math.cos(baseAngle + randomAngle) * nextSpeed;
      ball.v.y = Math.sin(baseAngle + randomAngle) * nextSpeed;
      return;
    } else if (zone === 2 || zone === 4) nextSpeed *= 1.1;
    if (nextSpeed > CONFIG.maxSpeed) nextSpeed = CONFIG.maxSpeed;
    else if (nextSpeed < CONFIG.minSpeed) nextSpeed = CONFIG.minSpeed;

    const ratio = nextSpeed / currentSpeed;
    ball.v.x *= ratio;
    ball.v.y *= ratio;
  }

  private collisionRacket(player: Player): void {
    const ball = this.ball;
    const racket = player.racket;

    const racketTop = player.racket.y - 0.5 * player.racket.height;
    const racketBottom = player.racket.y + 0.5 * player.racket.height;

    let racketFace: number;
    let ballEdge: number;
    let isApproaching: boolean;

    if (player.side === "left") {
      racketFace = player.racket.x;
      ballEdge = ball.pos.x - ball.size / 2;
      isApproaching = ball.v.x < 0;
    } else {
      racketFace = player.racket.x;
      ballEdge = ball.pos.x + ball.size / 2;
      isApproaching = ball.v.x > 0;
    }

    const ballInRacketY =
      ball.pos.y + ball.size / 2 >= racketTop &&
      ball.pos.y - ball.size / 2 <= racketBottom;

    if (
      player.side === "left" &&
      ballEdge <= racketFace &&
      ballInRacketY &&
      isApproaching
    ) {
      ball.pos.x = racketFace + ball.size / 2;
      ball.v.x = Math.abs(ball.v.x);
      this.applySpeedVariation(player);
    } else if (
      player.side === "right" &&
      ballEdge >= racketFace &&
      ballInRacketY &&
      isApproaching
    ) {
      ball.pos.x = racketFace - ball.size / 2;
      ball.v.x = -Math.abs(ball.v.x);
      this.applySpeedVariation(player);
    }
  }

  private collisionWalls(): void {
    const topLimit =
      CONFIG.borderGap + CONFIG.borderThickness + this.ball.size / 2;
    const bottomLimit =
      CONFIG.canvasHeight -
      CONFIG.borderGap -
      CONFIG.borderThickness -
      this.ball.size / 2;

    if (this.ball.pos.y >= bottomLimit) {
      this.ball.pos.y = bottomLimit;
      this.ball.v.y = -Math.abs(this.ball.v.y);
    } else if (this.ball.pos.y <= topLimit) {
      this.ball.pos.y = topLimit;
      this.ball.v.y = Math.abs(this.ball.v.y);
    }
  }

  private checkScore(): void {
    if (this.ball.pos.x <= 0) {
      this.player2.score++;
      this.onScore(this.player2);
    } else if (this.ball.pos.x >= CONFIG.canvasWidth) {
      this.player1.score++;
      this.onScore(this.player1);
    }
  }

  private onScore(scorer: Player): void {
    if (scorer.score >= CONFIG.pointsToWin) {
      this.state = "finished";
      this.winner = scorer.side;
      this.ball.v = { x: 0, y: 0 };
    } else {
      this.ball.pos = {
        x: CONFIG.canvasWidth / 2,
        y: CONFIG.canvasHeight / 2,
      };
      this.ball.v = { x: 0, y: 0 };
      this.countdown = CONFIG.ballRestartDelay;
    }
  }

  getSnapshot(): GameSnapshot {
    return {
      ball: {
        ...this.ball,
        pos: { ...this.ball.pos },
        v: { ...this.ball.v },
      },
      player1: { ...this.player1, racket: { ...this.player1.racket } },
      player2: { ...this.player2, racket: { ...this.player2.racket } },
      state: this.state,
      countdown: this.countdown,
      winner: this.winner,
    };
  }
}
