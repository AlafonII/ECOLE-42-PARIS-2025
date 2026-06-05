import BetterSqlite3 from "better-sqlite3";
import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";
import { dirname } from "path";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

class Database {
	private db: BetterSqlite3.Database;

	constructor(dbPath: string) {
		try {
			this.db = new BetterSqlite3(dbPath);
			this.db.pragma("foreign_keys = ON");
			//console.log("Connected to SQLite database");
		} catch (err) {
			console.error("Error opening database: ", err);
			throw err;
		}
	}

	async run(
		sql: string,
		params: any[] = []
	): Promise<BetterSqlite3.RunResult> {
		const stmt = this.db.prepare(sql);
		return stmt.run(params);
	}

	async get<T>(sql: string, params: any[] = []): Promise<T | undefined> {
		const stmt = this.db.prepare(sql);
		return stmt.get(params) as T | undefined;
	}

	async all<T>(sql: string, params: any[] = []): Promise<T[]> {
		const stmt = this.db.prepare(sql);
		return stmt.all(params) as T[];
	}

	async exec(sql: string): Promise<void> {
		this.db.exec(sql);
	}

	close(): Promise<void> {
		this.db.close();
		return Promise.resolve();
	}
}

export async function initDatabase(
	dbPath: string = "./data/transcendence.db"
): Promise<Database> {
	const dataDir = path.dirname(dbPath);
	if (!fs.existsSync(dataDir)) {
		fs.mkdirSync(dataDir, { recursive: true });
	}

	const db = new Database(dbPath);

	const schemaPath = path.join(__dirname, "schema.sql");
	if (fs.existsSync(schemaPath)) {
		const schema = fs.readFileSync(schemaPath, "utf-8");
		await db.exec(schema);
	} else {
		console.warn(`Warning: schema.sql not found at ${schemaPath}`);
	}

	const seedPath = path.join(__dirname, "seed.sql");
	if (fs.existsSync(seedPath)) {
		const seed = fs.readFileSync(seedPath, "utf-8");
		await db.exec(seed);
	}

	return db;
}

export default Database;
