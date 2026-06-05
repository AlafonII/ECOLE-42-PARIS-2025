import Database from "better-sqlite3";
import bcrypt from "bcrypt";
import path from "path";

//parameters
const DB_PATH = path.join(process.cwd(), "data", "transcendence.db");
const NUM_USERS = 50;
const NUM_MATCHES = 300;
const NUM_TOURNAMENT = 10;
const NUM_FRIENDSHIP = 100;

//random names to use
const firstNames = [
  "Alex",
  "Jordan",
  "Taylor",
  "Morgan",
  "Casey",
  "Riley",
  "Quinn",
  "Avery",
  "Charlie",
  "Dakota",
  "Emery",
  "Finley",
  "Harper",
  "Jamie",
  "Kendall",
  "Logan",
  "Max",
  "Noah",
  "Oakley",
  "Parker",
  "Reese",
  "Sage",
  "Tyler",
  "River",
  "Skyler",
  "Phoenix",
  "Blake",
  "Drew",
  "Ellis",
  "Flynn",
  "Gray",
  "Hayden",
  "Indigo",
  "Jude",
  "Kai",
  "Lane",
  "Milan",
  "Nico",
  "Onyx",
  "Peyton",
  "Raven",
  "Shay",
  "Tatum",
  "Vale",
  "Winter",
  "Zion",
  "Arden",
  "Briar",
  "Cypress",
  "Devon",
];

const lastNames = [
  "Smith",
  "Johnson",
  "Williams",
  "Brown",
  "Jones",
  "Garcia",
  "Miller",
  "Davis",
  "Rodriguez",
  "Martinez",
  "Hernandez",
  "Lopez",
  "Wilson",
  "Anderson",
  "Thomas",
  "Taylor",
  "Moore",
  "Jackson",
  "Martin",
  "Lee",
  "Perez",
  "Thompson",
  "White",
  "Harris",
  "Sanchez",
  "Clark",
  "Ramirez",
  "Lewis",
  "Robinson",
  "Walker",
  "Young",
  "Allen",
  "King",
  "Wright",
  "Scott",
  "Torres",
  "Nguyen",
  "Hill",
  "Flores",
  "Green",
  "Adams",
  "Nelson",
  "Baker",
  "Hall",
  "Rivera",
  "Campbell",
  "Mitchell",
  "Carter",
];

const gamingAdjectives = [
  "Swift",
  "Dark",
  "Silent",
  "Mighty",
  "Cosmic",
  "Shadow",
  "Thunder",
  "Golden",
  "Silver",
  "Iron",
  "Steel",
  "Crystal",
  "Mystic",
  "Ancient",
  "Blazing",
  "Frozen",
  "Electric",
  "Atomic",
  "Cyber",
  "Neo",
  "Ultra",
  "Mega",
  "Super",
  "Hyper",
  "Alpha",
  "Omega",
  "Prime",
  "Elite",
  "Pro",
  "Epic",
  "Legend",
  "Master",
  "Chief",
  "Captain",
];

const gamingNouns = [
  "Wolf",
  "Dragon",
  "Phoenix",
  "Ninja",
  "Knight",
  "Warrior",
  "Hunter",
  "Sniper",
  "Wizard",
  "Mage",
  "Rogue",
  "Assassin",
  "Paladin",
  "Ranger",
  "Titan",
  "Giant",
  "Demon",
  "Angel",
  "Ghost",
  "Spirit",
  "Storm",
  "Flame",
  "Frost",
  "Shadow",
  "Blade",
  "Arrow",
  "Shield",
  "Hammer",
  "Axe",
  "Sword",
  "Dagger",
  "Bow",
  "Staff",
];

const fourLetterNames = [
  // Gaming/Action
  "Rush",
  "Rift",
  "Bliz",
  "Nova",
  "Lynx",
  "Rage",
  "Dash",
  "Bolt",
  "Flux",
  "Apex",
  "Zeal",
  "Fury",
  "Echo",
  "Void",
  "Peak",
  "Core",
  "Neon",
  "Pulse",
  "Wave",
  "Glow",
  "Fang",
  "Claw",
  "Hawk",
  "Wolf",

  // Cool/Style
  "Aces",
  "Arch",
  "Boss",
  "Duke",
  "Epic",
  "Flex",
  "Grit",
  "Hero",
  "Icon",
  "Jazz",
  "King",
  "Luxe",
  "Mars",
  "Onyx",
  "Prox",
  "Raze",
  "Sage",
  "Myth",
  "Vibe",
  "Whiz",
  "Zoom",
  "Blaz",
  "Code",
  "Data",

  // Nature/Elements
  "Aqua",
  "Floe",
  "Hail",
  "Mist",
  "Rain",
  "Snow",
  "Tide",
  "Wind",
  "Dune",
  "Mesa",
  "Peak",
  "Vale",
  "Dawn",
  "Dusk",
  "Moon",
  "Star",

  // Abstract/Tech
  "Byte",
  "Sync",
  "Link",
  "Node",
  "Grid",
  "Mesh",
  "Loop",
  "Scan",
  "Ping",
  "Load",
  "Boot",
  "Disk",
  "Port",
  "Mode",
  "Digi",
  "Mech",

  // Gaming terms
  "Rank",
  "Tank",
  "Mage",
  "Heal",
  "Farm",
  "Gank",
  "Buff",
  "Poke",
  "Push",
  "Solo",
  "Dual",
  "Quad",
  "Team",
  "Clan",
  "Crew",
  "Gang",

  // Cool words
  "Aero",
  "Atom",
  "Axon",
  "Cruz",
  "Daze",
  "Edge",
  "Flux",
  "Hype",
  "Jade",
  "Kaze",
  "Lava",
  "Maze",
  "Nero",
  "Opal",
  "Pyro",
  "Rune",
  "Silk",
  "Tron",
  "Volt",
  "Xion",
  "Yale",
  "Zeta",
  "Zion",
  "Aura",

  // More variety
  "Gale",
  "Jazz",
  "Jinx",
  "Knox",
  "Loki",
  "Mars",
  "Nash",
  "Oryx",
  "Rhys",
  "Sven",
  "Thor",
  "Vega",
  "Wren",
  "Xeno",
  "Zeke",
  "Ajax",
  "Beck",
  "Cade",
  "Dane",
  "Emil",
  "Finn",
  "Grey",
  "Ivan",
  "Jett",
  "Kent",
  "Leon",
  "Milo",
  "Nico",
  "Otto",
  "Paul",
  "Quinn",
  "Reed",
  "Seth",
  "Tate",
  "Ulan",
  "Vito",
  "Wade",
  "Xavi",
  "York",
  "Zane",
];

function randomElement<T>(arr: T[]): T {
  return arr[Math.floor(Math.random() * arr.length)];
}

function randomInt(min: number, max: number): number {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

function generateUsername(): string {
	return fourLetterNames[Math.floor(Math.random() * fourLetterNames.length)];
}

function generateEmail(username: string): string {
  const domains = [
    "gmail.com",
    "outlook.com",
    "yahoo.com",
    "proton.me",
    "icloud.com",
    "42.fr",
  ];
  return `${username.toLowerCase().replace(/[^a-z0-9]/g, "")}@${randomElement(domains)}`;
}

function hashPassword(password: string): string {
  return bcrypt.hashSync(password, 10);
}

function randomDate(startDays: number, endDays: number): string {
  const now = Date.now();
  const start = now - startDays * 24 * 60 * 60 * 1000;
  const end = now - endDays * 24 * 60 * 60 * 1000;
  const date = new Date(randomInt(end, start));
  return date.toISOString().replace("T", " ").substring(0, 19);
}

async function seed() {
  //console.log("Starting DB seed...");

  const db = new Database(DB_PATH);
  db.pragma("foreign_keys = ON");

  const tableCheck = db
    .prepare(
      "SELECT name FROM sqlite_master WHERE type='table' AND name='users'",
    )
    .get();
  if (!tableCheck) {
    console.error("Error: table 'users' do not exist.");
    process.exit(1);
  }

  //cleaning database
  //console.log("Clearing existing data...");
  db.exec(`
		DELETE FROM match_participants;
		DELETE FROM matches;
		DELETE FROM tournament_participants;
		DELETE FROM tournaments;
		DELETE FROM users_relationships;
		DELETE FROM messages;
		DELETE FROM chat_thread_members;
		DELETE FROM chat_threads;
		DELETE FROM notifications;
		DELETE FROM game_invitations;
		DELETE FROM USERS;
  `);

  //console.log("Creating game types...");
  const insertGameType = db.prepare(`
		INSERT OR IGNORE INTO game_types (name, min_players, max_players)
		VALUES (?, ?, ?)
	`);

  insertGameType.run("pong", 2, 2);

  const gameTypes = db.prepare("SELECT id, name FROM game_types").all() as {
    id: number;
    name: string;
  }[];
  //console.log(`${gameTypes.length} game types ready`);

  //console.log(`Creating ${NUM_USERS} users...`);
  const insertUser = db.prepare(`
		INSERT INTO users (username, email, password_hash, display_name, avatar_url, two_factor, created_at)
		VALUES (?, ?, ?, ?, ?, ?, ?)
		`);

  const usedUsernames = new Set<string>();
  const usedDisplayNames = new Set<string>();
  const usedMails = new Set<string>();
  const userIds: number[] = [];
  const passwordHash = hashPassword("password123");

  //adding users
  const insertUsers = db.transaction(() => {
    for (let i = 0; i < NUM_USERS; i++) {
      let username: string;
      let displayName: string;
      let email: string;

      do {
        username = generateUsername();
      } while (usedUsernames.has(username.toLowerCase()));
      usedUsernames.add(username.toLowerCase());

        displayName = username;
      	usedDisplayNames.add(displayName.toLowerCase());

      do {
        email = generateEmail(username + randomInt(1, 9999));
      } while (usedMails.has(email.toLowerCase()));
      usedMails.add(email.toLowerCase());

      const avatarUrl =
        Math.random() > 0.3
          ? `https://api.dicebear.com/7.x/avataaars/svg?seed=${username}`
          : null;
      const twoFactor = Math.random() > 0.8 ? 1 : 0;
      const createdAt = randomDate(180, 1);

      const result = insertUser.run(
        username,
        email,
        passwordHash,
        displayName,
        avatarUrl,
        twoFactor,
        createdAt,
      );
      userIds.push(result.lastInsertRowid as number);
    }
  });
  insertUsers();
  //console.log(`${userIds.length}+ users created.`);

  //console.log(`Creating ${NUM_MATCHES} matches...`);
  const insertMatch = db.prepare(`
		INSERT INTO matches (game_type_id, winner_id, duration_seconds, created_at)
		
		VALUES (?, ?, ?, ?)
	`);
  const insertMatchParticipant = db.prepare(`
		INSERT INTO match_participants (match_id, user_id, is_ai, score)
		VALUES (?, ?, ?, ?)
	`);

  //adding matches
  const matchIds: number[] = [];
  const insertMatches = db.transaction(() => {
    for (let i = 0; i < NUM_MATCHES; i++) {
      const gameType = gameTypes.find((g) => g.name === "pong")!;

      const player1Id = randomElement(userIds);
      let player2Id: number;
      do {
        player2Id = randomElement(userIds);
      } while (player2Id === player1Id);

      const maxScore = randomElement([5]);
      const winnerScore = maxScore;
      const loserScore = randomInt(0, maxScore - 1);

      const player1Wins = Math.random() > 0.5;
      const winnerId = player1Wins ? player1Id : player2Id;
      const loserId = player1Wins ? player2Id : player1Id;
      const score1 = player1Wins ? winnerScore : loserScore;
      const score2 = player1Wins ? loserScore : winnerScore;

      const totalPoints = score1 + score2;
      const duration = totalPoints * randomInt(30, 60);
      const createdAt = randomDate(90, 0);

      const matchResult = insertMatch.run(
        gameType.id,
        winnerId,
        duration,
        createdAt,
      );
      const matchId = matchResult.lastInsertRowid as number;
      matchIds.push(matchId);

      insertMatchParticipant.run(matchId, player1Id, 0, score1);
      insertMatchParticipant.run(matchId, player2Id, 0, score2);

      if (Math.random() > 0.9) {
        const aiMatchresult = insertMatch.run(
          gameType.id,
          player1Id,
          randomInt(120, 300),
          createdAt,
        );
        const aiMatchId = aiMatchresult.lastInsertRowid as number;
        insertMatchParticipant.run(aiMatchId, player1Id, 0, maxScore);
        insertMatchParticipant.run(
          aiMatchId,
          null,
          1,
          randomInt(0, maxScore - 1),
        );
      }
    }
  });
  insertMatches();
  //console.log(`${matchIds.length}+ matches created.`);

  //console.log(`Creating ${NUM_TOURNAMENT} tournaments...`);
  const insertTournament = db.prepare(`
		INSERT INTO tournaments (name, game_type_id, status, max_participants, winner_id, tournament_organizer_id, created_at, started_at, finished_at)
		VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
	`);

  const insertTournamentParticipant = db.prepare(`
		INSERT INTO tournament_participants (tournament_id, user_id, alias, final_position, created_at)
		VALUES (?, ?, ?, ?, ?)
	`);

  const updateWinner = db.prepare(
    `UPDATE tournaments SET winner_id = ? WHERE id = ?`,
  );

  const tournamentNames = [
    "Spring Championship",
    "Summer Showdown",
    "Fall Finals",
    "Winter Warfare",
    "42 Masters",
    "Pong Pro League",
    "Campus Cup",
    "Midnight Madness",
    "Weekend Warriors",
    "Grand Slam",
    "Ultimate Challenge",
    "Elite Series",
  ];

  //adding tournaments
  const insertTournaments = db.transaction(() => {
    for (let i = 0; i < NUM_TOURNAMENT; i++) {
      const tournamentOrganizerId = randomElement(userIds);
      const gameType = gameTypes.find((g) => g.name === "pong")!;
      const name = `${tournamentNames[i % tournamentNames.length]}`;
      const maxParticipants = randomElement([4, 8, 16]);

      const statusRoll = Math.random();
      let status: string;
      let winnerId: number | null = null;
      let startedAt: string | null = null;
      let finishedAt: string | null = null;
      const createdAt = randomDate(40, 15);

      if (statusRoll < 0.2) {
        status = "registration";
      } else if (statusRoll < 0.4) {
        status = "in_progress";
        startedAt = randomDate(5, 0);
      } else {
        status = "finished";
        startedAt = randomDate(14, 10);
        finishedAt = randomDate(9, 1);
      }

      const tournamentResult = insertTournament.run(
        name,
        gameType.id,
        status,
        maxParticipants,
        winnerId,
        tournamentOrganizerId,
        createdAt,
        startedAt,
        finishedAt,
      );
      const tournamentId = tournamentResult.lastInsertRowid as number;

      const numParticipants =
        status === "registration"
          ? randomInt(1, maxParticipants - 1)
          : maxParticipants;

      const shuffledUsers = [...userIds].sort(() => Math.random() - 0.5);
      const participants = shuffledUsers.slice(0, numParticipants);

      participants.forEach((userId, index) => {
        const alias = `Player_${randomElement(gamingAdjectives)}`;
        const finalPosition = status === "finished" ? index + 1 : null;
		if (finalPosition === 1)
			winnerId = userId;
        insertTournamentParticipant.run(
          tournamentId,
          userId,
          alias,
          finalPosition,
          createdAt,
        );
      });
	  if (status === "finished")
		updateWinner.run(winnerId, tournamentId);
    }
  });
  insertTournaments();
  //console.log(`${NUM_TOURNAMENT} tournaments created`);

  //console.log(`Creating ${NUM_FRIENDSHIP} friendships...`);

  const insertRelationship = db.prepare(`
		INSERT OR IGNORE INTO users_relationships (user_id, contact_id, status, created_at)
		VALUES (?, ?, ?, ?)
	`);

  //adding friendships
  const insertFriendships = db.transaction(() => {
    const existingPairs = new Set<string>();
    let created = 0;

    for (let i = 0; i < NUM_FRIENDSHIP * 2 && created < NUM_FRIENDSHIP; i++) {
      const user1 = randomElement(userIds);
      let user2: number;
      do {
        user2 = randomElement(userIds);
      } while (user2 === user1);

      const [smallerId, biggerId] = user1 < user2 ? [user1, user2] : [user2, user1];
	  const pairKey = `${smallerId}-${biggerId}`;

      if (existingPairs.has(pairKey)) continue;
      existingPairs.add(pairKey);

      const status = Math.random() > 0.1 ? "friends" : "blocked";
      const createdAt = randomDate(90, 0);

      insertRelationship.run(smallerId, biggerId, status, createdAt);
      created++;
    }
  });
  insertFriendships();
  //console.log(`${NUM_FRIENDSHIP}+ friendships created`);

  //console.log("SEED COMPLETE!");

  const stats = {
    users: db.prepare("SELECT COUNT(*) as count FROM users").get() as {
      count: number;
    },
    matches: db.prepare("SELECT COUNT(*) as count FROM matches").get() as {
      count: number;
    },
    tournaments: db
      .prepare("SELECT COUNT(*) as count FROM tournaments")
      .get() as { count: number },
    friendships: db
      .prepare("SELECT COUNT(*) as count FROM users_relationships")
      .get() as { count: number },
  };

  //console.log("Database stats");
  //console.log(`Users:		${stats.users.count}`);
  //console.log(`Matches:	${stats.matches.count}`);
  //console.log(`Tournaments:	${stats.tournaments.count}`);
  //console.log(`Friendships:	${stats.friendships.count}`);

  // show 10 users with at least 1 friend, 1 game, 1 tounament
  const testUsers = db
    .prepare(
      `
		SELECT DISTINCT u.id, u.username, u.email
		FROM users u
		WHERE EXISTS (
			SELECT 1 FROM users_relationships ur
			WHERE ur.user_id = u.id AND ur.status = 'friends'
		)
		AND EXISTS (
			SELECT 1 FROM match_participants mp WHERE mp.user_id = u.id)
		AND EXISTS (
			SELECT 1 FROM tournament_participants tp WHERE tp.user_id = u.id)		
		LIMIT 10	
	`,
    )
    .all() as { id: number; username: string; email: string }[];

  //console.log("\n==============================");
  //console.log("TEST USERS || pass: password123");
  //console.log("\n==============================");
  testUsers.forEach((user, i) => {
    //console.log(`${i + 1}. ${user.username} | ${user.email}`);
  });
  //console.log("\n==============================");

  db.close();
}
seed().catch(console.error);
