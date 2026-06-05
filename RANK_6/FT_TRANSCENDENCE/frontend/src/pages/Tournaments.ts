import tournamentsTitle from "../assets/images/page_titles/TOURNAMENT_PAGE.png";
import { sessionService } from "../services/session.service";
import { tournamentService } from "../services/tournament.service";
import { Tournament } from "../services/types";
import { escapeHtml } from "../utils/htmlhelpers";

export function statusBadge(status: string): string {
	const base =
		"retro-card inline-flex items-center border px-3 py-1 text-[11px] font-bold uppercase tracking-[0.18em]";
	switch (status) {
		case "registration":
			return `<span class="${base} border-[rgba(255,255,255,0.35)] bg-[rgba(255,255,255,0.08)] text-[var(--fg)]">Registration</span>`;
		case "in_progress":
			return `<span class="${base} border-[rgba(80,200,255,0.45)] bg-[rgba(80,200,255,0.12)] text-[var(--fg)]">In progress</span>`;
		case "finished":
			return `<span class="${base} border-[rgba(120,255,160,0.45)] bg-[rgba(120,255,160,0.12)] text-[var(--fg)]">Finished</span>`;
		default:
			return `<span class="${base} border-[rgba(255,200,80,0.45)] bg-[rgba(255,200,80,0.12)] text-[var(--fg)]">${status}</span>`;
	}
}

const Tournaments = () => {
	const html = `
    <section class="retro-card crt space-y-6 p-8">
      <div class="flex flex-col gap-4 sm:flex-row sm:items-center sm:justify-between">
        <div class="flex justify-center sm:justify-start">
          <img src="${tournamentsTitle}" alt="Tournaments" class="mx-auto h-auto max-w-full" />
        </div>
      </div>

      <div class="pixel-frame pixel-frame--clear p-6 space-y-4">
        <div class="flex flex-col gap-3 sm:flex-row sm:items-center sm:justify-between">
          <div class="space-y-1">
            <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">All tournaments</p>
            <p class="text-sm text-[var(--muted)]">Browse ongoing events, register, or jump into the bracket.</p>
          </div>
          <div class="flex items-center gap-2">
            <button id="tournaments-refresh" class="retro-btn">Refresh</button>
          </div>
        </div>

        <div id="tournaments-list" class="space-y-3 max-h-96 overflow-auto no-scrollbar">
          <p class="text-sm text-[var(--muted)]">Loading tournaments...</p>
        </div>
      </div>

      <div id="tournaments-create-wrapper" class="pixel-frame pixel-frame--clear p-6 space-y-4 hidden">
        <div class="space-y-1">
          <p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Create a tournament</p>
          <p class="text-sm text-[var(--muted)]">Any logged in player can create a tournament.</p>
        </div>

        <form id="tournaments-create-form" class="grid grid-cols-1 gap-3 sm:grid-cols-3">
          <label class="space-y-1 sm:col-span-2">
            <span class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Name</span>
            <input
              id="tournament-name"
              class="w-full rounded-md border border-[rgba(230,240,255,0.25)] bg-[rgba(10,20,30,0.35)] px-3 py-2 text-[var(--fg)] placeholder:text-[rgba(230,240,255,0.45)]"
              placeholder="e.g. Winter Cup"
              required
            />
          </label>

          <label class="space-y-1">
            <span class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">Max players</span>
            <input
              id="tournament-max"
              type="number"
              min="2"
              max="64"
              value="8"
              class="w-full rounded-md border border-[rgba(230,240,255,0.25)] bg-[rgba(10,20,30,0.35)] px-3 py-2 text-[var(--fg)]"
              required
            />
          </label>

          <div class="flex items-end">
            <button type="submit" class="retro-btn w-full justify-center">Create</button>
          </div>
        </form>

        <div id="tournaments-create-msg" class="text-sm text-[var(--muted)]"></div>
      </div>

      <div id="tournaments-login-hint" class="pixel-frame pixel-frame--clear p-6 hidden">
        <div class="space-y-2 text-center">
          <p class="text-base font-semibold text-[var(--fg)]">Want to create a tournament?</p>
          <p class="text-sm text-[var(--muted)]">Log in to host your own bracket.</p>
          <a data-link href="/login" class="retro-btn justify-center">Go to Login</a>
        </div>
      </div>
    </section>
  `;

	const onMount = () => {
		const listEl =
			document.querySelector<HTMLDivElement>("#tournaments-list");
		const refreshBtn = document.querySelector<HTMLDivElement>(
			"#tournaments-refresh",
		);
		const createWrapper = document.querySelector<HTMLDivElement>(
			"#tournaments-create-wrapper",
		);
		const loginHint = document.querySelector<HTMLDivElement>(
			"#tournaments-login-hint",
		);
		const createForm = document.querySelector<HTMLDivElement>(
			"#tournaments-create-form",
		);
		const createMsg = document.querySelector<HTMLDivElement>(
			"#tournaments-create-msg",
		);

		if (!listEl || !refreshBtn) return;

		const user = sessionService.loadUser();
		if (user) {
			createWrapper?.classList.remove("hidden");
			loginHint?.classList.add("hidden");
		} else {
			createWrapper?.classList.add("hidden");
			loginHint?.classList.remove("hidden");
		}

		const render = (tournaments: Tournament[]) => {
			if (!tournaments.length) {
				listEl.innerHTML = `
          <div class="text-center space-y-2">
              <p class="text-base font-semibold text-[var(--fg)]">No tournaments yet</p>
             <p class="text-sm text-[var(--muted)]">Create one to get started.</p>
          </div>
          `;
				return;
			}

			listEl.innerHTML = tournaments
				.sort((a, b) => b.id - a.id)
				.map((t) => {
					return `
            <a data-link href="/tournaments/${t.id}" class="block">
              <div class="pixel-frame pixel-frame--clear p-4 transition hover:translate-y-[-1px] hover:bg-[rgba(230,240,255,0.05)]">
                <div class="flex flex-col gap-3 sm:flex-row sm:items-center sm:justify-between">
                  <div class="space-y-1">
                    <p class="text-base font-bold text-[var(--fg)]">${escapeHtml(
						t.name,
					)}</p>
                    <p class="text-xs text-[var(--muted)]">
                      ID #${t.id} • Max players: ${t.max_participants}
                    </p>
                  </div>
                  <div class="flex items-center justify-between gap-3 sm:justify-end">
                    ${statusBadge(t.status)}
                    <span class="retro-btn px-4 py-2">Open</span>
                  </div>
                </div>
              </div>
            </a>
          `;
				})
				.join("");
		};

		const load = async () => {
			listEl.innerHTML = `<p class="text-sm text-[var(--muted)]">Loading tournaments...</p>`;
			const res = await tournamentService.getAllTournaments();
			if (!res.success || !res.data) {
				listEl.innerHTML = `
           <div class="space-y-2">
            <p class="text-base font-semibold text-[var(--fg)]">Failed to load tournaments</p>
            <p class="text-sm text-[var(--muted)]">${escapeHtml(
				res.error ?? "Unknown error",
			)}</p>
          </div>
        `;
				return;
			}
			render(res.data);
		};

		refreshBtn.addEventListener("click", load);

		if (createForm && createMsg) {
			createForm.addEventListener("submit", async (e) => {
				e.preventDefault();
				const currentUser = sessionService.loadUser();
				if (!currentUser) {
					createMsg.textContent =
						"You must be loggeed in to create a tournament";
					return;
				}

				const nameInput =
					document.querySelector<HTMLInputElement>(
						"#tournament-name",
					);
				const maxInput =
					document.querySelector<HTMLInputElement>("#tournament-max");
				const name = nameInput?.value.trim() ?? "";
				const participantsNb = Number(maxInput?.value ?? 0);
				const gameTypeId = 1;

				if (
					!name ||
					!Number.isFinite(participantsNb) ||
					participantsNb < 2
				) {
					createMsg.textContent =
						"Please provide a valid name and at least 2 max players";
					return;
				}

				createMsg.textContent = "Creating tournament...";
				const createRes = await tournamentService.createTournament({
					name,
					gameTypeId,
					participantsNb,
					organiserId: currentUser.id,
				});

				if (!createRes.success || !createRes.data) {
					createMsg.textContent =
						createRes.error ?? "Failed to create tournament";
					return;
				}

				createMsg.textContent = "Tournament created !";
				window.history.pushState(
					{},
					"",
					`/tournaments/${createRes.data}`,
				);
				window.dispatchEvent(new Event("popstate"));
			});
		}
		load();
	};
	return { html, onMount };
};
export default Tournaments;
