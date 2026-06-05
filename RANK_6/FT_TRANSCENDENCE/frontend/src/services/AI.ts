// Difficulty levels used by the AI tuner and UI.
export type AiDifficulty = "easy" | "medium" | "hard";
// Directional output: up, idle, down.
export type AiMove = -1 | 0 | 1;

// Snapshot of the game state the AI needs to decide movement.
type AiInput = {
  ball: { x: number; y: number; vx: number; vy: number };
  racket: { x: number; y: number; height: number };
  bounds: { top: number; bottom: number };
  nowMs: number;
};

// Per-difficulty tuning values (single knob: error range).
type AiLevel = { errorRange: number };

// AI sampling cadence (milliseconds).
const AI_SAMPLE_MS = 1000;

// Difficulty presets: higher level = smaller error range.
const LEVELS: Record<AiDifficulty, AiLevel> = {
  easy: { errorRange: 140 },
  medium: { errorRange: 70 },
  hard: { errorRange: 6 },
};

// Math helpers.
const clamp = (value: number, min: number, max: number) =>
  Math.min(Math.max(value, min), max);

const randomBetween = (min: number, max: number) =>
  Math.random() * (max - min) + min;

// Reflect a Y position within bounds (simulates wall bounces).
const reflectY = (y: number, bounds: { top: number; bottom: number }) => {
  const range = bounds.bottom - bounds.top;
  if (range <= 0) return bounds.top;
  let pos = y - bounds.top;
  const period = range * 2;
  pos = ((pos % period) + period) % period;
  if (pos > range) pos = period - pos;
  return bounds.top + pos;
};

// Target prediction: estimate where the ball will be at the racket's x.
const pickTargetY = (input: AiInput, difficulty: AiDifficulty) => {
  const { ball, racket, bounds } = input;
  const centerY = (bounds.top + bounds.bottom) / 2;
  let target = centerY;

  if (ball.vx > 0) {
    const timeToRacket = (input.racket.x - ball.x) / ball.vx;
    if (timeToRacket > 0) {
      const projected = ball.y + ball.vy * timeToRacket;
      target = reflectY(projected, bounds);
    }
  }

  // Inject simple error based on difficulty.
  const error = LEVELS[difficulty].errorRange;
  target += randomBetween(-error, error);

  const margin = racket.height / 2;
  return clamp(target, bounds.top + margin, bounds.bottom - margin);
};

// Core AI logic: sample target occasionally and output a move.
export const createAIOpponent = (sampleIntervalMs = AI_SAMPLE_MS) => {
  let lastSample = -Infinity;
  let targetY = 0;

  return {
    update: (input: AiInput, difficulty: AiDifficulty): AiMove => {
      if (input.nowMs - lastSample >= sampleIntervalMs) {
        lastSample = input.nowMs;
        targetY = pickTargetY(input, difficulty);
      }

      const deadZone = input.racket.height * 0.15;
      if (input.racket.y < targetY - deadZone) return 1;
      if (input.racket.y > targetY + deadZone) return -1;
      return 0;
    },
  };
};

type AiKeyHandlers = {
  keydown: (event: KeyboardEvent) => void;
  keyup: (event: KeyboardEvent) => void;
};

// Keyboard adapter: converts AI moves into the same input path as players.
export const createAiKeyController = (handlers: AiKeyHandlers) => {
  const makeEvent = (key: "ArrowUp" | "ArrowDown") =>
    ({ key, preventDefault: () => {} }) as KeyboardEvent;

  const pressUp = () => {
    handlers.keydown(makeEvent("ArrowUp"));
    handlers.keyup(makeEvent("ArrowDown"));
  };

  const pressDown = () => {
    handlers.keydown(makeEvent("ArrowDown"));
    handlers.keyup(makeEvent("ArrowUp"));
  };

  const releaseAll = () => {
    handlers.keyup(makeEvent("ArrowUp"));
    handlers.keyup(makeEvent("ArrowDown"));
  };

  const applyMove = (move: AiMove) => {
    if (move < 0) {
      pressUp();
      return;
    }
    if (move > 0) {
      pressDown();
      return;
    }
    releaseAll();
  };

  return { applyMove, clear: releaseAll };
};

// High-level controller that ties AI decisions to key simulation.
export const createAiController = (
  handlers: AiKeyHandlers,
  sampleIntervalMs = AI_SAMPLE_MS
) => {
  const opponent = createAIOpponent(sampleIntervalMs);
  const keyController = createAiKeyController(handlers);

  return {
    update: (input: AiInput, difficulty: AiDifficulty) => {
      keyController.applyMove(opponent.update(input, difficulty));
    },
    clear: () => keyController.clear(),
  };
};
