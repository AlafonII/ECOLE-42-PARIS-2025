import { ApiResponse } from "./types";
import { handleAuthError } from "../utils/authUtils";

// Public shape of a user returned by the backend
export interface PublicUser {
	id: number;
	username: string;
	email: string;
	avatar_url?: string | null;
	created_at: string;
	updated_at: string;
}

// Data sent when creating a new user
export interface CreateUserInput {
	username: string;
	email: string;
	password: string;
	avatar_url?: string | null;
}

export interface LoginChallenge {
	twoFactor: true;
	challengeId: string;
	message?: string;
}

export type LoginResponse = PublicUser | LoginChallenge;

// Which base URL to use for the API
function resolveApiBaseUrl(): string {
	// Prefer a URL from the Vite environment (e.g. VITE_API_BASE_URL in .env)
	const envBase = import.meta.env.VITE_API_BASE_URL;
	if (envBase) return envBase.replace(/\/$/, "");

	// If running in a browser, inspect the current page URL
	if (typeof window !== "undefined") {
		return `${window.location.origin}/api`;
	}

	// Fallback
	return "http://localhost:3000/api";
}

// Final API base URL decided at module load
export const API_BASE_URL = resolveApiBaseUrl();

// Call POST /users to create a new user in the backend
export async function createUser(
	input: CreateUserInput
): Promise<ApiResponse<PublicUser>> {
	try {
		const response = await fetch(`${API_BASE_URL}/users`, {
			method: "POST",
			headers: {
				"Content-Type": "application/json",
			},
			body: JSON.stringify({
				username: input.username,
				email: input.email,
				password: input.password,
				avatar_url: input.avatar_url ?? null,
			}),
		});

		let rawBody = "";
		let payload: ApiResponse<PublicUser> | null = null;

		// Try to parse the response body as JSON
		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<PublicUser>)
				: null;
		} catch (parseError) {
			console.warn(
				"createUser: failed to parse response",
				parseError,
				rawBody
			);
		}

		// If HTTP status is not OK, or API says success=false then return an error
		if (!response.ok || !payload?.success) {
			handleAuthError(response.status);
			return {
				success: false,
				error:
					payload?.error ??
					`Request failed with status ${response.status} ${
						response.statusText || ""
					}`.trim(),
			};
		}

		// return the parsed payload
		return payload;
	} catch (error) {
		// Network / low-level errors
		console.error("Failed to create user:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

// Call GET /users/by-email/:email to fetch a user by email
export async function getUserByEmail(
	email: string
): Promise<ApiResponse<PublicUser>> {
	try {
		const response = await fetch(
			`${API_BASE_URL}/users/by-email/${encodeURIComponent(email)}`
		);

		// If 404 = "User not found" error
		if (response.status === 404) {
			return { success: false, error: "User not found" };
		}

		let rawBody = "";
		let payload: ApiResponse<PublicUser> | null = null;

		// Try to parse the response body as JSON
		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<PublicUser>)
				: null;
		} catch (parseError) {
			console.warn(
				"getUserByEmail: failed to parse response",
				parseError,
				rawBody
			);
		}

		// Handle non-OK HTTP responses or API-level errors
		if (!response.ok || !payload?.success) {
			handleAuthError(response.status);
			return {
				success: false,
				error:
					payload?.error ??
					`Request failed with status ${response.status} ${
						response.statusText || ""
					}`.trim(),
			};
		}

		// Successfully got a user
		return payload;
	} catch (error) {
		console.error("Failed to fetch user by email:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

// Call GET /users/by-username/:username to see if a username exists
export async function getUserByUsername( // TODO: too much personal info exposed for frontend
	username: string
): Promise<ApiResponse<PublicUser>> {
	try {
		const response = await fetch(
			`${API_BASE_URL}/users/by-username/${encodeURIComponent(username)}`
		);

		// If 404 = "User not found" error
		if (response.status === 404) {
			return { success: false, error: "User not found" };
		}

		let rawBody = "";
		let payload: ApiResponse<PublicUser> | null = null;

		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<PublicUser>)
				: null;
		} catch (parseError) {
			console.warn(
				"getUserByUsername: failed to parse response",
				parseError,
				rawBody
			);
		}

		if (!response.ok || !payload?.success) {
			handleAuthError(response.status);
			return {
				success: false,
				error:
					payload?.error ??
					`Request failed with status ${response.status} ${
						response.statusText || ""
					}`.trim(),
			};
		}

		return payload;
	} catch (error) {
		console.error("Failed to fetch user by username:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

/**
 * Call POST /api/login to authenticate a user with password verification
 */
export async function loginUser(
	email: string,
	password: string
): Promise<ApiResponse<LoginResponse>> {
	try {
		const response = await fetch(`${API_BASE_URL}/login`, {
			method: "POST",
			headers: {
				"Content-Type": "application/json",
			},
			credentials: "include",
			body: JSON.stringify({ email, password }),
		});

		let rawBody = "";
		let payload: ApiResponse<LoginResponse> | null = null;

		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<LoginResponse>)
				: null;
		} catch (parseError) {
			console.warn(
				"loginUser: failed to parse response",
				parseError,
				rawBody
			);
		}

		if (!response.ok || !payload?.success) {
			return {
				success: false,
				error:
					payload?.error ??
					`Request failed with status ${response.status} ${
						response.statusText || ""
					}`.trim(),
			};
		}

		return payload;
	} catch (error) {
		console.error("Failed to login:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

export async function verifyOTP(
	challengeId: string,
	otp: string
): Promise<ApiResponse<PublicUser>> {
	try {
		//console.log("challengeID = " + challengeId);
		const response = await fetch(`${API_BASE_URL}/verify-otp`, {
			method: "POST",
			headers: {
				"Content-Type": "application/json",
			},
			credentials: "include",
			body: JSON.stringify({ challengeId: challengeId, otp: otp }),
		});

		let rawBody = "";
		let payload: ApiResponse<PublicUser> | null = null;

		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<PublicUser>)
				: null;
		} catch (parseError) {
			console.warn(
				"verifyOTP: failed to parse response",
				parseError,
				rawBody
			);
		}

		if (!response.ok || !payload?.success) {
			handleAuthError(response.status);
			return {
				success: false,
				error:
					payload?.error ??
					`Request failed with status ${response.status} ${
						response.statusText || ""
					}`.trim(),
			};
		}

		return payload;
	} catch (error) {
		console.error("Failed to verify OTP:", error);
		return {
			success: false,
			error: "Unable to reach the server",
		};
	}
}

export function isLoginChallenge(data: LoginResponse): data is LoginChallenge {
	return (data as any).twoFactor === true;
}

export function isPublicUser(data: LoginResponse): data is PublicUser {
	return (data as PublicUser).id !== undefined;
}
