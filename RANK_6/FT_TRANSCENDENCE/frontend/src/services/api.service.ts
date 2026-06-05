import { API_CONFIG } from "./api.config";
import { ApiResponse } from "./types";
import { handleAuthError } from '../utils/authUtils';

//This is our basic ApiService class that will be used by each specific service
//It just lists the API calls (get, post, fetch etc) that will be used then in the various services
class ApiService {
	private base_url: string;
	private timeout: number;

	constructor() {
		this.base_url = API_CONFIG.BASE_URL;
		this.timeout = API_CONFIG.TIMEOUT;
	}

	//The request is our way of handling all calls in a consistent manner, it always returns an ApiResponse of the type we need
	private async request<T>(
		endpoint: string,
		options: RequestInit = {}
	): Promise<ApiResponse<T>> {
		const url = `${this.base_url}${endpoint}`;

		const headers: HeadersInit = {
			...options.headers,
		};

		if (options.body !== undefined && options.body !== null) {
			if (!(options.body instanceof FormData)) {
				(headers as Record<string, string>)["Content-Type"] =
					"application/json";
			}
		}

		//The controller will allow us to handle the timeout of any API call
		const controller = new AbortController();
		const timeout_id = setTimeout(() => controller.abort(), this.timeout);

		try {
			const response = await fetch(url, {
				...options,
				headers,
				signal: controller.signal,
				credentials: "include",
			});

			//If we got a response before timeout, we clear it so that the timeout does not happen while we transform the data
			clearTimeout(timeout_id);

			let rawBody = "";
			let payload: ApiResponse<T> | null = null;

			//We parse the response into a format that matches ApiResponse
			try {
				rawBody = await response.text();
				payload = rawBody
					? (JSON.parse(rawBody) as ApiResponse<T>)
					: null;
			} catch (parseError) {
				console.error(
					"Failed to parse response JSON:",
					parseError,
					rawBody
				);
			}

			//Handling of basic errors
			if (!response.ok || !payload?.success) {
				handleAuthError(response.status);
				if (response.status === 404) {
					return {
						success: false,
						error: payload?.error || "Ressource not found",
					};
				}

				return {
					success: false,
					error:
						payload?.error ||
						`Request failed with status ${response.status} ${
							response.statusText || ""
						}`.trim(),
				};
			}
			//We return the payload
			return payload;
		} catch (error) {
			clearTimeout(timeout_id);

			//If timeout, we show a specific error
			if (error instanceof Error && error.name === "AbortError") {
				return {
					success: false,
					error: "Request timeout. Please try again",
				};
			}

			console.error("Network error:", error);
			return {
				success: false,
				error: "Unable to reach the server. Check connection or API url",
			};
		}
	}

	async get<T>(endpoint: string): Promise<ApiResponse<T>> {
		return this.request<T>(endpoint, { method: "GET" });
	}

	async post<T>(endpoint: string, data?: any): Promise<ApiResponse<T>> {
		return this.request<T>(endpoint, {
			method: "POST",
			body: JSON.stringify(data),
		});
	}

	async put<T>(endpoint: string, data?: any): Promise<ApiResponse<T>> {
		return this.request<T>(endpoint, {
			method: "PUT",
			body: JSON.stringify(data),
		});
	}

	async delete<T>(endpoint: string): Promise<ApiResponse<T>> {
		return this.request<T>(endpoint, { method: "DELETE" });
	}

	async uploadFile<T>(
		endpoint: string,
		formData: FormData
	): Promise<ApiResponse<T>> {
		const url = `${this.base_url}${endpoint}`;
		const headers: HeadersInit = {};

		try {
			const response = await fetch(url, {
				method: "POST",
				headers,
				body: formData,
				credentials: "include",
			});

			let rawBody = "";
			let payload: ApiResponse<T> | null = null;

			try {
				rawBody = await response.text();
				payload = rawBody
					? (JSON.parse(rawBody) as ApiResponse<T>)
					: null;
			} catch (parseError) {
				console.warn(
					"Failed to parse upload response:",
					parseError,
					rawBody
				);
			}

			if (!response.ok || !payload?.success) {
				return {
					success: false,
					error:
						payload?.error ||
						`Upload failed with status ${response.status}`,
				};
			}
			return payload;
		} catch (error) {
			console.error("Failed to upload file:", error);
			return {
				success: false,
				error: "Unable to upload file. Check your connection",
			};
		}
	}
}

export const apiService = new ApiService();
