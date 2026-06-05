import Database, { initDatabase } from "./db.ts";
import { Queries } from "./queries.ts";

let db: Database | null = null;
let queries: Queries | null = null;

export async function initialize(): Promise<void> {
	db = await initDatabase("./data/transcendence.db");
	queries = new Queries(db);
}

export function getQueries(): Queries {
	if (!queries) {
		throw new Error("Database not initialized");
	}
	return queries;
}

export function getDb(): Database {
	if (!db)
			throw new Error("No database");
	return db;
}

export * from "./types";
