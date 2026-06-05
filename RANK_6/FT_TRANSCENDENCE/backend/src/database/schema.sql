PRAGMA foreign_keys = ON;

CREATE TABLE
	IF NOT EXISTS users (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		username VARCHAR(50) UNIQUE NOT NULL,
		email VARCHAR(100) UNIQUE NOT NULL,
		password_hash VARCHAR(255) NOT NULL,
		display_name VARCHAR(50) UNIQUE NOT NULL,
		avatar_url VARCHAR(255),
		two_factor BOOLEAN DEFAULT 0,
		created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
		updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
		last_seen_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
	);

CREATE TABLE
	IF NOT EXISTS game_types (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		name VARCHAR(50) UNIQUE NOT NULL,
		min_players INTEGER DEFAULT 2,
		max_players INTEGER DEFAULT 2
	);

CREATE TABLE
	IF NOT EXISTS tournaments (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		name VARCHAR(100) NOT NULL,
		game_type_id INTEGER NOT NULL,
		status VARCHAR(20) DEFAULT "registration",
		max_participants INTEGER DEFAULT 8,
		winner_id INTEGER,
		tournament_organizer_id INTEGER NOT NULL,
		created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
		started_at TIMESTAMP,
		finished_at TIMESTAMP,
		FOREIGN KEY (winner_id) REFERENCES users (id) ON DELETE SET NULL,
		FOREIGN KEY (tournament_organizer_id) REFERENCES users (id) ON DELETE RESTRICT,
		FOREIGN KEY (game_type_id) REFERENCES game_types (id) ON DELETE RESTRICT
	);

CREATE TABLE
	IF NOT EXISTS matches (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		game_type_id INTEGER NOT NULL,
		winner_id INTEGER,
		duration_seconds INTEGER,
		tournament_id INTEGER,
		round_number INTEGER,
		match_number INTEGER,
		created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
		FOREIGN KEY (game_type_id) REFERENCES game_types (id) ON DELETE RESTRICT,
		FOREIGN KEY (tournament_id) REFERENCES tournaments (id) ON DELETE CASCADE,
		FOREIGN KEY (winner_id) REFERENCES users (id) ON DELETE SET NULL,
		UNIQUE (tournament_id, round_number, match_number)
	);

CREATE TABLE
	IF NOT EXISTS tournament_participants (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		tournament_id INTEGER NOT NULL,
		user_id INTEGER NOT NULL,
		alias VARCHAR(50) NOT NULL,
		final_position INTEGER,
		created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
		FOREIGN KEY (tournament_id) REFERENCES tournaments (id) ON DELETE CASCADE,
		FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE,
		UNIQUE (tournament_id, user_id)
	);

CREATE TABLE
	IF NOT EXISTS match_participants (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		match_id INTEGER NOT NULL,
		user_id INTEGER,
		is_ai BOOLEAN DEFAULT 0,
		score INTEGER DEFAULT 0,
		FOREIGN KEY (match_id) REFERENCES matches (id) ON DELETE CASCADE,
		FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE SET NULL,
		UNIQUE (match_id, user_id)
	);

CREATE TABLE
	IF NOT EXISTS chat_threads (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		is_group BOOLEAN DEFAULT 0,
		created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
	);

CREATE TABLE
	IF NOT EXISTS chat_thread_members (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		thread_id INTEGER NOT NULL,
		user_id INTEGER NOT NULL,
		last_read_message_id INTEGER,
		joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
		FOREIGN KEY (thread_id) REFERENCES chat_threads (id) ON DELETE CASCADE,
		FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE,
		FOREIGN KEY (last_read_message_id) REFERENCES messages (id) ON DELETE SET NULL,
		UNIQUE (thread_id, user_id)
	);

CREATE TABLE
	IF NOT EXISTS messages (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		thread_id INTEGER NOT NULL,
		sender_id INTEGER NOT NULL,
		message_type VARCHAR(20) DEFAULT 'text',
		content TEXT,
		metadata JSON,
		created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
		FOREIGN KEY (thread_id) REFERENCES chat_threads (id) ON DELETE CASCADE,
		FOREIGN KEY (sender_id) REFERENCES users (id) ON DELETE CASCADE
	);

CREATE TABLE IF NOT EXISTS users_relationships (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	user_id INTEGER NOT NULL,
	contact_id INTEGER NOT NULL,
	status VARCHAR(20) DEFAULT 'friends',
	created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	responded_at TIMESTAMP,
	FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
	FOREIGN KEY (contact_id) REFERENCES users(id) ON DELETE CASCADE,
	CHECK (user_id != contact_id),
	CHECK (status IN ('pending', 'friends', 'blocked')),
	UNIQUE (user_id, contact_id)
);

CREATE TABLE
	IF NOT EXISTS game_invitations (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		sender_id INTEGER NOT NULL,
		receiver_id INTEGER NOT NULL,
		game_type_id INTEGER NOT NULL,
		status VARCHAR(20) DEFAULT 'pending',
		created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
		FOREIGN KEY (sender_id) REFERENCES users (id) ON DELETE CASCADE,
		FOREIGN KEY (receiver_id) REFERENCES users (id) ON DELETE CASCADE,
		FOREIGN KEY (game_type_id) REFERENCES game_types (id) ON DELETE RESTRICT
	);

CREATE TABLE
	IF NOT EXISTS notifications (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		user_id INTEGER NOT NULL,
		message TEXT NOT NULL,
		notification_type VARCHAR(50),
		metadata JSON,
		is_read BOOLEAN DEFAULT 0,
		created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
		FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE
	);

CREATE TABLE
	IF NOT EXISTS users_one_time_password (
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		user_id INTEGER NOT NULL,
		otp_hash VARCHAR(255) NOT NULL,
		expire_at TIMESTAMP NOT NULL,
		attempts INTEGER DEFAULT 0,
		challenge_id TEXT UNIQUE NOT NULL,
		FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE
);

CREATE INDEX IF NOT EXISTS idx_messages_thread_id ON messages (thread_id);

CREATE INDEX IF NOT EXISTS idx_users_email ON users (email);

CREATE INDEX IF NOT EXISTS idx_users_username ON users (username);

CREATE INDEX IF NOT EXISTS idx_match_participants_match ON match_participants (match_id);

CREATE INDEX IF NOT EXISTS idx_match_participants_user ON match_participants (user_id);

CREATE INDEX IF NOT EXISTS idx_tournaments_status ON tournaments (status);

CREATE INDEX IF NOT EXISTS idx_otp_challenge ON users_one_time_password(challenge_id);

-- users_relationships table indexes
CREATE INDEX IF NOT EXISTS idx_users_relationships_user ON users_relationships(user_id);
CREATE INDEX IF NOT EXISTS idx_users_relationships_contact ON users_relationships(contact_id);
CREATE INDEX IF NOT EXISTS idx_users_relationships_status ON users_relationships(status);
CREATE INDEX IF NOT EXISTS idx_otp_user_id ON users_one_time_password (user_id);
