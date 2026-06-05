import { ApiResponse, BasicStats, UserDashboard, VsStats } from "./types";
import { apiService } from "./api.service";
import { API_CONFIG } from "./api.config";

export class DashboardService {
	async getFullUserDashboard(
		userId: number
	): Promise<ApiResponse<UserDashboard>> {
		return apiService.get<UserDashboard>(
			API_CONFIG.ENDPOINTS.DASHBOARD.USER(userId)
		);
	}

	async getUserBasicStats(userId: number): Promise<ApiResponse<BasicStats>> {
		return apiService.get<BasicStats>(
			API_CONFIG.ENDPOINTS.DASHBOARD.STATS(userId)
		);
	}

	async getVsStats(
		userId: number,
		opponentId: number
	): Promise<ApiResponse<VsStats>> {
		return apiService.get<VsStats>(
			API_CONFIG.ENDPOINTS.DASHBOARD.VS(userId, opponentId)
		);
	}
}

export const dashboardService = new DashboardService();
