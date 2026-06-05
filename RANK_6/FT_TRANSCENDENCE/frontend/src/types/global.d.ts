import type { PublicUser } from "../services/types";

export {};

declare global {
  interface Window {
    __ME__?: PublicUser | null;
  }
}
