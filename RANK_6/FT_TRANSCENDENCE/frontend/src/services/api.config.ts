// Which base URL to use for the API
function resolveApiBaseUrl(): string {
	// Prefer a URL from the Vite environment (e.g. VITE_API_BASE_URL in .env)
	const envBase = import.meta.env.VITE_API_BASE_URL;
	if (envBase) return envBase.replace(/\/$/, "");

	// If running in a browser, inspect the current page URL
	if (typeof window !== "undefined") {
		const { hostname, protocol, port, origin } = window.location;
		const isLocalhost =
			hostname === "localhost" ||
			hostname === "127.0.0.1" ||
			hostname === "0.0.0.0";

		// Always use same-origin /api to ensure cookies work{"success":false,"error":"No authentication"}
		return "/api";
	}

	return "/api";
}

//This is the main pipeline through which all API calls pass through. To call a route, we will simply call the relevant part of our API_CONFIG object
export const API_CONFIG = {
	BASE_URL: resolveApiBaseUrl(),
	TIMEOUT: 10000,
	ENDPOINTS: {
		USERS: {
			ALL: "/users",
			BY_ID: (id: number) => `/users/by-id/${id}`,
			BY_EMAIL: (email: string) =>
				`/users/by-email/${encodeURIComponent(email)}`,
			BY_USERNAME: (username: string) =>
				`/users/by-username/${encodeURIComponent(username)}`,
			IS_USERNAME_FREE: (username: string) =>
				`/users/is-username-free/${encodeURIComponent(username)}`,
			UPDATE: (id: number) => `/users/${id}`,
			DELETE: (id: number) => `/users/${id}`,
			LOGIN: "/login",
			AVATAR: (id: number) => `/users/${id}/avatar`,
		},

		MATCHES: {
			ALL: "/matches",
			BY_ID: (id: number) => `/matches/${id}`,
			BY_USER: (id: number) => `/matches/${id}`,
			FINISHED: "/matches/finished",
			UNPLAYED: "/matches/unplayed",
			CREATE: "/matches/new-match",
			ADD_PARTICIPANT: "/matches/add-participant",
			UPDATE_SCORE: "/matches/update-score",
			SET_WINNER: "/matches/set-winner",
		},

		TOURNAMENTS: {
			ALL: "/tournaments",
			BY_ID: (id: number) => `/tournaments/by-id/${id}`,
			MATCHES: (id: number) => `/tournaments/tournament-matches/${id}`,
			PARTICIPANTS: (id: number) =>
				`/tournaments/tournament-participants/${id}`,
			USER_TOURNAMENTS: (userId: number) =>
				`/tournaments/user-tournaments/${userId}`,
			IS_USER_PARTICIPANT: (tournamentId: number, userId: number) =>
				`/tournaments/is-user-participant/${tournamentId}/${userId}`,
			FINISH: (id: number) => `/tournaments/finish-tournament/${id}`,
			ADD_PARTICIPANT: "/tournaments/add-participant",
			REMOVE_PARTICIPANT: (tournamentId: number, userId: number) =>
				`/tournaments/delete-participant/${tournamentId}/${userId}`,
			IS_FULL: (tournamentId: number) =>
				`/tournaments/is-tournament-full/${tournamentId}`,
			START: (id: number) => `/tournaments/start-tournament/${id}`,
			GET_TABLE: (id: number) => `/tournaments/tournament-table/${id}`,
			NEXT_MATCH: (id: number) => `/tournaments/next-match/${id}`,
		},

		DASHBOARD: {
			USER: (userId: number) => `/dashboard/${userId}`,
			STATS: (userId: number) => `/dashboard/${userId}/stats`,
			VS: (userId: number, opponentId: number) =>
				`/dashboard/${userId}/vs/${opponentId}`,
		},

		CHAT: {
			CREATE_THREAD: "/chat/thread",
			USER_THREADS: (userId: number) => `/chat/threads/${userId}`,
			SEND_MESSAGE: "/chat/message",
			GET_MESSAGES: (threadId: number) => `/chat/messages/${threadId}`,
			BLOCK: "/chat/block",
			UNBLOCK: "/chat/unblock",
			INVITE: "/chat/invite",
			RESPOND_INVITE: (invitationId: number) =>
				`/chat/invite/${invitationId}/respond`,
			NOTIFY: "/chat/notify",
			NOTIFICATIONS: (userId: number) => `/chat/notifications/${userId}`,
			CHECK_BLOCK: "/chat/check-block",
			BLOCKED_USERS: (userId: number) => `/chat/blocked/${userId}`,
		},
	},
};
