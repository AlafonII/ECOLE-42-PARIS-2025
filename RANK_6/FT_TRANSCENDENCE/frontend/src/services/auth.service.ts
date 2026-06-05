import { apiService } from "./api.service";

export const getMe = async () => {
	return apiService.get<{
		id: number;
		username: string;
		email: string;
	}>("/auth/me");
};
