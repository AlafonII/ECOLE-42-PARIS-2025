import dashboardTitle from "../assets/images/page_titles/DASHBOARD_PAGE.png";
import arrowBack from "../assets/images/page_assets/ARROW_BACK.png";
import { dashboardService } from "../services/dashboard.service";
import { sessionService } from "../services/session.service";
import type { Adversary, MatchRow, UserDashboard } from "../services/types";
import { escapeHtml } from "../utils/htmlhelpers";

// Keep averages readable (no trailing .00)
const formatAverageScore = (value: number): string => {
	if (!Number.isFinite(value)) return "0";
	return value.toFixed(2).replace(/\.?0+$/, "");
};

// Guard percent math for UI graphs
const clampPercent = (value: number): number => {
	if (!Number.isFinite(value)) return 0;
	return Math.min(100, Math.max(0, value));
};

const percentOf = (value: number, total: number): number => {
	if (!Number.isFinite(value) || !Number.isFinite(total) || total <= 0)
		return 0;
	return (value / total) * 100;
};

// Small layout helper for consistent stat cards
const graphCard = (
	label: string,
	value: string,
	graph: string,
	note?: string,
	wrapperClass = "",
): string => `
  <div class="rounded-lg border border-[rgba(230,240,255,0.25)] bg-[rgba(255,255,255,0.03)] p-4 space-y-3${wrapperClass ? ` ${wrapperClass}` : ""}">
    <p class="text-[11px] font-semibold uppercase tracking-[0.2em] text-[var(--muted)]">${label}</p>
    ${graph}
    <div class="space-y-1">
      <p class="text-sm font-semibold text-[var(--fg)]">${value}</p>
      ${note ? `<p class="text-xs text-[var(--muted)]">${note}</p>` : ""}
    </div>
  </div>
`;

// SVG donut chart for win rate / completion style stats
const donutGraph = (percent: number, label: string, color: string): string => {
	const safePercent = clampPercent(percent);
	return `
    <div class="relative mx-auto h-20 w-20">
      <svg viewBox="0 0 36 36" class="h-20 w-20">
        <circle
          cx="18"
          cy="18"
          r="15.9155"
          fill="none"
          stroke="rgba(255,255,255,0.12)"
          stroke-width="3"
        ></circle>
        <circle
          cx="18"
          cy="18"
          r="15.9155"
          fill="none"
          stroke="${color}"
          stroke-width="3"
          stroke-dasharray="${safePercent} 100"
          stroke-linecap="round"
          transform="rotate(-90 18 18)"
        ></circle>
      </svg>
      <span class="absolute inset-0 flex items-center justify-center text-sm font-semibold text-[var(--fg)]">${label}</span>
    </div>
  `;
};

// Horizontal split bar for wins vs losses
const matchSplitGraph = (
	wins: number,
	losses: number,
	total: number,
): string => {
	const winPercent = clampPercent(percentOf(wins, total));
	const lossPercent = clampPercent(percentOf(losses, total));
	return `
    <div class="space-y-2">
      <div class="h-3 w-full overflow-hidden rounded-full bg-[rgba(255,255,255,0.1)]">
        <div class="flex h-full w-full">
          <span class="h-full bg-[var(--easy-color)]" style="width: ${winPercent}%"></span>
          <span class="h-full bg-[var(--hard-color)]" style="width: ${lossPercent}%"></span>
        </div>
      </div>
      <div class="flex flex-wrap justify-between gap-2 text-xs text-[var(--muted)]">
        <span>${wins} wins</span>
        <span>${losses} losses</span>
      </div>
    </div>
  `;
};

// Simple progress bar for average score
const scoreMeterGraph = (averageScore: number, scale: number): string => {
	const safeScale = Number.isFinite(scale) && scale > 0 ? scale : 1;
	const percent = clampPercent((averageScore / safeScale) * 100);
	return `
    <div class="mx-auto w-full max-w-[220px] space-y-2">
      <div class="relative h-3 w-full overflow-hidden rounded-full bg-[rgba(255,255,255,0.1)]">
        <span class="absolute inset-y-0 left-0 bg-[var(--game-yellow)]" style="width: ${percent}%"></span>
      </div>
      <div class="flex justify-between text-xs text-[var(--muted)]">
        <span>0</span>
        <span>${safeScale}</span>
      </div>
    </div>
  `;
};

// Card for the most frequent opponent
const rivalCard = (adversary: Adversary | null): string => {
	if (!adversary) {
		return `
      <div class="rounded-lg border border-dashed border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.02)] p-4 text-center text-sm text-[var(--muted)]">
        No rivals yet. Play a few matches to build your matchup history.
      </div>
    `;
	}

	const totalGames = adversary.games_against || 0;
	const winRate = totalGames
		? Math.round((adversary.wins_against / totalGames) * 100)
		: 0;

	return `
    <div class="rounded-lg border border-[rgba(230,240,255,0.25)] bg-[rgba(255,255,255,0.03)] p-4 space-y-3">
      <div class="space-y-1">
        <p class="text-base font-semibold text-[var(--fg)]">${escapeHtml(
			adversary.username,
		)}</p>
        <p class="text-xs text-[var(--muted)]">${totalGames} matches together</p>
      </div>
      <div class="flex flex-wrap items-center gap-3 text-sm">
        <span class="text-[var(--easy-color)]">${adversary.wins_against} wins</span>
        <span class="text-[var(--hard-color)]">${adversary.losses_against} losses</span>
        <span class="text-[var(--muted)]">${winRate}% win rate</span>
      </div>
    </div>
  `;
};

// Row renderer for recent matches list
const matchRow = (match: MatchRow): string => {
	const result =
		match.userScore === match.opponentScore
			? "Draw"
			: match.userScore > match.opponentScore
				? "Win"
				: "Loss";
	const resultClass =
		result === "Win"
			? "text-[var(--easy-color)]"
			: result === "Loss"
				? "text-[var(--hard-color)]"
				: "text-[var(--game-yellow)]";
	return `
    <div class="rounded-lg border border-[rgba(230,240,255,0.2)] bg-[rgba(255,255,255,0.03)] p-4">
      <div class="flex flex-col gap-2 sm:flex-row sm:items-center sm:justify-between">
        <div class="space-y-1">
          <p class="text-sm font-semibold text-[var(--fg)]">vs ${escapeHtml(
				match.opponentName,
			)}</p>
          <p class="text-xs text-[var(--muted)]">Match #${match.matchId}</p>
        </div>
        <div class="flex items-center gap-3">
          <span class="text-base font-semibold text-[var(--fg)]">${match.userScore} - ${match.opponentScore}</span>
          <span class="text-xs font-semibold uppercase tracking-[0.2em] ${resultClass}">${result}</span>
        </div>
      </div>
    </div>
  `;
};

const Dashboard = () => {
	const html = `
    <section class="retro-card crt space-y-6 p-8">
      <a
        id="dashboard-back-link"
        data-link
        href="/livechat"
        class="fixed left-6 top-6 z-20 flex items-center gap-3 text-base font-semibold uppercase text-[var(--fg)]"
        aria-label="Back"
      >
        <img src="${arrowBack}" alt="" class="h-7 w-auto" />
        <span>Back</span>
      </a>
      <div class="flex flex-col gap-4 sm:flex-row sm:items-center sm:justify-between">
        <div class="flex justify-center sm:justify-start">
          <img src="${dashboardTitle}" alt="Dashboard" class="mx-auto h-auto max-w-full" />
        </div>
      </div>

      <div id="dashboard-body" class="space-y-6">
        <div class="grid gap-4 lg:grid-cols-2">
          <div class="pixel-frame pixel-frame--clear p-6 space-y-4">
            <div class="flex flex-col gap-3 sm:flex-row sm:items-center sm:justify-between">
              <div class="space-y-1">
                <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Player overview</p>
                <p class="text-sm text-[var(--muted)]">Matches against human opponents only.</p>
              </div>
              <button id="dashboard-refresh" class="retro-btn">Refresh</button>
            </div>
            <div id="dashboard-stats-grid" class="grid gap-3 sm:grid-cols-2">
              <p class="text-sm text-[var(--muted)]">Loading stats...</p>
            </div>
          </div>

          <div class="pixel-frame pixel-frame--clear p-6 space-y-3">
            <div class="space-y-1">
              <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Rival focus</p>
              <p class="text-sm text-[var(--muted)]">Your most frequent matchup.</p>
            </div>
            <div id="dashboard-rival-card" class="text-sm text-[var(--muted)]">Loading rival stats...</div>
          </div>
        </div>

        <div class="pixel-frame pixel-frame--clear p-6 space-y-3">
          <div class="space-y-1">
            <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Recent matches</p>
            <p class="text-sm text-[var(--muted)]">Latest 5 games played.</p>
          </div>
          <div id="dashboard-matches-list" class="space-y-3">
            <p class="text-sm text-[var(--muted)]">Loading recent matches...</p>
          </div>
        </div>
      </div>
    </section>
  `;

	const onMount = () => {
		// 1) Grab the dashboard container; bail if the page skeleton isn't mounted
		const body = document.querySelector<HTMLDivElement>("#dashboard-body");
		if (!body) return;

		// 2) Require a logged-in session before showing any stats
		const user = sessionService.loadUser();
		if (!user) {
			body.innerHTML = `
        <div class="pixel-frame pixel-frame--clear p-6 text-center space-y-2">
          <p class="text-base font-semibold text-[var(--fg)]">You are not signed in.</p>
          <p class="text-sm text-[var(--muted)]">Log in to see your dashboard stats.</p>
          <a data-link href="/login" class="retro-btn justify-center">Go to Login</a>
        </div>
      `;
			return;
		}

		// 3) Optional user override via ?user=; fall back to the current user
		const params = new URLSearchParams(window.location.search);
		const rawUserId = params.get("user");
		const requestedUserId = rawUserId ? Number(rawUserId) : null;
		const viewUserId =
			Number.isFinite(requestedUserId) &&
			(requestedUserId as number) > 0 &&
			requestedUserId !== user.id
				? (requestedUserId as number)
				: user.id;

		// 4) Back link: prefer browser history, otherwise fallback to /livechat
		const backLink = document.querySelector<HTMLAnchorElement>(
			"#dashboard-back-link",
		);
		const backFallback = backLink?.getAttribute("href") ?? "/livechat";
		const handleBackClick = (event: MouseEvent) => {
			event.preventDefault();
			event.stopPropagation();

			if (window.history.length > 1) {
				history.back();
				return;
			}

			history.pushState({}, "", backFallback);
			window.dispatchEvent(new PopStateEvent("popstate"));
		};
		backLink?.addEventListener("click", handleBackClick);

		// 5) Cache DOM hooks for each section we need to update
		const statsGrid = document.querySelector<HTMLDivElement>(
			"#dashboard-stats-grid",
		);
		const rivalCardEl = document.querySelector<HTMLDivElement>(
			"#dashboard-rival-card",
		);
		const matchesList = document.querySelector<HTMLDivElement>(
			"#dashboard-matches-list",
		);
		const refreshBtn =
			document.querySelector<HTMLButtonElement>("#dashboard-refresh");

		if (!statsGrid || !rivalCardEl || !matchesList) return;

		// 6) Renderer: build HTML for all sections from the API payload
		const renderDashboard = (dashboard: UserDashboard) => {
			const totalGames = dashboard.gamesPlayed;
			const victories = dashboard.victories;
			const defeats = dashboard.defeats;
			const averageScore = Number.isFinite(dashboard.averageScore)
				? dashboard.averageScore
				: 0;
			const winRate = totalGames
				? Math.round((victories / totalGames) * 100)
				: 0;
			const hasGames = totalGames > 0;
			const scoreScale = 5;
			const averageScoreOutOfFive = Math.min(
				scoreScale,
				Math.max(0, averageScore),
			);

			statsGrid.innerHTML = [
				graphCard(
					"Win rate",
					hasGames ? `${winRate}% win rate` : "No matches yet",
					donutGraph(winRate, `${winRate}%`, "var(--easy-color)"),
					hasGames
						? `${victories} wins in ${totalGames} games`
						: "Play a match to start the trend.",
				),
				graphCard(
					"Average score",
					`${formatAverageScore(averageScoreOutOfFive)} / ${scoreScale}`,
					scoreMeterGraph(averageScoreOutOfFive, scoreScale),
					undefined,
					"flex flex-col justify-center text-center",
				),
				graphCard(
					"Match split",
					hasGames
						? `${victories} wins / ${defeats} losses`
						: "No matches yet",
					matchSplitGraph(victories, defeats, totalGames),
					hasGames ? `${totalGames} total games` : undefined,
					"sm:col-span-2",
				),
			].join("");

			rivalCardEl.innerHTML = rivalCard(dashboard.mostFrequentAdversary);

			if (!dashboard.recentMatches.length) {
				matchesList.innerHTML = `
          <div class="rounded-lg border border-dashed border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.02)] p-4 text-center text-sm text-[var(--muted)]">
            No matches played yet.
          </div>
        `;
				return;
			}

			matchesList.innerHTML = dashboard.recentMatches
				.map((match) => matchRow(match))
				.join("");
		};

		// 7) Renderer: common error state for all sections
		const renderError = (message: string) => {
			const safeMessage = escapeHtml(message);
			statsGrid.innerHTML = `<p class="text-sm text-[var(--error)]">${safeMessage}</p>`;
			rivalCardEl.innerHTML = `<p class="text-sm text-[var(--error)]">${safeMessage}</p>`;
			matchesList.innerHTML = `<p class="text-sm text-[var(--error)]">${safeMessage}</p>`;
		};

		// 8) Fetch data, show loading states, then updates the UI
		const loadDashboard = async () => {
			statsGrid.innerHTML = `<p class="text-sm text-[var(--muted)]">Loading stats...</p>`;
			rivalCardEl.innerHTML = `<p class="text-sm text-[var(--muted)]">Loading rival stats...</p>`;
			matchesList.innerHTML = `<p class="text-sm text-[var(--muted)]">Loading recent matches...</p>`;

			const response =
				await dashboardService.getFullUserDashboard(viewUserId);
			if (!response.success || !response.data) {
				const message =
					response.error ?? "Failed to load dashboard stats";
				renderError(message);
				return;
			}

			renderDashboard(response.data);
		};

		// 9) Wire refresh + do the initial load
		refreshBtn?.addEventListener("click", loadDashboard);
		loadDashboard();
	};

	return { html, onMount };
};

export default Dashboard;
