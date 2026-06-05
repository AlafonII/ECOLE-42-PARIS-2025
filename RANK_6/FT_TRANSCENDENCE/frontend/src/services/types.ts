//Default response from API
export interface ApiResponse<T> {
	success: boolean;
	data?: T;
	error?: string;
}

//User types
export interface PublicUser {
	id: number;
	username: string;
	email: string;
	avatar_url?: string | null;
	created_at: string;
	updated_at: string;
}

export interface CreateUserInput {
	username: string;
	email: string;
	password: string;
	avatar_url?: string | null;
	twoFactor: 0 | 1;
}

export interface LoginInput {
	email: string;
	password: string;
}

export interface CreateUserDTO {
	username: string;
	email: string;
	password_hash: string;
	avatar_url?: string | null;
	two_factor: 0 | 1;
}

export interface UpdateUserInput {
	username?: string;
	email?: string;
	password?: string;
	avatar_url?: string | null;
}

//Match types
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

export interface MatchParticipant {
	user_id: number | null;
	username?: string;
	score: number;
	is_ai: boolean;
}

export interface CreateMatchInput {
	gameTypeId: number;
	tournamentId?: number;
	roundNumber?: number;
	matchNumber?: number;
}

//Tournament types
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

export interface TournamentCreation {
	name: string;
	gameTypeId: number;
	participantsNb: number;
	organiserId: number;
}

export interface AddParticipantInput {
	tournamentId: number;
	userId: number;
	alias: string;
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

//Dashboard types
export interface UserDashboard {
	gamesPlayed: number;
	victories: number;
	defeats: number;
	averageScore: number;
	mostFrequentAdversary: Adversary | null;
	recentMatches: MatchRow[];
}

export interface Adversary {
	opponent_id: number;
	username: string;
	games_against: number;
	wins_against: number;
	losses_against: number;
}

export interface MatchRow {
	matchId: number;
	opponentId: number;
	opponentName: string;
	userScore: number;
	opponentScore: number;
}

export interface BasicStats {
	gamesPlayed: number;
	victories: number;
	defeats: number;
	averageScore: number;
}

export interface VsStats {
	playerAId: number;
	playerAUsername: string;
	playerBId: number;
	playerBUsername: string;
	gamesPlayed: number;
	playerAWins: number;
	playerBWins: number;
}

//Live chat types

export type ChatMessageType =
	| "TEXT"
	| "GAME_INVITE"
	| "GAME_INVITE_ACCEPTED"
	| "GAME_INVITE_DECLINED";

export interface ChatThread {
	id: number;
	is_group: boolean;
	created_at: string;
}

export interface ChatMessage {
	id: number;
	thread_id: number;
	sender_id: number;
	content: string;
	message_type: string;
	metadata?: any;
	created_at: string;
}

export interface SendMessageInput {
	threadId?: number;
	receiverId?: number;
	senderId: number;
	content: string;
	messageType?: string;
	metadata?: any;
}

export interface SendChatTextMessage extends SendMessageInput {
	messageType?: "TEXT";
}

export interface SendGameInviteMessage extends SendMessageInput {
	messageType: "GAME_INVITE";
	metadata: GameInviteMetadata;
}

export interface SendGameInviteAcceptedMessage extends SendMessageInput {
	messageType: "GAME_INVITE_ACCEPTED";
	metadata: GameInviteMetadata & { matchId: number };
}

export interface CreateThreadInput {
	userIds: number[];
	isGroup?: number;
}

export interface GameInvitation {
	id: number;
	sender_id: number;
	receiver_id: number;
	game_type_id: number;
	status: string;
	created_at: string;
}

export interface GameInviteMetadata {
	game: "pong";
	matchId?: number;
}

export interface InvitationResponse {
	message: string;
	matchId?: number;
}

export interface Notification {
	id: number;
	user_id: number;
	message: string;
	type: string;
	metadata?: any;
	read: boolean;
	created_at: string;
}
