import {
	AddParticipantInput,
	ApiResponse,
	TournamentCreation,
	Match,
	Tournament,
	TournamentParticipant,
	MatchWithPlayers,
	TournamentRanking,
} from "./types";
import { apiService } from "./api.service";
import { API_CONFIG } from "./api.config";

export class TournamentService {
	async getAllTournaments(): Promise<ApiResponse<Tournament[]>> {
		return apiService.get<Tournament[]>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.ALL
		);
	}

	async getTournamentById(id: number): Promise<ApiResponse<Tournament>> {
		return apiService.get<Tournament>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.BY_ID(id)
		);
	}

	async getTournamentMatches(
		id: number
	): Promise<ApiResponse<MatchWithPlayers[]>> {
		return apiService.get<MatchWithPlayers[]>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.MATCHES(id)
		);
	}

	async getTournamentParticipants(
		id: number
	): Promise<ApiResponse<TournamentParticipant[]>> {
		return apiService.get<TournamentParticipant[]>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.PARTICIPANTS(id)
		);
	}

	async getUserTournaments(
		userId: number
	): Promise<ApiResponse<Tournament[]>> {
		return apiService.get<Tournament[]>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.USER_TOURNAMENTS(userId)
		);
	}

	async isUserParticipant(
		tournamentId: number,
		userId: number
	): Promise<ApiResponse<boolean>> {
		return apiService.get<boolean>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.IS_USER_PARTICIPANT(
				tournamentId,
				userId
			)
		);
	}

	async isTournamentFull(
		tournamentId: number
	): Promise<ApiResponse<boolean>> {
		return apiService.get<boolean>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.IS_FULL(tournamentId)
		);
	}

	async createTournament(
		input: TournamentCreation
	): Promise<ApiResponse<number>> {
		return apiService.post<number>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.ALL,
			input
		);
	}

	async startTournament(id: number): Promise<ApiResponse<number>> {
		return apiService.put<number>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.START(id)
		);
	}

	async finishTournament(id: number): Promise<ApiResponse<void>> {
		return apiService.put<void>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.FINISH(id)
		);
	}

	async addParticipant(
		input: AddParticipantInput
	): Promise<ApiResponse<void>> {
		return apiService.post<void>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.ADD_PARTICIPANT,
			input
		);
	}

	async removeParticipant(
		tournamentId: number,
		userId: number
	): Promise<ApiResponse<void>> {
		return apiService.delete<void>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.REMOVE_PARTICIPANT(
				tournamentId,
				userId
			)
		);
	}

	async getTournamentRanking(
		tournamentId: number
	): Promise<ApiResponse<TournamentRanking[]>> {
		return apiService.get<TournamentRanking[]>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.GET_TABLE(tournamentId)
		);
	}

	async getNextMatch(
		tournamentId: number
	): Promise<ApiResponse<MatchWithPlayers>> {
		return apiService.get<MatchWithPlayers>(
			API_CONFIG.ENDPOINTS.TOURNAMENTS.NEXT_MATCH(tournamentId)
		);
	}

	async getPendingMatch(userId: number): Promise<ApiResponse<{
		matchId: number;
		tournamentId: number;
		tournamentName: string;
		player1Name: string;
		player2Name: string;
		roundNumber: number;
	} | null>> {
		return apiService.get(`/tournaments/pending-match/${userId}`);
	}
}

export const tournamentService = new TournamentService();
