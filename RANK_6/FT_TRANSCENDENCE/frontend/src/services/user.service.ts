import {
	ApiResponse,
	CreateUserInput,
	LoginInput,
	PublicUser,
	UpdateUserInput,
} from "./types";
import { apiService } from "./api.service";
import { API_CONFIG } from "./api.config";

export class UserService {
	async getAllUsers(): Promise<ApiResponse<PublicUser[]>> {
		return apiService.get<PublicUser[]>(API_CONFIG.ENDPOINTS.USERS.ALL);
	}

	async getUserByID(id: number): Promise<ApiResponse<PublicUser>> {
		return apiService.get<PublicUser>(API_CONFIG.ENDPOINTS.USERS.BY_ID(id));
	}

	async getUserByEmail(email: string): Promise<ApiResponse<PublicUser>> {
		return apiService.get<PublicUser>(
			API_CONFIG.ENDPOINTS.USERS.BY_EMAIL(email),
		);
	}

	async isUsernameFree(username: string): Promise<ApiResponse<boolean>> {
		return apiService.get<boolean>(
			API_CONFIG.ENDPOINTS.USERS.IS_USERNAME_FREE(username),
		);
	}

	async getUserByUsername(
		username: string,
	): Promise<ApiResponse<PublicUser>> {
		return apiService.get<PublicUser>(
			API_CONFIG.ENDPOINTS.USERS.BY_USERNAME(username),
		);
	}

	async createUser(input: CreateUserInput): Promise<ApiResponse<PublicUser>> {
		return apiService.post<PublicUser>(API_CONFIG.ENDPOINTS.USERS.ALL, {
			username: input.username,
			email: input.email,
			password_hash: input.password,
			avatar_url: input.avatar_url ?? null,
			two_factor: input.twoFactor,
		});
	}

	async updateUser(
		id: number,
		updates: UpdateUserInput,
	): Promise<ApiResponse<PublicUser>> {
		return apiService.put<PublicUser>(
			API_CONFIG.ENDPOINTS.USERS.UPDATE(id),
			updates,
		);
	}

	async deleteUser(id: number): Promise<ApiResponse<{ message: string }>> {
		return apiService.delete<{ message: string }>(
			API_CONFIG.ENDPOINTS.USERS.DELETE(id),
		);
	}

	async login(input: LoginInput): Promise<ApiResponse<PublicUser>> {
		const result = await apiService.post<PublicUser>(
			API_CONFIG.ENDPOINTS.USERS.LOGIN,
			input,
		);

		return result;
	}

	async uploadAvatar(
		id: number,
		file: File,
	): Promise<ApiResponse<PublicUser>> {
		const form = new FormData();
		form.append("avatar", file);
		return apiService.uploadFile<PublicUser>(
			API_CONFIG.ENDPOINTS.USERS.AVATAR(id),
			form,
		);
	}
}

export const userService = new UserService();
