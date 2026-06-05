import tournamentTitle from "../assets/images/page_titles/TOURNAMENT_PAGE.png";
import { sessionService } from "../services/session.service";
import { tournamentService } from "../services/tournament.service";
import {
	Match,
	MatchWithPlayers,
	Tournament,
	TournamentParticipant,
	TournamentRanking,
} from "../services/types";
import { userService } from "../services/user.service";
import { renderAvatarDisplay } from "../utils/avatar";
import { escapeHtml } from "../utils/htmlhelpers";
import { statusBadge } from "./Tournaments";

function getTournamentIdFromPath(): number | null {
	const parts = window.location.pathname.split("/").filter(Boolean);
	const idStr = parts[1];
	const id = Number(idStr);
	return Number.isFinite(id) && id > 0 ? id : null;
}

function getParticipantAlias(
	participants: TournamentParticipant[],
	userId: number | null,
): string {
	const participant = participants.find((p) => p.user_id === userId);
	return participant?.alias ?? "Unknown alias";
}

function formatPlayers(
	m: MatchWithPlayers,
	participants: TournamentParticipant[],
): string {
	const p = m.players ?? [];
	const a = p[0];
	const b = p[1];
	const aName = a ? getParticipantAlias(participants, a.user_id) : "TBD";
	const bName = b ? getParticipantAlias(participants, b.user_id) : "TBD";
	const aScore = typeof a?.score === "number" ? a.score : "-";
	const bScore = typeof b?.score === "number" ? b.score : "-";
	return `${escapeHtml(
		aName,
	)} <span class="text-[var(--muted)]">(${aScore})</span> vs ${escapeHtml(
		bName,
	)} <span class="text-[var(--muted)]">(${bScore})</span>`;
}

const TournamentPage = () => {
	const html = `
    <section class="retro-card crt space-y-6 p-8">
      <div class="flex flex-col gap-4 sm:flex-row sm:items-center sm:justify-between">
        <div class="flex justify-center sm:justify-start">
          <img src="${tournamentTitle}" alt="Tournament" class="mx-auto h-auto max-w-full" />
        </div>
      </div>

      <div id="tournament-header" class="pixel-frame pixel-frame--clear p-6 space-y-3">
        <p class="text-sm text-[var(--muted)]">Loading tournament...</p>
      </div>

      <div id="tournament-actions" class="flex flex-wrap items-center gap-2">
	  	<button id="tournament-refresh" class="retro-btn">Refresh</button>
	  </div>
      	<div id="participants-container" class="pixel-frame pixel-frame--clear p-6 space-y-3">
        	<div class="flex items-center justify-between gap-3">
        	  <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Registered players</p>
        	  <p id="tournament-participants-count" class="text-xs text-[var(--muted)]"></p>
        	</div>
        	<div id="tournament-participants" class="text-sm text-[var(--muted)]">Loading participants...</div>
      	</div>

		<div id="winner-container" class="hidden pixel-frame pixel-frame--clear p-6 space-y-3">
        	<p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Tournament Winner</p>
        	<div id="tournament-winner" class="text-sm text-[var(--muted)]"></div>
      	</div>

      <div id="alias-modal" class="fixed inset-0 z-50 hidden items-center justify-center">
        <div id="alias-backdrop" class="absolute inset-0 bg-black/60"></div>

        <div class="relative w-[min(520px,92vw)] pixel-frame pixel-frame--clear p-6 space-y-4 bg-[rgba(10,20,30,0.9)]">
          <div class="space-y-1">
            <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Join tournament</p>
            <p class="text-sm text-[var(--muted)]">Choose an alias for this tournament.</p>
          </div>

          <label class="space-y-1 block">
            <span class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Alias</span>
            <input
              id="alias-input"
              class="w-full rounded-md border border-[rgba(230,240,255,0.25)] bg-[rgba(10,20,30,0.35)] px-3 py-2 text-[var(--fg)] placeholder:text-[rgba(230,240,255,0.45)]"
              placeholder="e.g. TheWall"
              maxlength="20"
            />
          </label>

          <div class="flex flex-col gap-2 sm:flex-row sm:justify-end">
            <button id="alias-cancel" class="retro-btn">Cancel</button>
            <button id="alias-confirm" class="retro-btn">Join</button>
          </div>

          <div id="alias-error" class="text-sm text-[var(--muted)]"></div>
        </div>
      </div>
	<div id="progress-sections" class="hidden space-y-4">
      <div class="grid grid-cols-1 gap-4 lg:grid-cols-2">
        <div class="pixel-frame pixel-frame--clear p-6 space-y-3">
          <div class="flex items-center justify-between gap-3">
            <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Ranking</p>
          </div>
          <div id="tournament-ranking" class="text-sm text-[var(--muted)]">Loading ranking...</div>
        </div>

        <div class="pixel-frame pixel-frame--clear p-6 space-y-3">
          <div class="flex items-center justify-between gap-3">
            <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Next match</p>
          </div>
          <div id="tournament-next-match" class="text-sm text-[var(--muted)]">Loading next match...</div>
          <div id="tournament-next-match-actions" class="pt-2"></div>
        </div>
      </div>

      <div class="grid grid-cols-1 gap-4 lg:grid-cols-2">
        <div class="pixel-frame pixel-frame--clear p-6 space-y-3">
          <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Games played</p>
          <div id="tournament-played" class="text-sm text-[var(--muted)]">Loading matches...</div>
        </div>

        <div class="pixel-frame pixel-frame--clear p-6 space-y-3">
          <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Games to be played</p>
          <div id="tournament-upcoming" class="text-sm text-[var(--muted)]">Loading matches...</div>
        </div>
      </div>
	</div>
    </section>
  `;

	const onMount = async () => {
		const id = getTournamentIdFromPath();
		const headerEl =
			document.querySelector<HTMLDivElement>("#tournament-header");
		const actionsEl = document.querySelector<HTMLDivElement>(
			"#tournament-actions",
		);
		const rankingEl = document.querySelector<HTMLDivElement>(
			"#tournament-ranking",
		);
		const nextMatchEl = document.querySelector<HTMLDivElement>(
			"#tournament-next-match",
		);
		const nextMatchActionsEl = document.querySelector<HTMLDivElement>(
			"#tournament-next-match-actions",
		);
		const playedEl =
			document.querySelector<HTMLDivElement>("#tournament-played");
		const upcomingEl = document.querySelector<HTMLDivElement>(
			"#tournament-upcoming",
		);
		const participantsCountEl =
			document.querySelector<HTMLParagraphElement>(
				"#tournament-participants-count",
			);
		const participantsContainer = document.querySelector<HTMLDivElement>(
			"#participants-container",
		);
		const participantsListEl = document.querySelector<HTMLDivElement>(
			"#tournament-participants",
		);
		const progressSections =
			document.querySelector<HTMLDivElement>("#progress-sections");
		const winnerContainer =
			document.querySelector<HTMLDivElement>("#winner-container");
		const winnerEl =
			document.querySelector<HTMLDivElement>("#tournament-winner");

		if (!headerEl || !rankingEl || !nextMatchEl || !playedEl || !upcomingEl)
			return;

		if (!id) {
			headerEl.innerHTML = `
        		<div class="space-y-2">
          			<p class="text-base font-semibold text-[var(--fg)]">Invalid tournament URL</p>
        		</div>
      		`;
			return;
		}

		const user = sessionService.loadUser();

		const [tRes, participantsRes, rankingRes, matchesRes, nextRes] =
			await Promise.all([
				tournamentService.getTournamentById(id),
				tournamentService.getTournamentParticipants(id),
				tournamentService.getTournamentRanking(id),
				tournamentService.getTournamentMatches(id),
				tournamentService.getNextMatch(id),
			]);

		if (!tRes.success || !tRes.data) {
			headerEl.innerHTML = `
        		<div class="space-y-2">
          			<p class="text-base font-semibold text-[var(--fg)]">Tournament not found</p>
         			 <p class="text-sm text-[var(--muted)]">${escapeHtml(
							tRes.error ?? "Unknown error",
						)}</p>
        		</div>
      		`;
			return;
		}

		const tournament: Tournament = tRes.data;
		const participants: TournamentParticipant[] =
			participantsRes.success && participantsRes.data
				? participantsRes.data
				: [];

		const usersById = new Map<number, any>();
		await Promise.all(
			participants.map(async (p) => {
				if (usersById.has(p.user_id)) return;
				const uRes = await userService.getUserByID(p.user_id);
				if (uRes.success && uRes.data) {
					usersById.set(p.user_id, uRes.data);
				}
			}),
		);

		const isInProgress =
			tournament.status === "in_progress" ||
			tournament.status === "finished";

		if (progressSections) {
			if (isInProgress) {
				progressSections.classList.remove("hidden");
			} else {
				progressSections.classList.add("hidden");
			}
		}

		if (isInProgress) {
			participantsContainer?.classList.add("hidden");
		} else {
			participantsContainer?.classList.remove("hidden");
		}

		if (participantsCountEl) {
			participantsCountEl.textContent = `${participants.length}/${tournament.max_participants}`;
		}

		if (participantsListEl) {
			if (!participants.length) {
				participantsListEl.innerHTML = `<p class="text-sm text-[var(--muted)]">No players registered yet.</p>`;
			} else {
				participantsListEl.innerHTML = `
          <div class="space-y-2">
          ${participants
				.slice()
				.sort((a, b) => a.created_at.localeCompare(b.created_at))
				.map((p, idx) => {
					const userData = usersById.get(p.user_id);
					const avatarHtml = userData
						? renderAvatarDisplay(userData)
						: `<span class="text-lg font-bold text-[var(--fg)]">${p.alias.slice(0, 2).toUpperCase()}</span>`;

					return `
                		<div class="flex items-center justify-between gap-3 rounded-md border border-[rgba(230,240,255,0.12)] bg-[rgba(230,240,255,0.04)] p-3">
                  			<div class="flex items-center gap-3">
                    			<div class="inline-flex h-10 w-10 items-center justify-center overflow-hidden rounded-full border border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.2)]">
                      				${avatarHtml}
                    			</div>
                    			<div class="space-y-0.5">
                      			<p class="text-sm font-semibold text-[var(--fg)]">
                        			${idx + 1}. ${escapeHtml(p.alias)}
                      			</p>
                    			</div>
                  			</div>
                		</div>
              		`;
				})
				.join("")}
        	</div>
        	`;
			}
		}

		const ranking: TournamentRanking[] =
			rankingRes.success && rankingRes.data ? rankingRes.data : [];
		const matches: MatchWithPlayers[] =
			matchesRes.success && matchesRes.data ? matchesRes.data : [];
		const nextMatch: MatchWithPlayers | null =
			nextRes.success && nextRes.data ? nextRes.data : null;

		const organiserId = tournament.tournament_organizer_id;

		const isOrganiser =
			user && organiserId != null && user.id === organiserId;
		const canStart =
			isOrganiser &&
			tournament.status === "registration" &&
			participants.length >= 2;

		const isUserInTournament =
			user &&
			(await tournamentService.isUserParticipant(id, user?.id)).data;

		const canUserRegister =
			user &&
			!isUserInTournament &&
			tournament.status === "registration" &&
			participants.length < tournament.max_participants;

		const canUserLeave =
			!!isUserInTournament &&
			tournament.status === "registration" &&
			!(tournament as any).started_at;

		headerEl.innerHTML = `
      		<div class="flex flex-col gap-3 sm:flex-row sm:items-start sm:justify-between">
        		<div class="space-y-1">
          			<p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Tournament</p>
          			<h2 class="text-xl font-extrabold text-[var(--fg)]">${escapeHtml(
						tournament.name,
					)}</h2>
          			<p class="text-sm text-[var(--muted)]">Max players: ${
						tournament.max_participants
					}</p>
          			<p class="text-sm text-[var(--muted)]">Players registered: ${
						participants.length
					}</p>
        		</div>
        		<div class="flex items-center gap-3">
          			${statusBadge(tournament.status)}
        		</div>
      		</div>
    	`;

		if (actionsEl) {
			actionsEl.innerHTML = `
        		<button id="tournament-refresh" class="retro-btn">Refresh</button>
        		${canStart ? `<button id="tournament-start" class="retro-btn">Start tournament</button>` : ""}
        		${canUserRegister ? `<button id="tournament-join" class="retro-btn">Join tournament</button>` : ""}
        		${canUserLeave ? `<button id="tournament-leave" class="retro-btn">Leave tournament</button>` : ""}
      		`;

			const refreshBtn = document.querySelector<HTMLButtonElement>(
				"#tournament-refresh",
			);
			refreshBtn?.addEventListener("click", async () => {
				refreshBtn.disabled = true;
				refreshBtn.textContent = "Refreshing...";
				window.dispatchEvent(new Event("popstate"));
				refreshBtn.disabled = false;
				refreshBtn.textContent = "Refresh";
			});

			const startBtn =
				document.querySelector<HTMLButtonElement>("#tournament-start");
			startBtn?.addEventListener("click", async () => {
				startBtn.disabled = true;
				startBtn.textContent = "Starting...";
				const res = await tournamentService.startTournament(
					tournament.id,
				);
				if (!res.success) {
					startBtn.disabled = false;
					startBtn.textContent = "Start tournament";
					alert(res.error ?? "Failed to start tournament");
					return;
				}
				window.dispatchEvent(new Event("popstate"));
			});

			const registerBtn =
				document.querySelector<HTMLButtonElement>("#tournament-join");
			const modal =
				document.querySelector<HTMLDivElement>("#alias-modal");
			const backdrop =
				document.querySelector<HTMLDivElement>("#alias-backdrop");
			const cancelBtn =
				document.querySelector<HTMLButtonElement>("#alias-cancel");
			const confirmBtn =
				document.querySelector<HTMLButtonElement>("#alias-confirm");
			const aliasInput =
				document.querySelector<HTMLInputElement>("#alias-input");
			const aliasError =
				document.querySelector<HTMLDivElement>("#alias-error");

			const openModal = () => {
				if (!modal || !aliasInput) return;
				modal.classList.remove("hidden");
				modal.classList.add("flex");
				aliasInput.value = "";
				aliasInput.focus();
				if (aliasError) aliasError.textContent = "";
			};

			const closeModal = () => {
				if (!modal) return;
				modal.classList.add("hidden");
				modal.classList.remove("flex");
			};

			backdrop?.addEventListener("click", closeModal);
			cancelBtn?.addEventListener("click", (e) => {
				e.preventDefault();
				closeModal();
			});

			registerBtn?.addEventListener("click", async () => {
				if (!user) return;
				openModal();
			});

			confirmBtn?.addEventListener("click", async (e) => {
				e.preventDefault();
				if (!user || !aliasInput || !confirmBtn) return;
				const alias = aliasInput.value.trim();
				if (!alias) {
					if (aliasError)
						aliasError.textContent = "Please enter an alias";
					return;
				}
				if (alias.length > 20) {
					if (aliasError)
						aliasError.textContent =
							"Alias is too long (max 20 characters)";
					return;
				}

				confirmBtn.disabled = true;
				confirmBtn.textContent = "Joining...";

				const res = await tournamentService.addParticipant({
					tournamentId: id,
					userId: user.id,
					alias,
				});

				confirmBtn.disabled = false;
				confirmBtn.textContent = "Join";

				if (!res.success) {
					if (aliasError)
						aliasError.textContent =
							res.error ?? "Failed to join tournament";
					return;
				}

				closeModal();
				window.dispatchEvent(new Event("popstate"));
			});

			const leaveBtn =
				document.querySelector<HTMLButtonElement>("#tournament-leave");
			leaveBtn?.addEventListener("click", async () => {
				if (user) {
					leaveBtn.disabled = true;
					leaveBtn.textContent = "Leaving...";
					const res = await tournamentService.removeParticipant(
						id,
						user.id,
					);
					if (!res.success) {
						leaveBtn.disabled = false;
						alert("Failed to leave tournament");
						return;
					}
					window.dispatchEvent(new Event("popstate"));
				}
			});
		}
		if (isInProgress) {
			if (!ranking.length) {
				rankingEl.innerHTML = `<p class="text-sm text-[var(--muted)]">No ranking yet.</p>`;
			} else {
				rankingEl.innerHTML = `
        			<div class="overflow-auto">
        			  <table class="w-full text-left text-sm">
            			<thead>
            			  <tr class="text-xs uppercase tracking-[0.18em] text-[var(--muted)]">
            			    <th class="py-2 pr-3">#</th>
            			    <th class="py-2 pr-3">Player</th>
            			    <th class="py-2 pr-3">W</th>
            			    <th class="py-2 pr-3">L</th>
            			    <th class="py-2 pr-3">Diff</th>
            			    <th class="py-2 pr-3">GP</th>
            			  </tr>
            			</thead>
            		<tbody>
              ${ranking
					.sort((a, b) => a.position - b.position)
					.map(
						(r) => `
                    <tr class="border-t border-[rgba(230,240,255,0.12)]">
                      <td class="py-2 pr-3 font-bold text-[var(--fg)]">${
							r.position
						}</td>
                      <td class="py-2 pr-3 text-[var(--fg)]">${escapeHtml(
							r.alias,
						)}</td>
                      <td class="py-2 pr-3 text-[var(--fg)]">${r.wins}</td>
                      <td class="py-2 pr-3 text-[var(--fg)]">${r.losses}</td>
                      <td class="py-2 pr-3 text-[var(--fg)]">${
							r.points_diff
						}</td>
                      <td class="py-2 pr-3 text-[var(--fg)]">${
							r.wins + r.losses
						}</td>
                    </tr>
                  `,
					)
					.join("")}
            </tbody>
          </table>
        </div>
      `;
			}

			if (!nextMatch) {
				nextMatchEl.innerHTML = `<p class="text-sm text-[var(--muted)]">No next match available.</p>`;
			} else {
				nextMatchEl.innerHTML = `
        <div class="space-y-2">
          <p class="text-base font-bold text-[var(--fg)]">${formatPlayers(
				nextMatch,
				participants,
			)}</p>
          <p class="text-xs text-[var(--muted)]">Round: ${
				nextMatch.round_number ?? "-"
			} • Match: ${nextMatch.match_number ?? "-"}</p>
        </div>
      `;
				if (nextMatchActionsEl) nextMatchActionsEl.innerHTML = "";

				if (user && nextMatch && Array.isArray(nextMatch.players)) {
					const isUserInNextMatch = nextMatch.players.some(
						(p) => p?.user_id === user.id,
					);
					if (
						isUserInNextMatch &&
						tournament.status === "in_progress"
					) {
						nextMatchActionsEl!.innerHTML = `
            <a data-link href="/pong/${nextMatch.id}" class="retro-btn inline-flex justify-center">Join game</a>
            `;
					}
				}
			}

			const played = matches.filter((m) => m.winner_id != null);
			const upcoming = matches.filter((m) => m.winner_id == null);

			playedEl.innerHTML = played.length
				? `<div class="space-y-2">
          		${played
					.map(
						(m) => `
                			<div class="rounded-md border border-[rgba(230,240,255,0.12)] bg-[rgba(230,240,255,0.04)] p-3">
                			  <p class="text-sm font-semibold text-[var(--fg)]">${formatPlayers(m, participants)}</p>
                  <p class="text-xs text-[var(--muted)]">Round: ${
						m.round_number ?? "-"
					} • Match: ${m.match_number ?? "-"}</p>
                </div>
              `,
					)
					.join("")}
        </div>`
				: `<p class="text-sm text-[var(--muted)]">No games played yet.</p>`;

			upcomingEl.innerHTML = upcoming.length
				? `<div class="space-y-2">
          ${upcoming
				.map(
					(m) => `
                <div class="rounded-md border border-[rgba(230,240,255,0.12)] bg-[rgba(230,240,255,0.04)] p-3">
                  <p class="text-sm font-semibold text-[var(--fg)]">${formatPlayers(
						m,
						participants,
					)}</p>
                  <p class="text-xs text-[var(--muted)]">Round: ${
						m.round_number ?? "-"
					} • Match: ${m.match_number ?? "-"}</p>
                </div>
              `,
				)
				.join("")}
        </div>`
				: `<p class="text-sm text-[var(--muted)]">No upcoming games.</p>`;
		}

		if (tournament.status === "finished" && winnerContainer && winnerEl) {
			winnerContainer.classList.remove("hidden");
			const winner = ranking.find((r) => r.position === 1);

			if (winner) {
				const winnerUser = usersById.get(
					participants.find((p) => p.alias === winner.alias)
						?.user_id ?? 0,
				);
				const avatarHtml = winnerUser
					? renderAvatarDisplay(winnerUser)
					: `<span class="text-lg font-bold text-[var(--fg)]">${winner.alias.slice(0, 2).toUpperCase()}</span>`;

				winnerEl.innerHTML = `
          			<div class="flex items-center gap-4 rounded-md border border-[rgba(255,215,0,0.3)] bg-[rgba(255,215,0,0.1)] p-4">
            			<div class="inline-flex h-16 w-16 items-center justify-center overflow-hidden rounded-full border-2 border-[rgba(255,215,0,0.5)] bg-[rgba(255,215,0,0.2)]">
            			  ${avatarHtml}
            			</div>
            			<div class="space-y-1">
            			  <p class="text-xl font-bold text-[var(--fg)]">🏆 ${escapeHtml(winner.alias)}</p>
            			  <p class="text-sm text-[var(--muted)]">${winner.wins} wins • ${winner.losses} losses</p>
            			</div>
          			</div>
        			`;
			} else {
				winnerEl.innerHTML = `<p class="text-sm text-[var(--muted)]">Winner information not available.</p>`;
			}
		} else if (winnerContainer) {
			winnerContainer.classList.add("hidden");
		}
	};

	return { html, onMount };
};

export default TournamentPage;
