import { ApiResponse, CreateMatchInput, MatchWithPlayers } from "./types";
import { apiService } from "./api.service";
import { API_CONFIG } from "./api.config";

export class MatchService {
	async getAllMatches(): Promise<ApiResponse<MatchWithPlayers[]>> {
		return apiService.get<MatchWithPlayers[]>(
			API_CONFIG.ENDPOINTS.MATCHES.ALL
		);
	}

	async getMatchById(id: number): Promise<ApiResponse<MatchWithPlayers>> {
		return apiService.get<MatchWithPlayers>(
			API_CONFIG.ENDPOINTS.MATCHES.BY_ID(id)
		);
	}

	async getUserMatches(id: number): Promise<ApiResponse<MatchWithPlayers[]>> {
		return apiService.get<MatchWithPlayers[]>(
			API_CONFIG.ENDPOINTS.MATCHES.BY_USER(id)
		);
	}

	async getFinishedMatches(): Promise<ApiResponse<MatchWithPlayers[]>> {
		return apiService.get<MatchWithPlayers[]>(
			API_CONFIG.ENDPOINTS.MATCHES.FINISHED
		);
	}

	async getUnplayedMatches(): Promise<ApiResponse<MatchWithPlayers[]>> {
		return apiService.get<MatchWithPlayers[]>(
			API_CONFIG.ENDPOINTS.MATCHES.UNPLAYED
		);
	}

	async createMatch(input: CreateMatchInput): Promise<ApiResponse<number>> {
		return apiService.post<number>(
			API_CONFIG.ENDPOINTS.MATCHES.CREATE,
			input
		);
	}

	async addParticipant(
		matchId: number,
		userId?: number
	): Promise<ApiResponse<MatchWithPlayers>> {
		return apiService.post<MatchWithPlayers>(
			API_CONFIG.ENDPOINTS.MATCHES.ADD_PARTICIPANT,
			{ matchId, userId }
		);
	}

	async updateScore(
		matchId: number,
		userId: number | null,
		score: number
	): Promise<ApiResponse<MatchWithPlayers>> {
		return apiService.put<MatchWithPlayers>(
			API_CONFIG.ENDPOINTS.MATCHES.UPDATE_SCORE,
			{ matchId, userId, score }
		);
	}

	async setWinner(
		matchId: number,
		winnerId: number | null,
		durationSeconds: number
	): Promise<ApiResponse<MatchWithPlayers>> {
		return apiService.put<MatchWithPlayers>(
			API_CONFIG.ENDPOINTS.MATCHES.SET_WINNER,
			{ matchId, winnerId, durationSeconds }
		);
	}
}

export const matchService = new MatchService();
