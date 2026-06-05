//##################[BACKEND TYPES]##################

//USER TYPES
//##################[BACKEND TYPES]##################

//USER TYPES
export interface DBUser {
	id: number;
	username: string;
	email: string;
	password_hash: string;
	display_name: string;
	avatar_url?: string | null;
	two_factor: 0 | 1;
	created_at: string;
	updated_at: string;
}

export interface PublicUser {
	id: number;
	username: string;
	email: string;
	avatar_url?: string | null;
	created_at: string;
	updated_at: string;
	last_seen_at: string;
}

export interface CreateUserDTO {
	username: string;
	email: string;
	password_hash: string;
	avatar_url?: string | null;
	two_factor: 0 | 1;
}

//MATCH TYPES
export interface Match {
	id: number;
	game_type_id: number;
	winner_id?: number | null;
	duration_seconds?: number | null;
	tournament_id?: number | null;
	round_number?: number | null;
	match_number?: number | null;
	created_at: string;
}

export interface MatchWithPlayers extends Match {
	players: {
		user_id: number | null;
		username: string | null;
		score: number;
	}[];
}

export interface MatchRow {
	matchId: number;
	opponentId: number;
	opponentName: string;
	userScore: number;
	opponentScore: number;
}

//TOURNAMENT TYPES
export interface Tournament {
	id: number;
	name: string;
	game_type_id: number;
	status: string;
	max_participants: number;
	winner_id?: number | null;
	tournament_organizer_id: number;
	created_at: string;
	started_at?: string | null;
	finished_at?: string | null;
}

export interface TournamentParticipant {
	id: number;
	tournament_id: number;
	user_id: number;
	alias: string;
	final_position?: number | null;
	created_at: string;
}

export interface TournamentRanking {
	user_id: number;
	alias: string;
	wins: number;
	losses: number;
	points_for: number;
	points_against: number;
	points_diff: number;
	games_played: number;
	total_duration: number;
	position: number;
}

//DASHBOARD TYPES
export interface UserDashboard {
	gamesPlayed: number;
	victories: number;
	defeats: number;
	averageScore: number;
	mostFrequentAdversary: AdversaryRow | null;
	recentMatches: MatchRow[];
}

export interface BasicStatsRow {
	gamesPlayed: number;
	victories: number;
	defeats: number;
	averageScore: number;
}

export interface AdversaryRow {
	opponent_id: number;
	username: string;
	games_against: number;
	wins_against: number;
	losses_against: number;
}

export interface vsStats {
	playerAId: number;
	playerAUsername: string;
	playerBId: number;
	playerBUsername: string;
	gamesPlayed: number;
	playerAWins: number;
	playerBWins: number;
}

export interface DBChatThread {
	id: number;
	is_group: 0 | 1;
	created_at: string;
}

export interface ChatThread {
	id: number;
	is_group: boolean;
	created_at: string;
	members?: PublicUser[];
	last_message?: Message;
}

export interface DBChatThreadMember {
	id: number;
	thread_id: number;
	user_id: number;
	last_read_message_id?: number | null;
	joined_at: string;
}

export interface ChatThreadMember {
	user: PublicUser;
	last_read_message_id?: number | null;
	joined_at: string;
}

export interface DBMessage {
	id: number;
	thread_id: number;
	sender_id: number;
	message_type: "text" | "system";
	content?: string | null;
	metadata?: unknown;
	created_at: string;
}

export interface Message {
	id: number;
	sender: PublicUser;
	content?: string | null;
	message_type: "text" | "system";
	created_at: string;
}

export interface DBNotification {
	id: number;
	user_id: number;
	message: string;
	notification_type?: string | null;
	metadata?: unknown;
	is_read: 0 | 1;
	created_at: string;
}

export interface Notification {
	id: number;
	message: string;
	notification_type?: string | null;
	metadata?: unknown;
	is_read: boolean;
	created_at: string;
}

export interface UsersOTP {
	id: number;
	user_id: number;
	otp_hash: string;
	expire_at: Date;
	attempts: number;
	challenge_id: string;
}

export interface NewUsersOTP {
	user_id: number;
	otp_hash: string;
	expire_at: Date;
	challenge_id: string;
}

export interface SendMessageDTO {
	thread_id: number;
	content: string;
}

export interface CreateChatThreadDTO {
	user_ids: number[];
	is_group?: boolean;
}
