import { match } from "assert";
import type Database from "./db.ts";
import {
	AdversaryRow,
	BasicStatsRow,
	CreateUserDTO,
	DBUser,
	Match,
	MatchRow,
	MatchWithPlayers,
	NewUsersOTP,
	PublicUser,
	Tournament,
	TournamentParticipant,
	TournamentRanking,
	UserDashboard,
	UsersOTP,
	vsStats,
} from "./types.ts";

//WHENEVER WE ADD A NEW CALL TO THE DATABASE, WE NEED TO ADD A CORRESPONDING QUERY HERE

export class Queries {
	constructor(private db: Database) {}

	//########################[USER ROUTES]########################

	//This adds a new user to the database
	async createUser(data: CreateUserDTO): Promise<number> {
		await this.db.run(
			"INSERT INTO users (username, email, password_hash, display_name, avatar_url, two_factor) VALUES (?, ?, ?, ?, ?, ?)",
			[
				data.username,
				data.email,
				data.password_hash,
				data.username, // Keep display_name in DB aligned with username
				data.avatar_url,
				data.two_factor,
			],
		);

		const user = await this.db.get<{ id: number }>(
			"SELECT id FROM users WHERE username = ?",
			[data.username],
		);

		return user?.id || 0;
	}

	// This updates an existing user of the database
	async updateUser(
		id: number,
		data: Partial<{
			username: string;
			email: string;
			password_hash: string;
			avatar_url: string | null;
		}>,
	): Promise<boolean> {
		const fields: string[] = [];
		const values: any[] = [];

		if (data.username) {
			fields.push("username = ?");
			values.push(data.username);
		}

		if (data.email) {
			fields.push("email = ?");
			values.push(data.email);
		}

		if (data.password_hash) {
			fields.push("password_hash = ?");
			values.push(data.password_hash);
		}

		if (data.avatar_url !== undefined) {
			fields.push("avatar_url = ?");
			values.push(data.avatar_url);
		}

		if (fields.length === 0) return false;

		values.push(id);

		const query = `
			UPDATE users
			SET ${fields.join(", ")}, updated_at = CURRENT_TIMESTAMP
			WHERE id = ?
		`;

		await this.db.run(query, values);
		return true;
	}

	// This deletes the given user
	async deleteUser(id: number): Promise<boolean> {
		await this.db.run("DELETE FROM users WHERE id = ?", [id]);
		return true;
	}

	//This returns all the information about a user except the password info
	async getUserById(id: number): Promise<PublicUser | undefined> {
		return this.db.get<PublicUser>(
			"SELECT id, username, email, avatar_url, created_at, updated_at FROM users WHERE id = ?",
			[id],
		);
	}

	async getUserByUsername(username: string): Promise<PublicUser | undefined> {
		return this.db.get<PublicUser>(
			`SELECT id, username, email, avatar_url, created_at, updated_at FROM users WHERE username = ? COLLATE NOCASE`,
			[username],
		);
	}

	async isUsernameFree(username: string): Promise<boolean> {
		const result = await this.db.get<{ count: number }>(
			`SELECT COUNT(*) as count FROM users WHERE username = ?`,
			[username],
		);
		return (result?.count ?? 0) == 0;
	}

	async getUserByEmail(email: string): Promise<PublicUser | undefined> {
		return this.db.get<PublicUser>(
			`SELECT id, username, email, avatar_url, created_at, updated_at FROM users WHERE email = ?`,
			[email],
		);
	}

	async getUserWithPasswordByEmail(
		email: string,
	): Promise<DBUser | undefined> {
		return this.db.get<DBUser>(
			`SELECT id, username, email, password_hash, display_name, avatar_url, two_factor, created_at, updated_at FROM users WHERE email = ?`,
			[email],
		);
	}

	//This returns information about all users from the db without password info
	async getAllUsers(): Promise<PublicUser[]> {
		return this.db.all<PublicUser>(
			"SELECT id, username, email, avatar_url, created_at, updated_at FROM users",
		);
	}

	//This returns a user with information about his password - TO BE USED ONLY FOR AUTHENTICATION
	async getUserForAuth(email: string): Promise<DBUser | undefined> {
		return this.db.get<DBUser>(
			"SELECT id, username, email, password_hash, display_name, avatar_url, created_at, updated_at FROM users WHERE email = ?",
			[email],
		);
	}

	//########################[MATCHES ROUTES]########################

	//If a match is not part of a tournament, we pass all tournamentId, roundNumber and matchNumber as null
	async createMatch(
		gameTypeId: number,
		tournamentId?: number,
		roundNumber?: number,
		matchNumber?: number,
	): Promise<number> {
		await this.db.run(
			`INSERT INTO matches (game_type_id, tournament_id, round_number, match_number) VALUES (?, ?, ?, ?)`,
			[gameTypeId, tournamentId, roundNumber, matchNumber],
		);

		const row = await this.db.get<{ id: number }>(
			"SELECT last_insert_rowid() as id",
		);
		return row?.id ?? 0;
	}

	//To add an AI opponent, we pass userId as null
	async addMatchParticipant(
		matchId: number,
		userId: number | null = null,
	): Promise<void> {
		const isAI = userId === null ? 1 : 0;
		await this.db.run(
			`INSERT INTO match_participants (match_id, user_id, is_ai) VALUES (?, ?, ?)`,
			[matchId, userId ?? null, isAI],
		);
	}

	//This updates the number of points scored by a user in a match - should be called separately for the scores of both users
	async updateMatchScore(
		matchId: number,
		userId: number | null = null,
		score: number,
	): Promise<void> {
		await this.db.run(
			`UPDATE match_participants SET score = ? WHERE match_id = ? AND user_id = ?`,
			[score, matchId, userId],
		);
	}

	//This sets who won each match. If the AI won, we keep winner as NULL but update the duration of the game
	//This means that as long as the game has no duration, it has not been finished
	async setMatchWinner(
		matchId: number,
		winnerId: number | null,
		durationSeconds: number,
	): Promise<void> {
		await this.db.run(
			`UPDATE matches set winner_id = ?, duration_seconds = ? WHERE id = ?`,
			[winnerId, durationSeconds, matchId],
		);

		const matchDetails = this.getMatchWithUsers(matchId);
		const tournamentId = (await matchDetails).tournament_id;
		if (typeof tournamentId === "number") {
			try {
				await this.finishTournament(tournamentId);
			} catch (err: any) {
				const msg = err?.message ?? String(err);
				if (
					msg.includes("matches still unplayed") ||
					msg.includes("has already finished")
				) {
					return;
				}
				throw err;
			}
		}
	}

	//This gets info about a match data and participants
	async getMatchWithUsers(matchId: number): Promise<MatchWithPlayers> {
		const match = await this.db.get<Match>(
			`SELECT * FROM matches WHERE id = ?`,
			[matchId],
		);

		if (!match) {
			throw new Error(`Match ${matchId} not found`);
		}

		const players = await this.db.all<{
			user_id: number | null;
			username: string | null;
			score: number;
		}>(
			`SELECT
				mp.user_id,
				u.username,
				mp.score
			FROM match_participants mp
			LEFT JOIN users u ON mp.user_id = u.id
			WHERE mp.match_id = ? 
			ORDER BY mp.score DESC`,
			[matchId],
		);

		return {
			...match,
			players,
		};
	}

	//This helper function allows us to filter matches for other subqueries
	private async getMatchesWithUsersHelper(
		whereClause: string = "",
	): Promise<MatchWithPlayers[]> {
		const query = `SELECT * FROM matches ${whereClause}`;

		const matches = await this.db.all<Match>(query);

		const matchesWithPlayers = await Promise.all(
			matches.map(async (match) => {
				const players = await this.db.all<{
					user_id: number | null;
					username: string | null;
					score: number;
				}>(
					`SELECT mp.user_id, u.username, mp.score
					FROM match_participants mp
					LEFT JOIN users u ON mp.user_id = u.id
					WHERE mp.match_id = ?`,
					[match.id],
				);
				return { ...match, players };
			}),
		);
		return matchesWithPlayers;
	}

	//This gets all matches with scores and participants
	async getAllMatchesWithUsers(): Promise<MatchWithPlayers[]> {
		return this.getMatchesWithUsersHelper();
	}

	//This gets all matches that have not yet been playerd
	async getAllUnplayedMatchesWithUsers(): Promise<MatchWithPlayers[]> {
		return this.getMatchesWithUsersHelper("WHERE duration_seconds IS NULL");
	}

	//This gets all matches that have not yet been playerd
	async getAllFinishedMatchesWithUsers(): Promise<MatchWithPlayers[]> {
		return this.getMatchesWithUsersHelper(
			"WHERE duration_seconds IS NOT NULL",
		);
	}

	//This returns all matches of a user
	async getAllMatchesOfUser(userId: number): Promise<MatchWithPlayers[]> {
		const matches = await this.db.all<Match>(
			`SELECT DISTINCT m.*
			FROM matches m
			JOIN match_participants mp ON m.id = mp.match_id
			WHERE mp.user_id = ?`,
			[userId],
		);

		return Promise.all(
			matches.map(async (match) => {
				const players = await this.db.all<{
					user_id: number | null;
					username: string | null;
					score: number;
				}>(
					`SELECT mp.user_id, u.username, mp.score
					FROM match_participants mp
					LEFT JOIN users u on mp.user_id = u.id
					WHERE mp.match_id = ?
					ORDER BY mp.score DESC`,
					[match.id],
				);
				return { ...match, players };
			}),
		);
	}

	//########################[TOURNAMENT ROUTES]########################

	//This creates a blank tournament without any participants
	async createTournament(
		name: string,
		gameTypeId: number,
		participantsNb: number,
		organiserId: number,
	): Promise<number> {
		await this.db.run(
			`
			INSERT INTO tournaments (name, game_type_id, max_participants, tournament_organizer_id) VALUES (?, ?, ?, ?)
			`,
			[name, gameTypeId, participantsNb, organiserId],
		);

		const row = await this.db.get<{ id: number }>(
			"SELECT id FROM tournaments WHERE name = ? ORDER BY id DESC LIMIT 1",
			[name],
		);

		return row?.id ?? 0;
	}

	//This start the tournament
	async startTournament(tournamentId: number): Promise<number> {
		let tournament = await this.getTournamentById(tournamentId);

		if (!tournament) {
			throw new Error(`Tournament ${tournamentId} not found`);
		}

		if (tournament?.started_at) {
			throw new Error(`Tournament ${tournamentId} has already started`);
		}

		await this.db.run(
			`UPDATE tournaments SET status = 'in_progress', started_at = CURRENT_TIMESTAMP WHERE id = ?`,
			[tournamentId],
		);

		tournament = await this.getTournamentById(tournamentId);

		if (tournament?.started_at) return tournament.id;

		return 0;
	}

	//This gets all the information about a single tournament
	async getTournamentById(id: number): Promise<Tournament | undefined> {
		return this.db.get<Tournament>(
			"SELECT * FROM tournaments WHERE id = ?",
			[id],
		);
	}

	//This returns all the tournaments from the database
	async getAllTournaments(): Promise<Tournament[]> {
		return this.db.all<Tournament>("SELECT * FROM tournaments");
	}

	//This helper will allow us to determine head to head results
	private async applyHeadToHeadResolution(
		tournamentId: number,
		rows: TournamentRanking[],
	): Promise<void> {
		const isEqual = (a: TournamentRanking, b: TournamentRanking) =>
			a.wins === b.wins &&
			a.points_diff === b.points_diff &&
			a.points_for === b.points_for;

		let i = 0;
		while (i < rows.length) {
			let j = i + 1;
			while (j < rows.length && isEqual(rows[i], rows[j])) j++;
			if (j - i > 1) {
				const group = rows.slice(i, j);
				const ids = group.map((g) => g.user_id);
				const placeholders = ids.map(() => "?").join(",");
				const h2h = await this.db.all<{
					user_id: number;
					opp_id: number;
					winner_id: number | null;
					score: number;
					opp_score: number;
					duration_seconds: number | null;
				}>(
					`
					SELECT
						mp.user_id AS user_id,
						opp.user_id AS opp_id,
						m.winner_id AS winner_id,
						mp.score AS score,
						opp.score AS opp_score,
						m.duration_seconds AS duration_seconds
					FROM matches m
					JOIN match_participants mp ON mp.match_id = m.id
					JOIN match_participants opp
						ON opp.match_id = m.id
						AND opp.user_id <> mp.user_id
					WHERE m.tournament_id = ?
						AND mp.user_id IN (${placeholders})
						AND opp.user_id IN (${placeholders})
						AND m.duration_seconds IS NOT NULL
					`,
					[tournamentId, ...ids, ...ids],
				);
				const headWins = new Map<number, number>();
				const headDiff = new Map<number, number>();

				for (const id of ids) {
					headWins.set(id, 0);
					headDiff.set(id, 0);
				}

				for (const row of h2h) {
					if (row.winner_id === row.user_id) {
						headWins.set(
							row.user_id,
							(headWins.get(row.user_id) ?? 0) + 1,
						);
					}
					headDiff.set(
						row.user_id,
						(headDiff.get(row.user_id) ?? 0) +
							(row.score - row.opp_score),
					);
				}
				group.sort((a, b) => {
					const aw = headWins.get(a.user_id) ?? 0;
					const bw = headWins.get(b.user_id) ?? 0;
					if (bw != aw) return bw - aw;

					const ad = headDiff.get(a.user_id) ?? 0;
					const bd = headDiff.get(b.user_id) ?? 0;
					if (bd != ad) return bd - ad;
					if (b.total_duration !== a.total_duration)
						return b.total_duration - a.total_duration;
					return a.user_id - b.user_id;
				});
				for (let k = 0; k < group.length; k++) rows[i + k] = group[k];
			}
			i = j;
		}
	}

	//This gets the current tournament table
	async getTournamentRanking(
		tournamentId: number,
	): Promise<TournamentRanking[]> {
		const base = await this.db.all<{
			user_id: number;
			alias: string;
			wins: number;
			losses: number;
			points_for: number;
			points_against: number;
			games_played: number;
			total_duration: number;
		}>(
			`WITH per_match AS (
      			SELECT
			        tp.user_id,
    			    tp.alias,
   				    m.id AS match_id,
 			        m.winner_id,
        			COALESCE(m.duration_seconds, 0) AS duration_seconds,
        			mp.score AS points_for,
       				opp.score AS points_against
      			FROM tournament_participants tp
      			JOIN matches m
        			ON m.tournament_id = tp.tournament_id
      			JOIN match_participants mp
        			ON mp.match_id = m.id AND mp.user_id = tp.user_id
      			JOIN match_participants opp
        			ON opp.match_id = m.id AND opp.user_id <> tp.user_id
      			WHERE tp.tournament_id = ?
    		)
    		SELECT
      			user_id,
      			alias,
      			SUM(CASE WHEN winner_id = user_id THEN 1 ELSE 0 END) AS wins,
      			SUM(CASE WHEN winner_id IS NOT NULL AND winner_id <> user_id THEN 1 ELSE 0 END) AS losses,
      			SUM(points_for) AS points_for,
      			SUM(points_against) AS points_against,
      			COUNT(CASE WHEN duration_seconds IS NOT NULL THEN 1 END) AS games_played,
      			SUM(duration_seconds) AS total_duration
    		FROM per_match
    		GROUP BY user_id, alias
    		`,
			[tournamentId],
		);

		const rows: TournamentRanking[] = base
			.map((p) => ({
				user_id: p.user_id,
				alias: p.alias,
				wins: p.wins,
				losses: p.losses,
				points_for: p.points_for,
				points_against: p.points_against,
				points_diff: p.points_for - p.points_against,
				games_played: p.games_played,
				total_duration: p.total_duration,
				position: 0,
			}))
			.sort((a, b) => {
				if (b.wins !== a.wins) return b.wins - a.wins;
				if (b.points_diff !== a.points_diff)
					return b.points_diff - a.points_diff;
				if (b.points_for !== a.points_for)
					return b.points_for - a.points_for;
				return 0;
			});

		await this.applyHeadToHeadResolution(tournamentId, rows);

		rows.forEach((r, idx) => (r.position = idx + 1));
		return rows;
	}

	//This sets the final position for all tournament participants (based on number of wins, then total score, then head to head)
	async setTournamentParticipantFinalPosition(
		tournamentId: number,
	): Promise<void> {
		try {
			await this.db.run("BEGIN IMMEDIATE");

			const tournamentRanking =
				await this.getTournamentRanking(tournamentId);

			for (const row of tournamentRanking) {
				await this.db.run(
					`
					UPDATE tournament_participants
					SET final_position = ?
					WHERE tournament_id = ? AND user_id = ?
					`,
					[row.position, tournamentId, row.user_id],
				);
			}
			await this.db.run("COMMIT");
		} catch (error) {
			await this.db.run("ROLLBACK");
			throw error;
		}
	}

	//This changes the status of the tournament to finished
	async finishTournament(id: number): Promise<void> {
		let tournament = await this.getTournamentById(id);

		if (!tournament) {
			throw new Error(`Tournament ${id} not found`);
		}
		if (tournament.finished_at) {
			throw new Error(`Tournament ${id} has already finished`);
		}

		const unplayedMatches = await this.db.get<{ count: number }>(
			`SELECT COUNT(*) as count
			FROM matches 
			WHERE tournament_id = ? AND duration_seconds IS NULL`,
			[id],
		);

		if (unplayedMatches && unplayedMatches.count > 0) {
			throw new Error(
				`Cannot finish tournament as ${unplayedMatches.count} matches still unplayed`,
			);
		}

		await this.setTournamentParticipantFinalPosition(id);

		const winner = await this.db.get<{ user_id: number }>(
			`SELECT user_id
			FROM tournament_participants
			WHERE tournament_id = ? AND final_position = 1`,
			[id],
		);

		await this.db.run(
			`
			UPDATE tournaments 
			SET finished_at = CURRENT_TIMESTAMP, 
				status = 'finished', 
				winner_id = ? 
			WHERE id = ?`,
			[winner?.user_id || null, id],
		);
	}

	//This gets us a table with the list of all matches from a tournament
	async getMatchesForTournament(
		tournamentId: number,
	): Promise<MatchWithPlayers[]> {
		const matches = await this.db.all<Match>(
			`
			SELECT *
			FROM matches
			WHERE tournament_id = ?
			ORDER BY round_number, match_number
			`,
			[tournamentId],
		);

		const matchesWithPlayers = await Promise.all(
			matches.map(async (match) => {
				const players = await this.db.all<{
					user_id: number | null;
					username: string | null;
					score: number;
				}>(
					`SELECT mp.user_id, u.username, mp.score
					FROM match_participants mp
					LEFT JOIN users u ON mp.user_id = u.id
					WHERE mp.match_id = ?`,
					[match.id],
				);
				return { ...match, players };
			}),
		);
		return matchesWithPlayers;
	}

	//When adding a participant to a tournament, we pass the id of the tournament, the userId and the alias chosen for the tournament
	async addTournamentParticipant(
		tournamentId: number,
		userId: number,
		alias: string,
	): Promise<void> {
		await this.db.run(
			`
			INSERT INTO tournament_participants (tournament_id, user_id, alias) VALUES (?, ?, ?)
			`,
			[tournamentId, userId, alias],
		);
	}

	//Remove participants from a tournament
	async removeTournamentParticipant(
		tournamentId: number,
		userId: number,
	): Promise<void> {
		await this.db.run(
			`DELETE FROM tournament_participants WHERE tournament_id = ? AND user_id = ?`,
			[tournamentId, userId],
		);
	}

	//Check if user is already in a tournament
	async isUserInTournament(
		tournamentId: number,
		userId: number,
	): Promise<boolean> {
		const result = await this.db.get<{ count: number }>(
			`SELECT COUNT(*) as count FROM tournament_participants WHERE tournament_id = ? AND user_id = ?`,
			[tournamentId, userId],
		);
		return (result?.count ?? 0) > 0;
	}

	//Check if alias is already in a tournament
	async isAliasInTournament(
		tournamentId: number,
		alias: string,
	): Promise<boolean> {
		const result = await this.db.get<{ count: number }>(
			`SELECT COUNT(*) as count FROM tournament_participants WHERE tournament_id = ? AND alias = ?`,
			[tournamentId, alias],
		);
		return (result?.count ?? 0) > 0;
	}

	//This finds all the tournaments of to which a player has participated
	async allTournamentsByUser(userId: number): Promise<Tournament[]> {
		return this.db.all(
			`SELECT t.* FROM tournaments t JOIN tournament_participants tp ON tp.tournament_id = t.id WHERE tp.user_id = ?`,
			[userId],
		);
	}

	//Get all tournament participants
	async getTournamentParticipants(
		tournamentID: number,
	): Promise<TournamentParticipant[]> {
		return this.db.all<TournamentParticipant>(
			`SELECT * FROM tournament_participants WHERE tournament_id = ?`,
			[tournamentID],
		);
	}

	//Check if tournament is already full
	async isTournamentFull(tournamentId: number): Promise<boolean> {
		const tournament = await this.getTournamentById(tournamentId);
		if (!tournament) return false;

		const participants = await this.db.get<{ count: number }>(
			`SELECT COUNT(*) as count FROM tournament_participants WHERE tournament_id = ?`,
			[tournamentId],
		);

		return (participants?.count ?? 0) >= tournament.max_participants;
	}

	//Get next match to be played
	async nextMatchToBePlayed(
		tournamentId: number,
	): Promise<MatchWithPlayers | null> {
		const match = await this.db.get<Match>(
			`SELECT * FROM matches
			WHERE tournament_id = ? AND duration_seconds IS NULL
			ORDER BY round_number ASC, match_number ASC
			LIMIT 1`,
			[tournamentId],
		);
		if (!match) return null;

		const players = await this.db.all<{
			user_id: number | null;
			username: string | null;
			score: number;
		}>(
			`SELECT mp.user_id, u.username, mp.score
         	FROM match_participants mp
         	LEFT JOIN users u ON mp.user_id = u.id
        	 WHERE mp.match_id = ?`,
			[match.id],
		);

		return { ...match, players };
	}

	//########################[DASHBOARD]########################

	//This route returns some basic stats of a user, with the number of games played, the number of victories, defeats and the user's average score
	async getUserBasicStats(userId: number): Promise<BasicStatsRow> {
		const result = await this.db.get<BasicStatsRow>(
			`
		SELECT
			COUNT(DISTINCT m.id) AS gamesPlayed,
			SUM(CASE WHEN m.winner_id = ? THEN 1 ELSE 0 END) AS victories,
			SUM(CASE WHEN m.winner_id <> ? THEN 1 ELSE 0 END) AS defeats,
			AVG(mp.score) AS averageScore
		FROM matches m
		JOIN match_participants mp
			ON mp.match_id = m.id AND mp.user_id = ?
		WHERE m.winner_id IS NOT NULL
		AND m.id IN (
			SELECT match_id
			FROM match_participants
			GROUP BY match_id
			HAVING SUM(CASE WHEN is_ai = 1 THEN 1 ELSE 0 END) = 0
			)
		`,
			[userId, userId, userId],
		);
		return {
			gamesPlayed: result?.gamesPlayed || 0,
			victories: result?.victories || 0,
			defeats: result?.defeats || 0,
			averageScore: result?.averageScore || 0,
		};
	}

	//This call returns the most frequent adversary of a user with statistics about the number of wins and losses gainst that user
	async getUserMostFrequentAdversary(
		userId: number,
	): Promise<AdversaryRow | null> {
		const result = await this.db.get<AdversaryRow | null>(
			`
		SELECT 
			opp.user_id AS opponent_id,
			u.username AS username,
			COUNT(*) AS games_against,
			SUM (CASE WHEN m.winner_id = me.user_id THEN 1 ELSE 0 END) as wins_against,
			SUM (CASE WHEN m.winner_id = opp.user_id THEN 1 ELSE 0 END) as losses_against
		FROM match_participants me
		JOIN match_participants opp
			ON opp.match_id = me.match_id
			AND opp.user_id <> me.user_id
		JOIN matches m 
			ON m.id = me.match_id
		JOIN users u
			ON u.id = opp.user_id
		WHERE me.user_id = ?
			AND m.winner_id IS NOT NULL
			AND m.id IN (
				SELECT match_id
				FROM match_participants
				GROUP BY match_id
				HAVING SUM(CASE WHEN is_ai = 1 THEN 1 ELSE 0 END) = 0
			)
		GROUP BY opp.user_id, u.username
		ORDER BY games_against DESC
		LIMIT 1
		`,
			[userId],
		);
		if (!result) return null;
		return {
			opponent_id: result.opponent_id,
			username: result.username,
			games_against: result.games_against,
			wins_against: result.wins_against,
			losses_against: result.losses_against,
		};
	}

	//This query returns the last 5 games played by a user
	async getUserRecentMatches(userId: number): Promise<MatchRow[]> {
		const result = await this.db.all<MatchRow>(
			`
			SELECT
				m.id AS matchId,
				opp.user_id AS opponentId,
				u.username AS opponentName,
				me.score AS userScore,
				opp.score AS opponentScore
			FROM match_participants me
			JOIN match_participants opp
				ON opp.match_id = me.match_id
				AND opp.user_id <> me.user_id
			JOIN matches m
				ON m.id = me.match_id
			JOIN users u
				ON u.id = opp.user_id
			WHERE me.user_id = ?
			AND opp.is_ai = 0
			AND m.duration_seconds IS NOT NULL
			ORDER BY m.id DESC
			LIMIT 5
		`,
			[userId],
		);
		return result ?? [];
	}

	//This is a single call that will return all stats needed for a user dashboard (basic stats, most frequent adversary and recent games)
	async getFullUserDashboard(userId: number): Promise<UserDashboard> {
		const [stats, adversary, matches] = await Promise.all([
			this.getUserBasicStats(userId),
			this.getUserMostFrequentAdversary(userId),
			this.getUserRecentMatches(userId),
		]);

		return {
			gamesPlayed: stats.gamesPlayed,
			victories: stats.victories,
			defeats: stats.defeats,
			averageScore: stats.averageScore,
			mostFrequentAdversary: adversary,
			recentMatches: matches,
		};
	}

	//This query allows to display stats between two players, with number of games played and number of wins per player
	async get1v1Stats(
		userAId: number,
		userBId: number,
	): Promise<vsStats | null> {
		const result = await this.db.get<vsStats | null>(
			`
		SELECT 
			uA.id AS playerAId,
			uA.username AS playerAUsername,
			uB.id AS playerBId,
			uB.username AS playerBUsername,
			COUNT(*) AS gamesPlayed,
			SUM (CASE WHEN m.winner_id = ? THEN 1 ELSE 0 END) as playerAWins,
			SUM (CASE WHEN m.winner_id = ? THEN 1 ELSE 0 END) as playerBWins
		FROM matches m
		JOIN match_participants mpA
			ON mpA.match_id = m.id
			AND mpA.user_id = ?
		JOIN match_participants mpB
			ON mpB.match_id = m.id
			AND mpB.user_id = ?
		JOIN users uA ON uA.id = mpA.user_id
		JOIN users uB ON uB.id = mpB.user_id
		WHERE m.winner_id IS NOT NULL
		AND m.id IN (
			SELECT match_id
			FROM match_participants
			GROUP BY match_id
			HAVING SUM(CASE WHEN is_ai = 1 THEN 1 ELSE 0 END) = 0
		)
		GROUP BY uA.id, uA.username, uB.id, uB.username
		`,
			[userAId, userBId, userAId, userBId],
		);
		if (!result) return null;
		return {
			playerAId: userAId,
			playerAUsername: result.playerAUsername,
			playerBId: userBId,
			playerBUsername: result.playerBUsername,
			gamesPlayed: result.gamesPlayed,
			playerAWins: result.playerAWins,
			playerBWins: result.playerBWins,
		};
	}

	//########################[CHAT THREADS]########################

	// Create a 1-1 thread or group thread
	async createThread(userIds: number[], isGroup = 0): Promise<number> {
		// TODO need to add checker if users blocked
		const result = await this.db.run(
			"INSERT INTO chat_threads (is_group) VALUES (?)",
			[isGroup],
		);

		const threadId = result.lastInsertRowid as number;

		for (const uid of userIds) {
			await this.db.run(
				`INSERT INTO chat_thread_members (thread_id, user_id)
				VALUES (?, ?)`,
				[threadId, uid],
			);
		}

		return threadId;
	}

	// Find existing 1-1 thread
	async getDirectThread(
		userA: number,
		userB: number,
	): Promise<number | null> {
		const row = await this.db.get<{ thread_id: number }>(
			`SELECT m1.thread_id
			FROM chat_thread_members m1
			JOIN chat_thread_members m2 ON m1.thread_id = m2.thread_id
			JOIN chat_threads t ON t.id = m1.thread_id
			WHERE t.is_group = 0
			AND m1.user_id = ?
			AND m2.user_id = ?`,
			[userA, userB],
		);

		return row?.thread_id ?? null;
	}

	async getUserThreads(userId: number) {
		const rows = await this.db.all(
			`SELECT
			t.id AS thread_id,
			t.created_at,
			m2.user_id AS participant_id
			FROM chat_threads t
			JOIN chat_thread_members m1
			ON m1.thread_id = t.id
			AND m1.user_id = ?
				JOIN chat_thread_members m2
			ON m2.thread_id = t.id
			ORDER BY t.created_at DESC`,
			[userId],
		);

		const threadsById = new Map<number, any>();

		for (const row of rows) {
			if (!threadsById.has(row.thread_id)) {
				threadsById.set(row.thread_id, {
					id: row.thread_id,
					name: row.name,
					created_at: row.created_at,
					participants: [],
				});
			}

			threadsById
				.get(row.thread_id)
				.participants.push(row.participant_id);
		}

		return Array.from(threadsById.values());
	}

	//########################[MESSAGES]########################

	// Send a message
	async createMessage(
		threadId: number,
		senderId: number,
		content: string,
		messageType: string = "text",
		metadata: any = null,
	): Promise<number> {
		const result = await this.db.run(
			`INSERT INTO messages (
			thread_id,
			sender_id,
			content,
			message_type, metadata)
		VALUES (?, ?, ?, ?, ?)`,
			[
				threadId,
				senderId,
				content,
				messageType,
				JSON.stringify(metadata),
			],
		);

		return result.lastInsertRowid as number;
	}

	// Get messages from a thread
	async getMessages(threadId: number, limit = 50, offset = 0) {
		return this.db.all(
			`SELECT m.*, u.username, u.display_name, u.avatar_url
			FROM messages m
			JOIN users u ON u.id = m.sender_id
			WHERE thread_id = ?
			ORDER BY created_at ASC
			LIMIT ? OFFSET ?`,
			[threadId, limit, offset],
		);
	}

	// Mark all messages as read
	async markThreadAsRead(
		threadId: number,
		userId: number,
		lastMessageId: number,
	) {
		await this.db.run(
			`UPDATE chat_thread_members
			SET last_read_message_id = ?
			WHERE thread_id = ? AND user_id = ?`,
			[lastMessageId, threadId, userId],
		);
	}

	//########################[RELATIONSHIPS: FRIENSHIP & BLOCKING]########################

	/**
	 * Add or update relationship between users
	 * @param userId - User initiating the relationship
	 * @param contactId - Target user
	 * @param status - 'friends' or 'blocked'
	 */
	async setUserRelationship(
		userId: number,
		contactId: number,
		status: "friends" | "blocked",
	): Promise<void> {
		await this.db.run(
			`INSERT INTO users_relationships (user_id, contact_id, status)
			VALUES (?, ?, ?)
			ON CONFLICT(user_id, contact_id) 
			DO UPDATE SET status = ?, created_at = CURRENT_TIMESTAMP`,
			[userId, contactId, status, status],
		);
	}

	async getBlockedFriends(userId: number) {
		return this.db.all(
			`SELECT contact_id as id, u.username, u.display_name, u.avatar_url
		  FROM users_relationships ur
		  JOIN users u ON u.id = ur.contact_id
		  WHERE ur.user_id = ? AND ur.status = 'blocked'`,
			[userId],
		);
	}

	// Remove relationship (unfriend or unblock)
	async removeUserRelationship(
		userId: number,
		contactId: number,
	): Promise<void> {
		await this.db.run(
			`DELETE FROM users_relationships 
			WHERE user_id = ? AND contact_id = ?`,
			[userId, contactId],
		);
	}

	// Get relationship status between two users
	async getRelationshipStatus(
		userId: number,
		contactId: number,
	): Promise<"pending" | "friends" | "blocked" | null> {
		const row = await this.db.get<{ status: string }>(
			`SELECT status FROM users_relationships 
			WHERE user_id = ? AND contact_id = ?`,
			[userId, contactId],
		);
		return row?.status as "pending" | "friends" | "blocked" | null;
	}

	// Check if two users are friends
	async areFriends(userId: number, contactId: number): Promise<boolean> {
		const result = await this.db.get<{ count: number }>(
			`SELECT COUNT(*) as count FROM users_relationships 
			WHERE ((user_id = ? AND contact_id = ?) OR (user_id = ? AND contact_id = ?))
			AND status = 'friends'`,
			[userId, contactId, contactId, userId],
		);
		return (result?.count ?? 0) === 1;
	}

	// Block a user (removes any existing relationships, sets blocked status)
	async blockUser(blockerId: number, blockedId: number): Promise<void> {
		await this.db.run(
			`DELETE FROM users_relationships 
			WHERE ((user_id = ? AND contact_id = ?) OR (user_id = ? AND contact_id = ?))
			AND status IN ('friends', 'pending')`,
			[blockerId, blockedId, blockedId, blockerId],
		);
		await this.setUserRelationship(blockerId, blockedId, "blocked");
	}

	// Unblock a user (replaces old unblockUser)
	async unblockUser(blockerId: number, blockedId: number): Promise<void> {
		await this.db.run(
			`UPDATE users_relationships
			SET status = 'friends', created_at = CURRENT_TIMESTAMP
			WHERE user_id = ? AND contact_id = ? AND status = 'blocked'`,
			[blockerId, blockedId],
		);
	}

	// Check if user A has blocked user B (replaces old isBlocked)
	async isBlocked(blockerId: number, blockedId: number): Promise<boolean> {
		const status = await this.getRelationshipStatus(blockerId, blockedId);
		return status === "blocked";
	}

	// Check if either user has blocked the other (bidirectional check)
	async isBlockedBidirectional(
		userA: number,
		userB: number,
	): Promise<boolean> {
		const blockedByA = await this.isBlocked(userA, userB);
		const blockedByB = await this.isBlocked(userB, userA);
		return blockedByA || blockedByB;
	}

	// Add a friend (single record, smaller ID first for consistency)
	async addFriend(userId: number, friendId: number): Promise<void> {
		const [user1, user2] =
			userId < friendId ? [userId, friendId] : [friendId, userId];
		await this.setUserRelationship(user1, user2, "friends");
	}

	// Remove a friend
	async removeFriend(userId: number, friendId: number): Promise<void> {
		const [user1, user2] =
			userId < friendId ? [userId, friendId] : [friendId, userId];
		await this.db.run(
			`DELETE FROM users_relationships 
			WHERE (user_id = ? AND contact_id = ?)
			AND status = 'friends'`,
			[userId, friendId],
		);
	}

	//########################[FRIEND REQUESTS]########################

	// Send friend request (creates pending relationship)
	async sendFriendRequest(
		senderId: number,
		receiverId: number,
	): Promise<void> {
		// Check if sender is blocked by receiver
		const receiverToSenderStatus = await this.getRelationshipStatus(
			receiverId,
			senderId,
		);
		if (receiverToSenderStatus === "blocked") {
			throw new Error("Cannot send friend request - you are blocked");
		}

		// Check if sender has blocked receiver
		const senderToReceiverStatus = await this.getRelationshipStatus(
			senderId,
			receiverId,
		);
		if (senderToReceiverStatus === "blocked") {
			throw new Error("Cannot send friend request - user is blocked");
		}

		// Check if already friends (check both directions since friends is single record)
		if (await this.areFriends(senderId, receiverId)) {
			throw new Error("Already friends");
		}

		// Check if pending request already exists
		if (senderToReceiverStatus === "pending") {
			throw new Error("Friend request already sent");
		}

		// Check if there's a reverse pending request
		if (receiverToSenderStatus === "pending") {
			throw new Error("This user already sent you a friend request");
		}

		// Create pending request
		await this.db.run(
			`INSERT INTO users_relationships (user_id, contact_id, status) 
			VALUES (?, ?, 'pending')`,
			[senderId, receiverId],
		);
	}

	// Accept friend request
	async acceptFriendRequest(
		senderId: number,
		receiverId: number,
	): Promise<boolean> {
		const request = await this.db.get<{
			user_id: number;
			contact_id: number;
		}>(
			`SELECT user_id, contact_id FROM users_relationships 
			WHERE user_id = ? AND contact_id = ? AND status = 'pending'`,
			[senderId, receiverId],
		);

		if (!request) return false;

		if (await this.isBlockedBidirectional(senderId, receiverId)) {
			return false;
		}

		await this.db.run(
			`UPDATE users_relationships 
			SET status = 'friends', responded_at = CURRENT_TIMESTAMP 
			WHERE user_id = ? AND contact_id = ?`,
			[senderId, receiverId],
		);

		return true;
	}

	// Reject friend request
	async rejectFriendRequest(
		senderId: number,
		receiverId: number,
	): Promise<boolean> {
		const result = await this.db.run(
			`DELETE FROM users_relationships 
			WHERE user_id = ? AND contact_id = ? AND status = 'pending'`,
			[senderId, receiverId],
		);
		return (result.changes ?? 0) > 0;
	}

	// Cancel friend request
	async cancelFriendRequest(
		senderId: number,
		receiverId: number,
	): Promise<boolean> {
		const result = await this.db.run(
			`DELETE FROM users_relationships 
			WHERE user_id = ? AND contact_id = ? AND status = 'pending'`,
			[senderId, receiverId],
		);
		return (result.changes ?? 0) > 0;
	}

	// Get pending friend requests received by user
	async getReceivedFriendRequests(userId: number): Promise<
		{
			id: number;
			sender_id: number;
			username: string;
			avatar_url: string | null;
			created_at: string;
		}[]
	> {
		return this.db.all(
			`SELECT ur.id, ur.user_id as sender_id, u.username, u.avatar_url, ur.created_at
			FROM users_relationships ur
			JOIN users u ON ur.user_id = u.id
			WHERE ur.contact_id = ? AND ur.status = 'pending'
			ORDER BY ur.created_at DESC`,
			[userId],
		);
	}

	// Get pending friend requests sent by user
	async getSentFriendRequests(userId: number): Promise<
		{
			id: number;
			receiver_id: number;
			username: string;
			avatar_url: string | null;
			created_at: string;
		}[]
	> {
		return this.db.all(
			`SELECT ur.id, ur.contact_id as receiver_id, u.username, u.avatar_url, ur.created_at
			FROM users_relationships ur
			JOIN users u ON ur.contact_id = u.id
			WHERE ur.user_id = ? AND ur.status = 'pending'
			ORDER BY ur.created_at DESC`,
			[userId],
		);
	}

	// Get all friends of a user (check both directions since only one record exists)
	async getFriends(userId: number): Promise<PublicUser[]> {
		return this.db.all<PublicUser>(
			`SELECT u.id, u.username, u.email, u.avatar_url, u.created_at, u.updated_at, u.last_seen_at
			FROM users u
			JOIN users_relationships ur ON 
				(ur.user_id = ? AND ur.contact_id = u.id) OR
				(ur.contact_id = ? AND ur.user_id = u.id)
			WHERE ur.status = 'friends'
			ORDER BY u.username`,
			[userId, userId],
		);
	}

	// Get all relations of a user (check both directions since only one record exists)
	async getRelations(userId: number): Promise<PublicUser[]> {
		return this.db.all<PublicUser>(
			`SELECT u.id, u.username, u.email, u.avatar_url, u.created_at, u.updated_at, u.last_seen_at
			FROM users u
			JOIN users_relationships ur ON 
				(ur.user_id = ? AND ur.contact_id = u.id) OR
				(ur.contact_id = ? AND ur.user_id = u.id)
			WHERE ur.status = 'friends' OR ur.status = 'blocked'
			ORDER BY u.username`,
			[userId, userId],
		);
	}

	// Get all blocked users
	async getBlockedUsers(userId: number): Promise<PublicUser[]> {
		return this.db.all<PublicUser>(
			`SELECT u.id, u.username, u.email, u.avatar_url, u.created_at, u.updated_at
			FROM users u
			JOIN users_relationships ur ON u.id = ur.contact_id
			WHERE ur.user_id = ? AND ur.status = 'blocked'
			ORDER BY u.username`,
			[userId],
		);
	}

	// Get all relationships of a user (friends in both directions, blocked only outgoing)
	async getAllRelationships(userId: number): Promise<
		{
			contact_id: number;
			username: string;
			status: "friends" | "blocked";
			created_at: string;
		}[]
	> {
		return this.db.all(
			`SELECT ur.contact_id, u.username, ur.status, ur.created_at
			FROM (
				SELECT 
					CASE WHEN user_id = ? THEN contact_id ELSE user_id END as contact_id,
					status,
					created_at
				FROM users_relationships
				WHERE (user_id = ? OR contact_id = ?)
				AND (status = 'friends' OR (status = 'blocked' AND user_id = ?))
			) ur
			JOIN users u ON u.id = ur.contact_id
			ORDER BY ur.created_at DESC`,
			[userId, userId, userId, userId],
		);
	}

	// Count friends (check both directions since only one record exists)
	async countFriends(userId: number): Promise<number> {
		const result = await this.db.get<{ count: number }>(
			`SELECT COUNT(*) as count 
			FROM users_relationships 
			WHERE ((user_id = ?) OR (contact_id = ?)) AND status = 'friends'`,
			[userId, userId],
		);
		return result?.count ?? 0;
	}

	// Count blocked users
	async countBlockedUsers(userId: number): Promise<number> {
		const result = await this.db.get<{ count: number }>(
			`SELECT COUNT(*) as count 
			FROM users_relationships 
			WHERE user_id = ? AND status = 'blocked'`,
			[userId],
		);
		return result?.count ?? 0;
	}

	//########################[GAME INVITATION]########################

	// Get invitation by ID
	async getGameInvitationById(invitationId: number) {
		return this.db.get<{
			id: number;
			sender_id: number;
			receiver_id: number;
			game_type_id: number;
			status: string;
		}>(
			`SELECT id, sender_id, receiver_id, game_type_id, status
			FROM game_invitations WHERE id = ?`,
			[invitationId],
		);
	}

	// Create an invitation
	async createGameInvitation(
		senderId: number,
		receiverId: number,
		gameTypeId: number,
	) {
		const res = await this.db.run(
			`INSERT INTO game_invitations (sender_id, receiver_id, game_type_id)
			VALUES (?, ?, ?)`,
			[senderId, receiverId, gameTypeId],
		);

		return res.lastInsertRowid as number;
	}

	// Response to invitation
	async respondToGameInvitation(invitationId: number, status: string) {
		await this.db.run(
			`UPDATE game_invitations SET status = ? WHERE id = ?`,
			[status, invitationId],
		);
	}

	//########################[NOTIFICATIONS]########################

	// Create a notification
	async createNotification(
		userId: number,
		message: string,
		type: string,
		metadata: any = null,
	) {
		await this.db.run(
			`INSERT INTO notifications (
				user_id, message,
				notification_type,
				metadata
			) VALUES (?, ?, ?, ?)`,
			[userId, message, type, JSON.stringify(metadata)],
		);
	}

	// Select all notification from an user
	async listNotifications(userId: number) {
		return this.db.all(
			`SELECT *
			FROM notifications
			WHERE user_id = ?
			ORDER BY created_at DESC`,
			[userId],
		);
	}

	// Set notification as read
	async markNotificationAsRead(id: number) {
		await this.db.run(`UPDATE notifications SET is_read = 1 WHERE id = ?`, [
			id,
		]);
	}

	//#####################[ ONE TIME PASSWORD ]########################

	// Save an one time password for 2FA
	async saveOTP(data: NewUsersOTP): Promise<string> {
		try {
			await this.db.run("BEGIN IMMEDIATE");
			await this.db.run(
				`
					DELETE FROM users_one_time_password
					WHERE user_id = ?
				`,
				[data.user_id],
			);
			await this.db.run(
				`
					INSERT INTO users_one_time_password
					(user_id, otp_hash, expire_at, challenge_id)
					VALUES (?, ?, ?, ?)
				`,
				[
					data.user_id,
					data.otp_hash,
					data.expire_at.toISOString(),
					data.challenge_id,
				],
			);
			await this.db.run("COMMIT");
			return data.challenge_id;
		} catch (error) {
			//console.log("saveOTP failed", error);
			await this.db.run("ROLLBACK");
			throw error;
		}
	}

	async getUserIdByChallengeId(challengeId: string): Promise<number | null> {
		const result = await this.db.get<{ user_id: number }>(
			`SELECT user_id
			 FROM users_one_time_password
			 WHERE challenge_id = ?`,
			[challengeId],
		);
		return result?.user_id ?? null;
	}

	// Return a one time password by userId
	async getOTPByUserId(userId: number): Promise<UsersOTP | undefined> {
		return this.db.get<UsersOTP>(
			`
				SELECT * FROM users_one_time_password WHERE user_id = ?
				ORDER BY expire_at DESC LIMIT 1
			`,
			[userId],
		);
	}

	// Delete a one time password by id
	async deleteOTP(id: number): Promise<boolean> {
		const result = await this.db.run(
			"DELETE FROM users_one_time_password WHERE id = ?",
			[id],
		);
		return result.changes > 0;
	}

	// Increase the otp attempt
	async increaseOTPAttempt(id: number): Promise<boolean> {
		const result = await this.db.run(
			`UPDATE users_one_time_password
			SET attempts = attempts + 1
			WHERE id = ?
			AND attempts < 3`,
			[id],
		);
		return result.changes === 1;
	}

	//########################[PRESENCE / ONLINE STATUS]########################

	/**
	 * Update user's last_seen_at timestamp to current time
	 * Online status is determined by checking if last_seen_at < 5 minutes ago
	 *
	 * @param userId - ID of user
	 */
	async updateUserLastSeen(userId: number): Promise<void> {
		await this.db.run(
			"UPDATE users SET last_seen_at = CURRENT_TIMESTAMP WHERE id = ?",
			[userId],
		);
	}

	/**
	 * Get all friends of a user who are currently online
	 * "Online" = last_seen_at within the last 5 minutes
	 *
	 * @param userId - ID of user whose online friends to get
	 * @returns Array of online friends with id, username, last_seen_at
	 */
	async getOnlineFriends(userId: number): Promise<
		Array<{
			id: number;
			username: string;
			last_seen_at: string;
		}>
	> {
		return this.db.all(
			`SELECT u.id, u.username, u.last_seen_at
			FROM users u
			JOIN users_relationships ur ON (
				(ur.user_id = ? AND ur.contact_id = u.id) OR
				(ur.contact_id = ? AND ur.user_id = u.id)
			)
			WHERE ur.status = 'friends'
			AND u.last_seen_at IS NOT NULL
			AND datetime(u.last_seen_at) > datetime('now', '-5 minutes')
			ORDER BY u.username`,
			[userId, userId],
		);
	}

	/**
	 * Get all friendship relationships for a user
	 * Returns single friendship records where user appears in either column
	 *
	 * @param userId - ID of user
	 * @returns Array of friendship records with user_id, contact_id, status
	 */
	async getUserFriendships(userId: number): Promise<
		Array<{
			user_id: number;
			contact_id: number;
			status: string;
		}>
	> {
		return this.db.all(
			`SELECT user_id, contact_id, status
			FROM users_relationships
			WHERE (user_id = ? OR contact_id = ?)
			AND status = 'friends'`,
			[userId, userId],
		);
	}

	//##################[HELPERS]##################
	async begin(): Promise<void> {
		await this.db.run("BEGIN IMMEDIATE");
	}

	async commit(): Promise<void> {
		await this.db.run("COMMIT");
	}

	async rollback(): Promise<void> {
		await this.db.run("ROLLBACK");
	}
}
