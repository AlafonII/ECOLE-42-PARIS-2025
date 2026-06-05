import { matchService } from "./match.service";
import { tournamentService } from "./tournament.service";

type MatchScore = {
	player1: number;
	player2: number;
};

export type PongMatchResult = {
	score: MatchScore;
	winner: 1 | 2 | null;
	durationSeconds: number;
};

export type ActiveMatchContext = {
	matchId: number;
	players: {
		userId: number | null;
		username?: string | null;
	}[];
	tournamentId?: number | null;
	tournamentName?: string;
	roundNumber?: number | null;
	matchNumber?: number | null;
};

type MatchStatus = {
	state: "loading" | "success" | "error";
	message: string;
};

const ACTIVE_MATCH_KEY = "ft_active_match";
let statusReporter: ((status: MatchStatus) => void) | null = null;
let listenerAttached = false;
let submissionInFlight = false;

export const matchContextService = {
	save(context: ActiveMatchContext): void {
		try {
			localStorage.setItem(ACTIVE_MATCH_KEY, JSON.stringify(context));
		} catch (error) {
			console.warn("Failed to store active match context", error);
		}
	},
	load(): ActiveMatchContext | null {
		try {
			const raw = localStorage.getItem(ACTIVE_MATCH_KEY);
			return raw ? (JSON.parse(raw) as ActiveMatchContext) : null;
		} catch (error) {
			console.warn("Failed to read active match context", error);
			return null;
		}
	},
	clear(): void {
		try {
			localStorage.removeItem(ACTIVE_MATCH_KEY);
		} catch (error) {
			console.warn("Failed to clear active match context", error);
		}
	},
};

export function bindPongMatchReporter(
	reportStatus: (status: MatchStatus) => void
): void {
	statusReporter = reportStatus;
	if (listenerAttached) return;
	listenerAttached = true;

	window.addEventListener("pong:match-finished", async (event) => {
		const detail = (event as CustomEvent<PongMatchResult>).detail;
		if (!detail || submissionInFlight) return;

		const context = matchContextService.load();
		if (!context) return;

		const players = context.players ?? [];
		if (players.length < 2) {
			report({
				state: "error",
				message: "Match participants are missing. Cannot submit score.",
			});
			return;
		}

		submissionInFlight = true;
		report({ state: "loading", message: "Submitting match result..." });

		const matchId = context.matchId;
		const [player1, player2] = players;
		const updates = [] as Promise<{ success: boolean }>[];

		if (player1?.userId != null) {
			updates.push(
				matchService.updateScore(matchId, player1.userId, detail.score.player1)
			);
		}
		if (player2?.userId != null) {
			updates.push(
				matchService.updateScore(matchId, player2.userId, detail.score.player2)
			);
		}

		const updateResults = await Promise.all(updates);
		const updateFailed = updateResults.some((result) => !result.success);

		if (updateFailed) {
			submissionInFlight = false;
			report({
				state: "error",
				message: "Unable to update match scores. Please try again.",
			});
			return;
		}

		const winnerId =
			detail.winner === 1
				? player1?.userId ?? null
				: detail.winner === 2
				? player2?.userId ?? null
				: null;

		const setWinnerResult = await matchService.setWinner(
			matchId,
			winnerId ?? null,
			Math.max(0, Math.round(detail.durationSeconds))
		);

		if (!setWinnerResult.success) {
			submissionInFlight = false;
			report({
				state: "error",
				message: setWinnerResult.error || "Failed to set match winner.",
			});
			return;
		}

		matchContextService.clear();

		if (context.tournamentId) {
			const nextMatch = await tournamentService.getNextMatch(
				context.tournamentId
			);
			if (nextMatch.success) {
				report({
					state: "success",
					message: "Match recorded. Next match is ready.",
				});
				submissionInFlight = false;
				return;
			}

			const noNextMatch = (nextMatch.error || "")
				.toLowerCase()
				.includes("no more matches");
			if (noNextMatch) {
				const finishResult = await tournamentService.finishTournament(
					context.tournamentId
				);
				if (finishResult.success) {
					report({
						state: "success",
						message: "Match recorded. Tournament finished.",
					});
					submissionInFlight = false;
					return;
				}
				report({
					state: "error",
					message:
						finishResult.error ||
						"Match recorded, but finishing tournament failed.",
				});
				submissionInFlight = false;
				return;
			}

			report({
				state: "error",
				message:
					nextMatch.error ||
					"Match recorded, but next match could not be loaded.",
			});
			submissionInFlight = false;
			return;
		}

		report({ state: "success", message: "Match recorded." });
		submissionInFlight = false;
	});
}

function report(status: MatchStatus): void {
	statusReporter?.(status);
}
