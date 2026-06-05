import pongTitle from "../assets/images/page_titles/PONG_PAGE.png";
import easyBadge from "../assets/images/page_assets/EASY.png";
import mediumBadge from "../assets/images/page_assets/MEDIUM.png";
import hardBadge from "../assets/images/page_assets/HARD.png";
import arrowBack from "../assets/images/page_assets/ARROW_BACK.png";
import { sessionService } from "../services/session.service";
import {
	closePongConnection,
	initPongClient,
	sendPongForfeit,
} from "../game/PongClient";
import { initPongGame } from "../game/PongGameLogic";
import { matchService } from "../services/match.service";

type aiDifficulty = "easy" | "medium" | "hard";
const PONG_AUTOSTART_KEY = "pong:autostart";

export function getMatchIdFromPath(): number | null {
	const parts = window.location.pathname.split("/").filter(Boolean);
	if (parts[0] !== "pong" || !parts[1]) return null;
	const id = Number(parts[1]);
	return Number.isFinite(id) ? id : null;
}

function navigate(path: string) {
	if (window.location.pathname !== path) history.pushState({}, "", path);
	window.dispatchEvent(new PopStateEvent("popstate"));
}

function setStatus(msg: string) {
	const el = document.getElementById("pong-status");
	if (el) el.textContent = msg;
}

function setButtonsEnabled(enabled: boolean) {
	const btnLocal = document.querySelector<HTMLButtonElement>("#pongLocal");
	const btnAIOpen = document.querySelector<HTMLButtonElement>("#ai-open");
	const btnAIChange = document.querySelector<HTMLButtonElement>("#ai-change");
	const btnAIOff = document.querySelector<HTMLButtonElement>("#ai-off");
	if (btnLocal) btnLocal.disabled = !enabled;
	if (btnAIOpen) btnAIOpen.disabled = !enabled;
	if (btnAIChange) btnAIChange.disabled = !enabled;
	if (btnAIOff) btnAIOff.disabled = !enabled;
}

function showCanvasAndHideButtons() {
	const btnWrap = document.getElementById("pong-buttons");
	const aiStartRow = document.getElementById("ai-start-row");
	const aiControls = document.getElementById("ai-controls");
	const canvas = document.getElementById(
		"gameCanvas"
	) as HTMLCanvasElement | null;
	const localControls = document.getElementById("local-controls");
	if (btnWrap) {
		btnWrap.hidden = true;
		btnWrap.style.display = "none";
	}
	if (aiStartRow) {
		aiStartRow.hidden = true;
		aiStartRow.style.display = "none";
	}
	if (aiControls) {
		aiControls.hidden = true;
		aiControls.classList.add("hidden");
		aiControls.style.display = "none";
	}
	if (canvas) {
		canvas.hidden = false;
		canvas.style.display = "block";
	}
	if (localControls) {
		localControls.hidden = false;
		localControls.style.display = "";
	}
}

function hideCanvasAndShowButtons() {
	const btnWrap = document.getElementById("pong-buttons");
	const aiStartRow = document.getElementById("ai-start-row");
	const aiControls = document.getElementById("ai-controls");
	const canvas = document.getElementById(
		"gameCanvas"
	) as HTMLCanvasElement | null;
	const localControls = document.getElementById("local-controls");
	if (btnWrap) {
		btnWrap.hidden = false;
		btnWrap.style.display = "";
	}
	if (aiStartRow) {
		aiStartRow.hidden = false;
		aiStartRow.style.display = "";
	}
	if (aiControls) {
		aiControls.hidden = true;
		aiControls.classList.add("hidden");
		aiControls.style.display = "none";
	}
	if (canvas) {
		canvas.hidden = true;
		canvas.style.display = "none";
	}
	if (localControls) {
		localControls.hidden = true;
		localControls.style.display = "none";
	}
}

function readAutoStart():
	| { mode: "local"; ai: false }
	| { mode: "local"; ai: true; difficulty: aiDifficulty }
	| { mode: "remote" }
	| null {
	try {
		const raw = sessionStorage.getItem(PONG_AUTOSTART_KEY);
		return raw ? JSON.parse(raw) : null;
	} catch {
		return null;
	}
}

function clearAutoStart() {
	sessionStorage.removeItem(PONG_AUTOSTART_KEY);
}

function writeAutoStart(
	payload:
		| { mode: "local"; ai: false }
		| { mode: "local"; ai: true; difficulty: aiDifficulty }
		| { mode: "remote" }
) {
	sessionStorage.setItem(PONG_AUTOSTART_KEY, JSON.stringify(payload));
}

function dispatchAi(enabled: boolean, difficulty?: aiDifficulty) {
	window.dispatchEvent(
		new CustomEvent("pong:ai", {
			detail: enabled
				? { enabled: true, difficulty }
				: { enabled: false },
		})
	);
}

const PongGame = () => {
	const showBackLink = !sessionService.isLoggedIn();
	const html = `
    <section class="retro-card crt space-y-4 p-0">
      ${
			showBackLink
				? `
        <a
          id="pong-back"
          data-link
          href="/"
          class="fixed left-6 top-6 z-20 flex items-center gap-3 text-base font-semibold uppercase text-[var(--fg)]"
          aria-label="Back"
        >
          <img src="${arrowBack}" alt="" class="h-7 w-auto" />
          <span>Back</span>
        </a>
      `
				: ""
		}
      <div class="space-y-2 px-8 pt-8 text-center text-[var(--fg)]">
        <div class="flex justify-center">
          <img src="${pongTitle}" alt="Pong Game" class="mx-auto h-auto max-w-full" />
        </div>
      </div>
      <div id="pong-status" class="px-8 text-center text-sm text-[var(--muted)]"></div>
      <div class="space-y-2 text-center text-sm text-[var(--muted)]">
       <div class="px-8">
  			<div id="pong-buttons" class="flex justify-center gap-4 mb-4">
   				<button id="pongLocal" type="button" class="retro-btn retro-btn--lg">
				LOCAL MODE
    			</button>
  		  </div>
		  <div id="ai-start-row" class="flex justify-center px-8 pb-6 pt-3">
          	<button id="ai-open" type="button" class="retro-btn retro-btn--lg">
           		PLAY WITH AI
          	</button>
        	</div>
			<div class="mx-auto w-full max-w-[800px] px-8">
  				<canvas
				    id="gameCanvas"
    				width="800"
    				height="500"
    				class="block w-full bg-black shadow-[inset_0_0_30px_rgba(0,0,0,0.5)]"
    				hidden
				></canvas>
				<div id="local-controls" class="mt-2 flex items-center justify-between text-sm text-[var(--muted)]">
    				<span>W/S</span>
    				<span>UP/DOWN</span>
				</div>
			</div>
			<div id="pong-actions" class="mt-4 flex flex-wrap justify-center gap-4" hidden>
  				<button id="pong-restart" type="button" class="retro-btn retro-btn--lg" hidden>
    				RESTART
  				</button>
				<button id="pong-forfeit-you" type="button" class="retro-btn retro-btn--lg" hidden>
    				FORFEIT (0-5)
  				</button>
				<button id="pong-back-tournament" type="button" class="retro-btn retro-btn--lg" hidden>
    				BACK TO TOURNAMENT
  				</button>
			</div>
		</div>
        <div
          id="ai-controls"
          class="hidden flex flex-wrap items-center justify-center gap-3 px-8 pb-6 pt-3"
        >
          <button id="ai-change" type="button" class="retro-btn retro-btn--lg">
            CHANGE DIFFICULTY
          </button>
          <button id="ai-off" type="button" class="retro-btn retro-btn--lg">
            PLAY 1v1
          </button>
        </div>
      </div>
    </section>
    <div
      id="ai-modal"
      class="fixed inset-0 z-20 hidden flex items-center justify-center bg-black/70 px-4"
      aria-hidden="true"
    >
      <div class="pixel-frame w-full max-w-sm space-y-4 p-6 text-center text-[var(--color-body)] shadow-[0_18px_60px_rgba(0,0,0,0.6)]">
        <h2 class="header-font-sm">CHOOSE DIFFICULTY</h2>
        <div class="space-y-3">
          <button
            type="button"
            class="retro-btn retro-btn--easy w-full"
            data-ai-difficulty="easy"
          >
            <span>EASY</span>
            <img src="${easyBadge}" alt="Easy" class="h-6 w-6" />
          </button>
          <button
            type="button"
            class="retro-btn retro-btn--medium w-full"
            data-ai-difficulty="medium"
          >
            <span>MEDIUM</span>
            <img src="${mediumBadge}" alt="Medium" class="h-6 w-6" />
          </button>
          <button
            type="button"
            class="retro-btn retro-btn--hard w-full"
            data-ai-difficulty="hard"
          >
            <span>HARD</span>
            <img src="${hardBadge}" alt="Hard" class="h-6 w-6" />
          </button>
        </div>
      </div>
    </div>
  `;

	const onMount = async () => {
		const btnLocal =
			document.querySelector<HTMLButtonElement>("#pongLocal");
		const openButton =
			document.querySelector<HTMLButtonElement>("#ai-open");
		const modal = document.querySelector<HTMLDivElement>("#ai-modal");
		const aiControls =
			document.querySelector<HTMLDivElement>("#ai-controls");
		const aiStartRow =
			document.querySelector<HTMLDivElement>("#ai-start-row");
		const changeButton =
			document.querySelector<HTMLButtonElement>("#ai-change");
		const offButton = document.querySelector<HTMLButtonElement>("#ai-off");
		const difficultyButtons = document.querySelectorAll<HTMLButtonElement>(
			"[data-ai-difficulty]"
		);
		const actionsRow =
			document.querySelector<HTMLDivElement>("#pong-actions");
		const restartBtn =
			document.querySelector<HTMLButtonElement>("#pong-restart");
		const forfeitYouBtn =
			document.querySelector<HTMLButtonElement>("#pong-forfeit-you");
		const backTournamentBtn = document.querySelector<HTMLButtonElement>(
			"#pong-back-tournament"
		);
		const localControls =
			document.querySelector<HTMLDivElement>("#local-controls");
		const backLink =
			document.querySelector<HTMLAnchorElement>("#pong-back");

		if (backLink) {
			const backFallback = backLink.getAttribute("href") ?? "/";
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
			backLink.addEventListener("click", handleBackClick);
		}

		const hideActions = () => {
			if (!actionsRow) return;
			actionsRow.hidden = true;
			actionsRow.style.display = "none";
			if (restartBtn) {
				restartBtn.hidden = true;
				restartBtn.style.display = "none";
			}
			if (forfeitYouBtn) {
				forfeitYouBtn.hidden = true;
				forfeitYouBtn.style.display = "none";
			}
			if (backTournamentBtn) {
				backTournamentBtn.hidden = true;
				backTournamentBtn.style.display = "none";
			}
		};

		const showActions = (opts: {
			restart?: boolean;
			forfeitYou?: boolean;
			backTournament?: boolean;
		}) => {
			if (!actionsRow) return;
			const shouldShow =
				opts.restart || opts.forfeitYou || opts.backTournament;
			if (!shouldShow) {
				hideActions();
				return;
			}
			actionsRow.hidden = false;
			actionsRow.style.display = "";
			if (restartBtn) {
				restartBtn.hidden = !opts.restart;
				restartBtn.style.display = opts.restart ? "" : "none";
			}
			if (forfeitYouBtn) {
				forfeitYouBtn.hidden = !opts.forfeitYou;
				forfeitYouBtn.style.display = opts.forfeitYou ? "" : "none";
			}
			if (backTournamentBtn) {
				backTournamentBtn.hidden = !opts.backTournament;
				backTournamentBtn.style.display = opts.backTournament
					? ""
					: "none";
			}
		};

		const setIdleView = () => {
			hideCanvasAndShowButtons();
			hideActions();
			setButtonsEnabled(true);
			setStatus("");
		};

		if (!btnLocal || !openButton || !modal) return;
		setIdleView();

		const matchId = getMatchIdFromPath();
		const user = sessionService.loadUser();
		const isLoggedIn = !!user;

		let currentGameCleanup: (() => void) | null = null;
		let isGameActive = false;
		let aiEnabled = false;

		type LocalMode =
			| { kind: "local1v1" }
			| { kind: "ai"; difficulty: aiDifficulty };
		let lastOfflineLocalMode: LocalMode | null = null;
		let isRegisteredMatch = false;
		let tournamentId: number | null = null;

		const stopCurrentGame = () => {
			if (currentGameCleanup) {
				currentGameCleanup();
				currentGameCleanup = null;
			}
			closePongConnection();
			isGameActive = false;
			hideCanvasAndShowButtons();
			hideActions();
			setButtonsEnabled(true);
		};

		const startGame = (cleanup: () => void) => {
			stopCurrentGame();
			currentGameCleanup = cleanup;
			isGameActive = true;
			setButtonsEnabled(false);
			showCanvasAndHideButtons();
		};

		const setAiEnabled = (enabled: boolean) => {
			aiEnabled = enabled;
			if (aiControls) {
				aiControls.hidden = true;
				aiControls.classList.add("hidden");
				aiControls.style.display = "none";
			}

			if (aiStartRow) {
				aiStartRow.hidden = enabled;
				aiStartRow.classList.toggle("hidden", enabled);
				aiStartRow.style.display = enabled ? "none" : "";
			}
		};

		const openModal = () => {
			modal.classList.remove("hidden");
			modal.setAttribute("aria-hidden", "false");
		};

		const closeModal = () => {
			modal.classList.add("hidden");
			modal.setAttribute("aria-hidden", "true");
		};

		hideActions();

		const startOfflineLocal1v1 = () => {
			setStatus("");
			setAiEnabled(false);
			isRegisteredMatch = false;
			lastOfflineLocalMode = { kind: "local1v1" };

			const cleanup = initPongGame();
			startGame(cleanup);
			hideActions();
			showActions({ restart: true });
			dispatchAi(false);
		};

		const startOfflineLocalAIGame = (diffculty: aiDifficulty) => {
			setStatus("");
			setAiEnabled(true);
			isRegisteredMatch = false;
			lastOfflineLocalMode = { kind: "ai", difficulty: diffculty };

			const cleanup = initPongGame();
			startGame(cleanup);
			hideActions();
			showActions({ restart: true });
			dispatchAi(true, diffculty);
		};

		const startRemoteGame = () => {
			setStatus("Connecting to game server...");
			setAiEnabled(false);
			dispatchAi(false);
			initPongClient();
			isGameActive = true;
			setButtonsEnabled(false);
			showCanvasAndHideButtons();
			showActions({ forfeitYou: true });
		};

		openButton.addEventListener("click", openModal);
		changeButton?.addEventListener("click", openModal);
		offButton?.addEventListener("click", () => {
			setAiEnabled(false);
			dispatchAi(false);
		});

		modal.addEventListener("click", (event) => {
			if (event.target === modal) {
				closeModal();
			}
		});

		btnLocal.addEventListener("click", () => {
			startOfflineLocal1v1();
		});

		restartBtn?.addEventListener("click", () => {
			if (!lastOfflineLocalMode) return;

			if (lastOfflineLocalMode.kind === "local1v1")
				startOfflineLocal1v1();
			else startOfflineLocalAIGame(lastOfflineLocalMode.difficulty);
		});

		forfeitYouBtn?.addEventListener("click", () => {
			const currentMatchId = getMatchIdFromPath();
			//console.log("Forfeit clicked, matchId:", currentMatchId);
			if (currentMatchId === null) {
				console.warn("Cannot forfeit: no match ID in URL");
				return;
			}
			sendPongForfeit();
		});

		backTournamentBtn?.addEventListener("click", () => {
			if (tournamentId !== null) {
				navigate(`/tournaments/${tournamentId}`);
			}
		});

		const onLocalGameOver = () => {
			setButtonsEnabled(true);

			const canRestart =
				!isRegisteredMatch && lastOfflineLocalMode !== null;
			showActions({ restart: canRestart });

			const btnWrap = document.getElementById("pong-buttons");

			if (btnWrap) {
				btnWrap.hidden = false;
				btnWrap.style.display = "";

				btnWrap.querySelectorAll("button").forEach((b) => {
					(b as HTMLButtonElement).hidden = true;
					(b as HTMLButtonElement).style.display = "none";
				});

				if (btnLocal) {
					btnLocal.hidden = false;
					btnLocal.style.display = "";
				}
			}

			if (aiControls) {
				if (lastOfflineLocalMode?.kind === "ai") {
					aiControls.classList.remove("hidden");
					aiControls.style.display = "";

					if (openButton) {
						openButton.hidden = true;
						openButton.style.display = "none";
					}
					if (offButton) {
						offButton.hidden = true;
						offButton.style.display = "none";
					}
					if (changeButton) {
						changeButton.hidden = false;
						changeButton.style.display = "";
					}
				} else {
					aiControls.classList.add("hidden");
					aiControls.style.display = "none";
				}
			}
		};

		window.addEventListener("pong:localGameOver", onLocalGameOver);

		const onRemoteGameOver = () => {
			setButtonsEnabled(true);
			showActions({ backTournament: tournamentId !== null });
		};

		window.addEventListener("pong:remoteGameOver", onRemoteGameOver);

		difficultyButtons.forEach((button) => {
			button.addEventListener("click", async () => {
				const diffculty =
					(button.dataset.aiDifficulty as aiDifficulty | undefined) ??
					"easy";
				closeModal();
				if (isGameActive) {
					setStatus("");
					setAiEnabled(true);
					isRegisteredMatch = false;
					lastOfflineLocalMode = {
						kind: "ai",
						difficulty: diffculty,
					};
					dispatchAi(true, diffculty);
					if (aiControls) {
						aiControls.classList.remove("hidden");
						aiControls.style.display = "";
					}
					return;
				}

				startOfflineLocalAIGame(diffculty);
			});
		});

		if (matchId !== null) {
			if (!isLoggedIn) {
				setStatus("You must be logged in to play a registered match");
				navigate("/pong");
				return;
			}

			setButtonsEnabled(false);
			setStatus("Loading match...");

			const res = await matchService.getMatchById(matchId);
			if (!res.success || !res.data) {
				setStatus(res.error || "Match not found");
				setButtonsEnabled(true);
				return;
			}

			const loadedMatch = res.data;
			if (
				loadedMatch.duration_seconds !== null &&
				loadedMatch.duration_seconds !== undefined
			) {
				setStatus("This match has already been played");
				setButtonsEnabled(true);
				return;
			}

			const players = Array.isArray(loadedMatch.players)
				? loadedMatch.players
				: [];
			const participantIds = players
				.map((p: any) => p?.user_id)
				.filter((id: any): id is number => typeof id === "number");

			const isParticipant = participantIds.includes(user!.id);
			if (!isParticipant) {
				setStatus("You are not a participant of this match");
				setButtonsEnabled(true);
				return;
			}

			tournamentId =
				typeof (loadedMatch as any).tournament_id === "number"
					? (loadedMatch as any).tournament_id
					: null;
			isRegisteredMatch = true;
			lastOfflineLocalMode = null;

			setStatus("");

			const auto = readAutoStart();
			if (auto) {
				clearAutoStart();

				if (auto.mode === "local") {
					if (auto.ai) {
						setAiEnabled(true);
					} else {
						setAiEnabled(false);
					}
					const cleanup = initPongGame();
					startGame(cleanup);
					if (auto.ai) {
						dispatchAi(true, auto.difficulty);
					} else {
						dispatchAi(false);
					}
					return;
				} else if (auto.mode === "remote") {
					startRemoteGame();
					return;
				}
			}

			setButtonsEnabled(true);
			startRemoteGame();
			return;
		}

		return () => {
			window.removeEventListener("pong:localGameOver", onLocalGameOver);
			window.removeEventListener("pong:remoteGameOver", onRemoteGameOver);
			stopCurrentGame();
			hideCanvasAndShowButtons();
		};
	};

	return { html, onMount };
};

export default PongGame;
