import { ApiResponse } from "./types";
import { API_CONFIG } from "./api.config";
import { handleAuthError } from "../utils/authUtils";

// Friend request and friends list types
export interface FriendRequest {
	id: number;
	sender_id: number;
	receiver_id: number;
	status: string;
	created_at: string;
	username?: string;
}

export interface Friend {
	id: number;
	username: string;
	last_seen_at: string;
}

const API_BASE_URL = API_CONFIG.BASE_URL;

/**
 * Send a friend request to another user
 */
export async function sendFriendRequest(
	friendId: number
): Promise<ApiResponse<{ message: string }>> {
	try {
		const response = await fetch(`${API_BASE_URL}/friend-requests`, {
			method: "POST",
			headers: {
				"Content-Type": "application/json",
			},
			credentials: "include",
			body: JSON.stringify({ friendId }),
		});

		let rawBody = "";
		let payload: ApiResponse<{ message: string }> | null = null;

		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<{ message: string }>)
				: null;
		} catch (parseError) {
			console.warn(
				"sendFriendRequest: failed to parse response",
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
		console.error("Failed to send friend request:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

/**
 * Get received friend requests
 */
export async function getReceivedFriendRequests(): Promise<
	ApiResponse<{ requests: FriendRequest[] }>
> {
	try {
		const response = await fetch(
			`${API_BASE_URL}/friend-requests/received`,
			{
				method: "GET",
				credentials: "include",
			}
		);

		let rawBody = "";
		let payload: ApiResponse<{ requests: FriendRequest[] }> | null = null;

		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<{
						requests: FriendRequest[];
				  }>)
				: null;
		} catch (parseError) {
			console.warn(
				"getReceivedFriendRequests: failed to parse response",
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
		console.error("Failed to fetch received friend requests:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

/**
 * Get sent friend requests
 */
export async function getSentFriendRequests(): Promise<
	ApiResponse<{ requests: FriendRequest[] }>
> {
	try {
		const response = await fetch(`${API_BASE_URL}/friend-requests/sent`, {
			method: "GET",
			credentials: "include",
		});

		let rawBody = "";
		let payload: ApiResponse<{ requests: FriendRequest[] }> | null = null;

		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<{
						requests: FriendRequest[];
				  }>)
				: null;
		} catch (parseError) {
			console.warn(
				"getSentFriendRequests: failed to parse response",
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
		console.error("Failed to fetch sent friend requests:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

/**
 * Accept a friend request
 */
export async function acceptFriendRequest(
	senderId: number
): Promise<ApiResponse<{ message: string }>> {
	try {
		const response = await fetch(`${API_BASE_URL}/friend-requests/accept`, {
			method: "POST",
			headers: {
				"Content-Type": "application/json",
			},
			credentials: "include",
			body: JSON.stringify({ senderId }),
		});

		let rawBody = "";
		let payload: ApiResponse<{ message: string }> | null = null;

		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<{ message: string }>)
				: null;
		} catch (parseError) {
			console.warn(
				"acceptFriendRequest: failed to parse response",
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
		console.error("Failed to accept friend request:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

/**
 * Reject a friend request
 */
export async function rejectFriendRequest(
	senderId: number
): Promise<ApiResponse<{ message: string }>> {
	try {
		const response = await fetch(`${API_BASE_URL}/friend-requests/reject`, {
			method: "POST",
			headers: {
				"Content-Type": "application/json",
			},
			credentials: "include",
			body: JSON.stringify({ senderId }),
		});

		let rawBody = "";
		let payload: ApiResponse<{ message: string }> | null = null;

		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<{ message: string }>)
				: null;
		} catch (parseError) {
			console.warn(
				"rejectFriendRequest: failed to parse response",
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
		console.error("Failed to reject friend request:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

/**
 * Get friends list
 */
export async function getFriendsList(): Promise<
	ApiResponse<{ friendsWithStatus: Friend[] }>
> {
	try {
		//console.log('[getFriendsList] Making request to:', `${API_BASE_URL}/friendsList`);
		
		const response = await fetch(`${API_BASE_URL}/friendsList`, {
			method: "GET",
			credentials: "include",
		});

		//console.log('[getFriendsList] Response status:', response.status, response.statusText);
		//console.log('[getFriendsList] Response status:', response.status, response.statusText);

		let rawBody = "";
		let payload: ApiResponse<{ friendsWithStatus: Friend[] }> | null = null;

		try {
			rawBody = await response.text();
			//console.log('[getFriendsList] Raw response body:', rawBody);
			//console.log('[getFriendsList] Raw response body:', rawBody);
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<{
						friendsWithStatus: Friend[];
				  }>)
				: null;
		} catch (parseError) {
			console.warn(
				"getFriendsList: failed to parse response",
				parseError,
				rawBody
			);
		}

		if (!response.ok || !payload?.success) {
			handleAuthError(response.status);
			console.error("[getFriendsList] Request failed:", payload);
			return {
				success: false,
				error:
					payload?.error ??
					`Request failed with status ${response.status} ${
						response.statusText || ""
					}`.trim(),
			};
		}

		//console.log('[getFriendsList] Success, payload:', payload);
		//console.log('[getFriendsList] Success, payload:', payload);
		return payload;
	} catch (error) {
		console.error("Failed to fetch friends list:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

/**
 * Get relations list
 */
export async function getRelationsList(): Promise<
	ApiResponse<{ relationsWithStatus: Friend[] }>
> {
	try {
		//console.log('[getRelationsList] Making request to:', `${API_BASE_URL}/relationsList`);
		
		const response = await fetch(`${API_BASE_URL}/relationsList`, {
			method: "GET",
			credentials: "include",
		});

		//console.log('[getRelationsList] Response status:', response.status, response.statusText);
		//console.log('[getRelationsList] Response status:', response.status, response.statusText);

		let rawBody = "";
		let payload: ApiResponse<{ relationsWithStatus: Friend[] }> | null =
			null;

		try {
			rawBody = await response.text();
			//console.log('[getRelationsList] Raw response body:', rawBody);
			//console.log('[getRelationsList] Raw response body:', rawBody);
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<{
						relationsWithStatus: Friend[];
				  }>)
				: null;
		} catch (parseError) {
			console.warn(
				"getRelationsList: failed to parse response",
				parseError,
				rawBody
			);
		}

		if (!response.ok || !payload?.success) {
			handleAuthError(response.status);
			console.error("[getRelationsList] Request failed:", payload);
			return {
				success: false,
				error:
					payload?.error ??
					`Request failed with status ${response.status} ${
						response.statusText || ""
					}`.trim(),
			};
		}

		//console.log('[getRelationsList] Success, payload:', payload);
		//console.log('[getRelationsList] Success, payload:', payload);
		return payload;
	} catch (error) {
		console.error("Failed to fetch relations list:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}

/**
 * Remove a friend
 */
export async function removeFriend(
	friendId: number
): Promise<ApiResponse<{ message: string }>> {
	try {
		const response = await fetch(`${API_BASE_URL}/friends/${friendId}`, {
			method: "DELETE",
			credentials: "include",
		});

		let rawBody = "";
		let payload: ApiResponse<{ message: string }> | null = null;

		try {
			rawBody = await response.text();
			payload = rawBody
				? (JSON.parse(rawBody) as ApiResponse<{ message: string }>)
				: null;
		} catch (parseError) {
			console.warn(
				"removeFriend: failed to parse response",
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
		console.error("Failed to remove friend:", error);
		return {
			success: false,
			error: "Unable to reach the server. Check your connection or API URL.",
		};
	}
}
