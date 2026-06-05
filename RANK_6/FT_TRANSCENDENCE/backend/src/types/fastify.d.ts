import type Database from "../database/db";
import "@fastify/jwt";

declare module "@fastify/jwt" {
	interface FastifyJWT {
    payload: {
      id: number;
      username: string;
      email: string;
    };
    user: {
      id: number;
      username: string;
      email: string;
    };
  }
}

declare module 'fastify' {
	interface FastifyInstance {
		db: Database;
	}
}
