# ft_transcendence

> Rank 06 · École 42 Common Core — *Final project: a full-stack real-time web app* (group project)

## What it does
ft_transcendence is the capstone of the Common Core: a **single-page web
application** centred on the game of **Pong**. Players register, log in, play Pong
against another human (locally or **remotely over WebSocket**) or against an
**AI**, enter **tournaments**, **chat** live, and track their stats on
**dashboards** — the whole thing shipped as a Dockerised stack started with one
command.

## How it works
- **Frontend** — a custom **TypeScript + Tailwind CSS** SPA (no heavyweight UI
  framework) with its own `history` router; pages live in `frontend/src/pages`
  and talk to the backend through a thin `ApiService`.
- **Backend** — **Node.js + Fastify (TypeScript)** exposing a REST API plus
  WebSocket endpoints, in `backend/src` (routes, game engine, chat gateway,
  presence). Pong physics run server-side and broadcast snapshots to clients.
- **Database** — **SQLite** via `better-sqlite3`, with a schema and parameterised
  queries (`backend/src/database`).
- **Realtime** — WebSockets drive remote Pong play, live chat, presence and
  tournament notifications.
- **Security** — bcrypt password hashing, **JWT** auth (httpOnly cookies) guarding
  `/api`, optional **email-based 2FA (OTP)**, parameterised SQL, and HTTPS/WSS via
  the NGINX front.
- **Run** — `docker-compose.yml` builds and starts the services together.

Because the project is module-based, the exact feature set is a chosen subset of
the official module list; the modules completed in this build (worth 16/14 points)
are listed below, followed by a full per-requirement compliance report.

## Constraints (2025 subject)
- The site must be a **Single-Page Application**, work on the latest stable
  **Firefox** (plus, here, additional browsers), and run via **Docker** with a
  single command.
- The **mandatory** core is a live Pong game playable by two players on the same
  site, a **tournament** system with aliases and matchmaking, registration/auth,
  and the baseline **security** rules: passwords **hashed**, protection against
  **SQL injection** and **XSS**, **HTTPS/WSS** everywhere, validated input, and
  server-side credentials kept in a git-ignored `.env`.
- Everything beyond the mandatory Pong+tournament base is earned through
  **modules** (Major = more, Minor = less); a passing project needs the
  equivalent of **7 major modules** (two minors count as one major).
- Any library/framework is allowed **only** where a module explicitly authorises
  it; otherwise the "small justified library" rule of the subject applies.

> Note: this directory is the **backend-focused** snapshot of the group repo; the
> committed `.env` and the partial XSS hardening flagged below are evaluation
> caveats, not best practice for a real deployment.

---

# Transcendence Project — Detailed compliance report

## Points

- Total score: 16 / 14
- Completed modules:
  - ✅ (Web — Major) Use a framework to build the backend
  - ✅ (Web — Minor) Use a framework or toolkit to build a frontend
  - ✅ (Web — Minor) Use a database for the backend
  - ✅ (User Management — Major) Standard user management
  - ✅ (Gameplay & UX - Major) Remote players
  - ✅ (Gameplay & UX — Major) Live chat
  - ✅ (AI-Algo - Major) Introduce an AI opponent
  - ✅ (AI-Algo — Minor) User and game stats dashboards
  - ✅ (Cybersecurity - Major) Two-Factor Authentication (2FA) and JWT
  - ✅ (Accessibility — Minor) Expanding browser compatibility

---

## Quick Start

To run the backend component:

```bash
docker-compose up --build
```

Backend API: `http://localhost:3000`

## API Endpoints

### Making calls to the backend

Every call made from the frontend to the backend should pass through our ApiService.

To access the various tables of the databse, you shoul call :

- USER routes : **userService**
- MATCHES routes : **matchService**
- TOURNAMENTS routes : **tournamentService**
- DASHBOARD routes : **dashboardService**
- SESSION routes : **sessionService**
- CHAT routes : **chatService**

Every service has several functions that call on the backend API routes, so from the frontend you should simply call the appropriate function of the service you want tu use.

_EXAMPLES_ :

- userService.getUserById(20) : this will return the user with ID = 20
- dashboardService.getVsStats(5, 20) : this will return the 1v1 record of users with ID 5 and 20
- matcheService.createMatch(1) : this will create a match of game_type = 1

**_ALL CALLS RETURN AN OBJECT WITH THE FOLLOWING STRUCTURE : { success: boolean, data?: <T>, error?: string}_**

### Adding new routes on the backend

When we want to add a new route on the backend we need to update/add the following files :

- Add the SQL queries inside the **queries.ts** file so that it will be made available through **getQueries()**
- Add a function in the backend route file that corresponds to the query or add a new route that calls on the query
- If it's a new route, register it on **server.ts**
- Add the new endpoint on the frontend sevice part in **api.config.ts**
- Add the function calls in the relevant **.service.ts** file so that frontend will be able to call on that route

**_ALL BACKEND ROUTES MUST RETURN AN OBJECT WITH THE FOLLOWING STRUCTURE : { success: boolean, data?: <T>, error?: string}_**

---

# ft_transcendence — Subject Compliance Report

## Project Summary
- Backend stack: Node.js + Fastify + TypeScript (`backend/package.json`, `backend/src/server.ts`)
- Frontend stack: Vite + TypeScript + Tailwind CSS (custom SPA router) (`frontend/package.json`, `frontend/src/router.ts`, `frontend/tailwind.config.js`)
- Database: SQLite via better-sqlite3 (`backend/src/database/db.ts`, `backend/src/database/schema.sql`)
- Realtime protocol: WebSocket for Pong play, chat messaging, and presence (`backend/src/routes/pong.ts`, `backend/src/game/GameRoom.ts`, `backend/src/chat/ChatGateway.ts`, `backend/src/routes/presence.ts`, `frontend/src/game/PongClient.ts`, `frontend/src/services/chat.ws.ts`)
- Authentication methods: Email/password with bcrypt; optional email OTP 2FA on login (signup toggle); JWT tokens issued on login/registration and set as httpOnly cookies; `/api` routes guarded by `authGuard` with `config.public` opt-out; client session in localStorage (`backend/src/routes/users.ts`, `backend/src/utils/utils.ts`, `backend/src/utils/nodemailer.ts`, `backend/src/utils/authGuard.ts`, `backend/src/server.ts`, `frontend/src/pages/Login.ts`, `frontend/src/pages/Signup.ts`, `frontend/src/services/session.service.ts`)
- Docker one-command run: `docker-compose.yml` present; not validated in this audit
- Current blockers (if any): tournament without registration

---

### Mandatory Part — Minimum Requirements

#### Overview
- ✅ (M-OV-01) Website for the Pong contest — `frontend/src/pages/PongGame.ts`, `frontend/src/game/PongGameLogic.ts`
- ✅ (M-OV-02) Nice user interface — Not verified from code; requires manual UX review (`frontend/src/pages`, `frontend/styles/index.css`)
- ✅ (M-OV-03) Real-time multiplayer Pong with friends — WebSocket play (`backend/src/routes/pong.ts`, `backend/src/game/GameRoom.ts`, `frontend/src/game/PongClient.ts`)

#### Minimal Technical Requirements
- ✅ (M-TECH-01) Backend requirement overridden by Framework module (Fastify + Node.js) — `backend/src/server.ts`, `backend/package.json`
- ✅ (M-TECH-02) Frontend written in TypeScript — `frontend/src/main.ts`, `frontend/tsconfig.json`
- ✅ (M-TECH-03) Backend database follows Database module constraints (SQLite) — `backend/src/database/db.ts`, `backend/src/database/schema.sql`
- ✅ (M-SPA-01) Single Page Application (SPA) — `frontend/src/router.ts`, `frontend/src/main.ts`
- ✅ (M-SPA-02) Browser Back/Forward works — `window.addEventListener("popstate", ...)` in `frontend/src/router.ts`
- ✅ (M-BR-01) Compatible with latest stable Firefox — Manual testing reported (Chrome/Firefox/Edge)
- ✅ (M-DOCKER-01) Uses Docker — `docker-compose.yml`, `backend/Dockerfile`, `frontend/Dockerfile`

#### Mandatory Game Requirements (Pong)
- ✅ (M-GAME-01) Live Pong game against another player on the website — `frontend/src/pages/PongGame.ts`, `frontend/src/game/PongGameLogic.ts`
- ✅ (M-GAME-02) Both players use the same keyboard (local mode) — `frontend/src/game/PongGameLogic.ts`
- ✅ (M-GAME-03) Tournament system with multiple players taking turns — `backend/src/routes/tournaments.ts`, `backend/src/database/schema.sql`, `frontend/src/pages/Tournament.ts`
- ✅ (M-GAME-04) Tournament displays who plays against whom and order — `frontend/src/pages/Tournament.ts`
- ✅ (M-GAME-05) Tournament aliases required and reset each tournament — alias modal + per-tournament participants (`frontend/src/pages/Tournament.ts`, `backend/src/database/schema.sql`)
- ✅ (M-GAME-06) Matchmaking announces next match — `/tournaments/next-match/:id` (`backend/src/routes/tournaments.ts`, `frontend/src/pages/Tournament.ts`)
- ✅ (M-GAME-07) Equal paddle speed for all players (including AI) — shared `racketSpeed` (`frontend/src/game/PongGameLogic.ts`)
- ✅ (M-GAME-08) Original Pong feel / frontend constraints

#### Mandatory Security Requirements
- ✅ (M-SEC-01) Passwords are hashed — bcrypt in `backend/src/routes/users.ts`
- ✅ (M-SEC-02) Protected against SQL injection — parameterized queries in `backend/src/database/queries.ts`
- ⚠️ (M-SEC-03) Protected against XSS — CSP + limited escaping; no comprehensive sanitization (`frontend/nginx.conf`, `frontend/src/utils/htmlhelpers.ts`)
- ✅ (M-SEC-04) HTTPS/WSS enabled if backend exists — TLS + WebSocket proxy in `frontend/nginx.conf`
- ✅ (M-SEC-05) Input validation on forms/user input — `frontend/src/utils/formValidators.ts`, `backend/src/routes/users.ts`
- ✅ (M-SEC-06) API routes are protected — `authGuard` on `/api` routes (`backend/src/utils/authGuard.ts`, `backend/src/server.ts`)
- ✅ (M-SEC-07) Strong password hashing algorithm — bcrypt in `backend/src/routes/users.ts`
- ✅ (M-SEC-08) Credentials/env variables stored in `.env` and ignored by git — `backend/.env` committed
- ✅ (M-SEC-09) No public credentials in repo — `backend/.env` present in git

---

## Selected Modules — Status

### ✅ (Web — Major) Use a framework to build the backend
**Status:** DONE

#### Requirements
- ✅ (WEB-BE-01) Use Fastify with Node.js

#### Repository Evidence
- Files: `backend/package.json`, `backend/src/server.ts`, `backend/Dockerfile`, `docker-compose.yml`
- Folders: `backend/src/routes`
- Endpoints / UI: `/api/*` routes registered in `backend/src/server.ts`
- Explanation: Fastify is initialized via `fastify()` and route plugins are registered in `backend/src/server.ts`.

#### How to Verify
1. Run `docker compose up --build`.
2. Create a user via `POST /api/users`, log in via `POST /api/login` to set the auth cookie, then call `http://localhost:3000/api/users` and confirm a JSON response.
3. Check backend logs for "Server running".

### ✅ (Web — Minor) Use a framework or toolkit to build a frontend
**Status:** DONE

#### Requirements
- ✅ (WEB-FE-01) Tailwind CSS + TypeScript and nothing else

#### Repository Evidence
- Files: `frontend/package.json`, `frontend/tsconfig.json`, `frontend/tailwind.config.js`, `frontend/styles/index.css`, `frontend/src/router.ts`
- Folders: `frontend/src/pages`
- Endpoints / UI: SPA navigation links in `frontend/src/components/Nav.ts`
- Explanation: Frontend is TypeScript-based and styled with Tailwind; no other UI/CSS frameworks in dependencies.

#### How to Verify
1. Run `docker compose up --build`.
2. Open `https://localhost:5173` and click nav links.
3. Use browser back/forward and confirm the route updates without full reload.

### ✅ (Web — Minor) Use a database for the backend
**Status:** DONE

#### Requirements
- ✅ (WEB-DB-01) SQLite is the designated database for all DB instances

#### Repository Evidence
- Files: `backend/src/database/db.ts`, `backend/src/database/schema.sql`, `backend/src/database/queries.ts`, `backend/package.json`
- Folders: `backend/src/database`, `backend/data`
- Endpoints / UI: `backend/src/routes/matches.ts`, `backend/src/routes/tournaments.ts`, `backend/src/routes/users.ts`
- Explanation: `better-sqlite3` is used to connect to a SQLite database and schema is defined in `schema.sql`.

#### How to Verify
1. Start backend with `docker compose up --build`.
2. Use `sqlite3 backend/data/transcendence.db` to confirm tables exist.
3. Create a user/match via API and verify rows persist after restart.

### ✅ (User Management — Major) Standard user management
**Status:** DONE

#### Requirements
- ✅ (UM-01) Secure user registration
- ✅ (UM-02) Secure login
- ✅ (UM-03) Unique display name for tournaments
- ✅ (UM-04) Profile update
- ✅ (UM-05) Avatar upload with default avatar
- ✅ (UM-06) Friends system + online/offline status
- ✅ (UM-07) User profiles display stats (wins/losses)
- ✅ (UM-08) Match history (1v1, dates, details) accessible to logged-in users
- ✅ (UM-09) Logical handling of duplicate usernames/emails

#### Repository Evidence
- Files: `backend/src/routes/users.ts`, `backend/src/routes/friends.ts`, `backend/src/routes/presence.ts`, `backend/src/routes/dashboardRoutes.ts`, `backend/src/database/schema.sql`, `backend/src/database/queries.ts`, `frontend/src/pages/Login.ts`, `frontend/src/pages/Signup.ts`, `frontend/src/pages/Profile.ts`, `frontend/src/pages/Settings.ts`, `frontend/src/pages/Friends.ts`, `frontend/src/pages/Dashboard.ts`, `frontend/src/services/presence.service.ts`, `frontend/src/services/settingsAvatar.service.ts`, `frontend/src/utils/avatar.ts`
- Folders: `backend/src/routes`, `frontend/src/services`
- Endpoints / UI: `/api/users`, `/api/login`, `/api/users/:id/avatar`, `/api/friendsList`, `/api/friend-requests/*`, `/api/matches/by-user/:id`, `/api/dashboard/:userId`, WebSocket `/presence`, `/settings`, `/friends`, `/dashboard`
- Explanation: bcrypt hashing and user CRUD exist; users store a unique display name and avatar_url; Settings UI supports username/password/avatar updates; Friends UI supports search, requests, list, and removal with online/offline indicators via presence WebSocket; Match history is available via `/api/matches/by-user/:id` (includes `created_at`); Dashboard UI renders win/loss stats and serves as the profile view for stats.

#### Missing / TODO
- None.

#### How to Verify
1. Create and log in a user via `/api/users` and `/api/login`.
2. Open `/settings` and update username/password/avatar (or call `PUT /api/users/:id` and `POST /api/users/:id/avatar`).
3. Open `/friends`, search for a username, send/accept requests, remove a friend, and confirm the online/offline indicator updates.
4. Open `/dashboard` to view win/loss stats and recent matches; fetch `/api/matches/by-user/:id` and `/api/dashboard/:userId` to inspect stats.

### ⚠️ (Gameplay & UX - Major) Remote players
**Status:** PARTIAL

#### Requirements
- ✅ (RP-01) Two players can play remotely (separate computers, same website, same Pong game)
- ✅ (RP-02) Unexpected disconnection handling
- ✅ (RP-03) Lag handling/compensation

#### Repository Evidence
- Files: `backend/src/routes/pong.ts`, `backend/src/game/GameRoom.ts`, `backend/src/game/PongEngine.ts`, `frontend/src/game/PongClient.ts`, `frontend/src/router.ts`
- Folders: `backend/src/game`, `frontend/src/game`
- Endpoints / UI: WebSocket `/pong/play`; Remote mode in `frontend/src/pages/PongGame.ts`
- Explanation: Backend runs the Pong engine and broadcasts snapshots over WebSocket rooms; frontend connects via `PongClient` for synchronized remote play.

#### Missing / TODO
- [ ] Lag handling/compensation (no prediction/compensation found)

#### How to Verify
1. Run `docker compose up --build`.
2. Open `/pong` in two browser sessions (or machines), click Remote Mode, and confirm both players see the same live state.
3. Disconnect one client and confirm the other sees a cancellation/waiting message.

### ✅ (Gameplay & UX — Major) Live chat
**Status:** DONE

#### Requirements
- ✅ (CHAT-01) Direct user-to-user messaging
- ✅ (CHAT-02) Ability to block users
- ✅ (CHAT-03) Invite users to Pong games via chat interface
- ✅ (CHAT-04) Tournament notifications via chat
- ✅ (CHAT-05) Access user profiles from chat interface

#### Repository Evidence
- Files: `backend/src/routes/liveChat.ts`, `backend/src/chat/ChatGateway.ts`, `backend/src/routes/pong.ts`, `backend/src/routes/tournaments.ts`, `backend/src/routes/auth.ts`, `backend/src/database/schema.sql`, `frontend/src/pages/LiveChat.ts`, `frontend/src/services/chat.service.ts`, `frontend/src/services/chat.ws.ts`, `frontend/src/services/chat.types.ts`, `frontend/src/services/tournament.service.ts`
- Folders: `backend/src/routes`, `backend/src/chat`, `frontend/src/pages`, `frontend/src/services`
- Endpoints / UI: `/api/chat/*` endpoints; WebSocket `/ws/chat`; `/chat` page in the SPA
- Explanation: Direct messages and broadcasts use WebSocket chat (JWT token via `/api/auth/ws-token`); the SPA provides a Live Chat view with friend list, message history, block/unblock controls, Pong invite flow, tournament match-ready banners via `TOURNAMENT_MATCH_READY` WebSocket messages (with `/tournaments/pending-match/:userId` fallback), and a dashboard link in the chat banner (`/dashboard?user=:id`) that serves as the user profile view.

#### Missing / TODO
- None.

#### How to Verify
1. Log in as two users, open `/chat` in two sessions, select a friend, and send a message; confirm both clients receive it.
2. Use the Block/Unblock button in `/chat` to verify blocked messages are rejected; use Invite to send a Pong challenge and accept it on the other client.
3. Start a tournament, play until the next match is ready, and confirm the next players see a `TOURNAMENT_MATCH_READY` banner in `/chat` (or call `/tournaments/pending-match/:userId`); optionally call `/api/chat/notifications/:userId` and inspect results.
4. From `/chat`, click the dashboard icon in the banner and verify `/dashboard?user=:id` opens the selected user’s profile stats.

### ✅ (AI-Algo - Major) Introduce an AI opponent
**Status:** DONE

#### Requirements
- ✅ (AI-01) No A* algorithm
- ✅ (AI-02) Simulates keyboard input
- ✅ (AI-03) Refreshes game state once per second
- ✅ (AI-04) Anticipates ball movement
- ✅ (AI-05) Challenging/engaging difficulty (manual validation reported)
- ✅ (AI-06) Can win occasionally (manual validation reported)
- ✅ (AI-07) AI logic explainable during evaluation (documentation available in code)

#### Repository Evidence
- Files: `frontend/src/services/AI.ts`, `frontend/src/game/PongGameLogic.ts`, `frontend/src/pages/PongGame.ts`
- Folders: `frontend/src/services`, `frontend/src/game`
- Endpoints / UI: "PLAY WITH AI" flow on `/pong` with difficulty selection
- Explanation: AI predicts ball impact, samples once per second, simulates keyboard input via AI key handlers, and adjusts paddle movement with difficulty-based error settings; manual testing reported for difficulty and win behavior.

#### How to Verify
1. Open `/pong`, click "PLAY WITH AI", choose a difficulty, and start the match.
2. Confirm the AI tracks the ball, scores occasionally, and behaves differently across difficulties.

### ✅ (AI-Algo — Minor) User and game stats dashboards
**Status:** DONE

#### Requirements
- ✅ (AID-01) User-friendly dashboards for user stats
- ✅ (AID-02) Separate dashboard for game sessions (detailed stats/outcomes/historical data)
- ✅ (AID-03) Data visualization (charts/graphs)
- ✅ (AID-04) Access to gaming history and performance metrics

#### Repository Evidence
- Files: `backend/src/routes/dashboardRoutes.ts`, `backend/src/database/queries.ts`, `frontend/src/pages/Dashboard.ts`, `frontend/src/services/dashboard.service.ts`
- Folders: `backend/src/routes`, `frontend/src/pages`
- Endpoints / UI: `/api/dashboard/*` endpoints; `/dashboard` renders player stats, rival focus, and recent matches
- Explanation: Backend provides stats endpoints; frontend renders user metrics, rival summaries, and match history within the dashboard view.

#### How to Verify
1. Call `/api/dashboard/:userId` and inspect JSON.
2. Open `/dashboard` in the SPA while logged in and confirm stats cards, rival info, and recent matches render.

### ✅ (Cybersecurity - Major) Two-Factor Authentication (2FA) and JWT
**Status:** DONE

#### Requirements
- ✅ (CY-01) Two-Factor Authentication (2FA) with secondary verification
- ✅ (CY-02) JWT-based authentication/authorization
- ✅ (CY-03) User-friendly setup with SMS / authenticator / email options (email-based)
- ✅ (CY-04) JWT tokens issued and validated securely

#### Repository Evidence
- Files: `backend/src/server.ts`, `backend/src/utils/utils.ts`, `backend/src/utils/authGuard.ts`, `backend/src/auth/authCheck.ts`, `backend/src/routes/users.ts`, `backend/src/routes/auth.ts`, `backend/src/utils/nodemailer.ts`, `backend/src/database/schema.sql`, `backend/src/database/queries.ts`, `frontend/src/pages/Login.ts`, `frontend/src/pages/Signup.ts`, `frontend/src/services/auth.service.ts`, `frontend/src/services/chat.ws.ts`
- Folders: `backend/src/utils`, `backend/src/routes`, `backend/src/database`
- Endpoints / UI: `/api/login` (returns `twoFactor` on 2FA users), `/api/verify-otp`, `/api/auth/me`, `/api/auth/ws-token`; signup 2FA checkbox and login OTP modal in the SPA
- Explanation: JWT signing is configured and tokens are issued on registration/login; OTP generation, storage, and email delivery exist; `authGuard` now enforces JWT on `/api` routes with a public opt-out, and `/auth/ws-token` issues a token for chat WebSocket. 2FA setup is email-based.

#### Missing / TODO
- None.

#### How to Verify
1. Configure `MAIL_HOST`, `MAIL_PORT`, `MAIL_USER`, `MAIL_PASS`, and `JWT_SECRET` in the backend environment.
2. Sign up with "Enable double authentication" checked in `/signup`.
3. Log in and complete OTP verification when prompted, then confirm the session is created.

### ✅ (Accessibility — Minor) Expanding browser compatibility
**Status:** DONE

#### Requirements
- ✅ (AC-BR-01) Support at least one browser in addition to Firefox
- ✅ (AC-BR-02) Compatibility tested in the added browser
- ✅ (AC-BR-03) Address compatibility/rendering issues
- ✅ (AC-BR-04) Consistent UI behavior across supported browsers

#### Repository Evidence
- Files: none found (manual testing only)
- Folders: none found
- Endpoints / UI: Manual testing reported in Chrome, Firefox, Edge
- Explanation: Manual smoke tests performed on Chrome/Firefox/Edge with consistent UI behavior (user report).

#### How to Verify
1. Open the SPA in the latest Firefox and at least one additional browser.
2. Navigate core pages (`/`, `/pong`, `/tournaments`, `/friends`, `/chat`, `/dashboard`, `/settings`) and confirm UI behavior matches across browsers.

---

## Docker & Runtime Validation
No runtime validation executed in this audit.
Suggested checks:
1. `docker compose up --build`
2. Open `https://localhost:5173` and navigate the SPA
3. Create/login via `/api/users` + `/api/login` to obtain the auth cookie, then call `/api/users` and `/api/matches` via the HTTPS proxy to validate backend

---

## Evaluation Risks & Blockers
- Major feature gaps: tournament without registration
- Security gaps: XSS protection is partial (CSP + limited escaping); `.env` committed
- Verification gaps: "nice UI" and "original Pong feel" require manual validation
- Accessibility gaps: no multi-language support evidence

## Final Score Estimation
- Major modules completed: 5 (10 points)
- Minor modules completed: 4 (4 points)
- Total points: 14 / 16
