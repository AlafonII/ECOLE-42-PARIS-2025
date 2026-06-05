import friendsTitle from "../assets/images/page_titles/FRIENDS_PAGE.png";
import { getUserByUsername } from "../services/api";
import {
	sendFriendRequest,
	getFriendsList,
	removeFriend,
	getReceivedFriendRequests,
	acceptFriendRequest,
	rejectFriendRequest,
	type Friend,
	type FriendRequest,
} from "../services/friends.service";
import { presenceService } from "../services/presence.service";

const Friends = () => {
	const html = `
	<section class="retro-card crt space-y-6 p-8">
		<div class="space-y-2 text-center text-[var(--fg)]">
			<div class="flex justify-center">
				<img src="${friendsTitle}" alt="Friends" class="mx-auto h-auto max-w-full" />
			</div>
		</div>

		<div class="flex flex-col gap-6">
			<div class="pixel-frame pixel-frame--clear p-6 space-y-4">
			<p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">
			Your circle
			</p>

				<div class="flex flex-wrap items-end gap-2 border-b border-[rgba(230,240,255,0.18)]">
					<button id="tab-friends" class="tab-btn tab-active">Friends</button>
					<button id="tab-requests" class="tab-btn tab-inactive">Requests</button>
					<button id="tab-add" class="tab-btn tab-inactive">Add Friend</button>
				</div>

				<div id="friends-list-content" class="tab-content">
					<div class="text-center text-[var(--muted)] py-4">Loading...</div>
				</div>

				<div id="friend-requests-content" class="tab-content hidden"></div>

				<div id="friend-add-content" class="tab-content hidden">
					<div class="space-y-4">
						<div class="flex flex-col gap-3 sm:flex-row sm:items-center">
							<input
							id="friend-search-input"
							type="text"
							placeholder="Enter username..."
							class="w-full rounded-lg border border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.03)] px-3 py-2 text-[var(--fg)] sm:flex-1"
							/>
							<button
							id="search-user-button"
							class="retro-btn px-6 py-3 sm:w-auto"
							>
								Search
							</button>
						</div>
						<div id="search-result" class="space-y-2"></div>
					</div>
				</div>
			</div>
		</div>

		<div id="confirm-modal" class="fixed inset-0 z-50 hidden flex items-center justify-center bg-black/70 px-4">
			<div class="pixel-frame pixel-frame--clear w-full max-w-md space-y-4 p-6">
				<h3 id="confirm-title" class="text-base font-semibold text-[var(--fg)]">
					Confirm
				</h3>
				<p id="confirm-message" class="text-sm text-[var(--fg)]"></p>
				<div class="flex flex-col gap-3 sm:flex-row sm:justify-end">
					<button id="confirm-cancel" class="retro-btn">Cancel</button>
					<button id="confirm-ok" class="retro-btn bg-[rgba(255,114,114,0.8)] hover:bg-[rgba(255,114,114,1)]">
					Confirm
					</button>
				</div>
			</div>
		</div>
	</section>
	`;

	const onMount = () => {
		/* ---------------- Tabs ---------------- */
		const tabFriends = document.getElementById("tab-friends")!;
		const tabRequests = document.getElementById("tab-requests")!;
		const tabAdd = document.getElementById("tab-add")!;

		const friendsContent = document.getElementById("friends-list-content")!;
		const requestsContent = document.getElementById(
			"friend-requests-content"
		)!;
		const addContent = document.getElementById("friend-add-content")!;

		type Tab = "friends" | "requests" | "add";
		const tabs: Record<Tab, HTMLElement> = {
			friends: friendsContent,
			requests: requestsContent,
			add: addContent,
		};

		const buttons: Record<Tab, HTMLButtonElement> = {
			friends: tabFriends,
			requests: tabRequests,
			add: tabAdd,
		};

		const setActiveTab = (tab: Tab) => {
			(Object.keys(tabs) as Tab[]).forEach((key) => {
				tabs[key].classList.toggle("hidden", key !== tab);
				buttons[key].classList.toggle("tab-active", key === tab);
				buttons[key].classList.toggle("tab-inactive", key !== tab);
			});
		};

		tabFriends.onclick = () => setActiveTab("friends");
		tabRequests.onclick = () => setActiveTab("requests");
		tabAdd.onclick = () => setActiveTab("add");

		setActiveTab("friends");

		/* ---------------- Confirm Modal ---------------- */
		const modal = document.getElementById("confirm-modal")!;
		const confirmTitle = document.getElementById("confirm-title")!;
		const confirmMessage = document.getElementById("confirm-message")!;
		const confirmOk = document.getElementById("confirm-ok")!;
		const confirmCancel = document.getElementById("confirm-cancel")!;

		const showConfirm = (message: string, title = "Confirm") =>
			new Promise<boolean>((resolve) => {
				confirmTitle.textContent = title;
				confirmMessage.textContent = message;
				modal.classList.remove("hidden");

				const cleanup = () => {
					confirmOk.onclick = null;
					confirmCancel.onclick = null;
				};

				confirmOk.onclick = () => {
					modal.classList.add("hidden");
					cleanup();
					resolve(true);
				};

				confirmCancel.onclick = () => {
					modal.classList.add("hidden");
					cleanup();
					resolve(false);
				};
			});

		/* ---------------- Presence ---------------- */
		let friends: Friend[] = [];

		const unsubscribe = presenceService.subscribe((id, online) => {
			renderFriends();
		});

		/* ---------------- Friends ---------------- */
		async function loadFriends() {
			friendsContent.innerHTML = `<div class="text-center text-[var(--muted)] py-4">Loading...</div>`;
			const res = await getFriendsList();

			if (res.success && res.data?.friendsWithStatus) {
				friends = res.data.friendsWithStatus;
				renderFriends();
			} else {
				friendsContent.innerHTML = `<div class="text-center text-[var(--error)] py-4">Failed to load friends</div>`;
			}
		}

		function renderFriends() {
			if (!friends.length) {
				friendsContent.innerHTML = `<div class="text-center text-[var(--muted)] py-4">No friends yet</div>`;
				return;
			}

			const sorted = [...friends].sort((a, b) => {
				const ao = presenceService.isOnline(a.id);
				const bo = presenceService.isOnline(b.id);
				if (ao !== bo) return ao ? -1 : 1;
				return a.username.localeCompare(b.username);
			});

			friendsContent.innerHTML = sorted
				.map(
					(f) => `
				<div class="pixel-frame bg-[var(--bg)] p-3 flex items-center justify-between mb-2">
				<div class="flex items-center gap-3">
				<span class="w-2 h-2 rounded-full ${
					presenceService.isOnline(f.id)
						? "bg-green-500"
						: "bg-gray-500"
				}"></span>
				<span class="font-bold text-[var(--fg)]">${f.username}</span>
				</div>
				<button data-id="${f.id}" data-name="${
						f.username
					}" class="retro-btn bg-[rgba(255,114,114,0.8)]">
				Remove
				</button>
				</div>`
				)
				.join("");

			friendsContent.querySelectorAll("button").forEach((btn) => {
				btn.addEventListener("click", async () => {
					const id = Number(btn.dataset.id);
					const name = btn.dataset.name!;
					if (await showConfirm(`Remove ${name}?`, "Remove Friend")) {
						await removeFriend(id);
						loadFriends();
					}
				});
			});
		}

		/* ---------------- Requests ---------------- */
		async function loadRequests() {
			const res = await getReceivedFriendRequests();
			if (!res.success || !res.data?.requests?.length) {
				requestsContent.innerHTML = `<div class="text-center text-[var(--muted)] py-4">No requests</div>`;
				return;
			}

			requestsContent.innerHTML = res.data.requests
				.map(
					(r) => `
				<div class="pixel-frame bg-[var(--bg)] p-3 flex justify-between mb-2">
				<span class="font-bold text-[var(--fg)]">${r.username}</span>
				<div class="flex gap-2">
				<button data-a="accept" data-id="${r.sender_id}" class="retro-btn">Accept</button>
				<button data-a="reject" data-id="${r.sender_id}" class="retro-btn bg-[rgba(255,114,114,0.8)]">Reject</button>
				</div>
				</div>`
				)
				.join("");

			requestsContent.querySelectorAll("button").forEach((btn) => {
				btn.addEventListener("click", async () => {
					const id = Number(btn.dataset.id);
					btn.dataset.a === "accept"
						? await acceptFriendRequest(id)
						: await rejectFriendRequest(id);
					await loadRequests();
					await loadFriends();
				});
			});
		}

		/* ---------------- Add Friend ---------------- */
		const input = document.getElementById(
			"friend-search-input"
		) as HTMLInputElement;
		const searchBtn = document.getElementById("search-user-button")!;
		const result = document.getElementById("search-result")!;

		searchBtn.onclick = async () => {
			const name = input.value.trim();
			result.innerHTML = "";

			if (name.length !== 4) {
				result.innerHTML = `<div class="text-[var(--error)]">Username must be 4 characters</div>`;
				return;
			}

			const res = await getUserByUsername(name);
			if (!res.success || !res.data) {
				result.innerHTML = `<div class="text-[var(--error)]">User not found</div>`;
				return;
			}

			result.innerHTML = `
			<div class="pixel-frame bg-[var(--bg)] p-3 flex justify-between">
			<span class="font-bold text-[var(--fg)]">${res.data.username}</span>
			<button id="add-btn" class="retro-btn">Add</button>
			</div>
			`;

			document.getElementById("add-btn")!.onclick = async () => {
				const addResult = await sendFriendRequest(res.data.id);
				if (addResult.success) {
					result.innerHTML = `<div class="text-[var(--success)]">Request sent!</div>`;
				} else {
					result.innerHTML = `<div class="text-[var(--error)]">${
						addResult.error || "Failed to send request"
					}</div>`;
				}
			};
		};

		loadFriends();
		loadRequests();

		return () => unsubscribe();
	};

	return { html, onMount };
};

export default Friends;
