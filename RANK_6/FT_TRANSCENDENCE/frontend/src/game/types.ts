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


export type Player = {
  racket: Racket;
  side: Side;
  score: number;
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
  borderThickness: 5,
  ballSize: 10,
} as const;