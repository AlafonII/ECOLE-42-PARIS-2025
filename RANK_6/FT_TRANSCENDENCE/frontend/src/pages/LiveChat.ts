import liveChatTitle from "../assets/images/page_titles/LIVECHAT_PAGE.png";
import dashboardIcon from "../assets/images/page_assets/DASHBOARD.png";
import { getRelationsList, type Friend } from "../services/friends.service";
import { ChatService } from "../services/chat.service";
import { sessionService } from "../services/session.service";
import { userService } from "../services/user.service";
import { matchService } from "../services/match.service";
import { presenceService } from "../services/presence.service";
import {
	connectChat,
	disconnectChat,
	sendDirectMessage,
	sendBroadcast,
} from "../services/chat.ws.ts";
import { resolveAvatarUrl } from "../utils/avatar";
import { tournamentService } from "../services/tournament.service";

type ChatMessage = {
	sender: "me" | "friend";
	content: string;
	timestamp: number;
	messageType?: string;
	metadata?: any;
};

const chatService = new ChatService();

const LiveChat = () => {
	const html = `
	<section class="retro-card crt space-y-4 p-8 h-full relative">

		<!-- Title -->
		<div class="text-center">
			<img src="${liveChatTitle}" alt="LiveChat" class="mx-auto max-w-full" />
		</div>

		<div class="grid grid-cols-1 md:grid-cols-4 gap-4 h-[450px] min-h-0">

			<!-- Friends -->
			<div class="pixel-frame pixel-frame--clear p-4 flex flex-col min-h-0">                                                                    
				<div class="font-bold border-b-2 border-[var(--fg)] pb-2 mb-2">FRIENDS</div>                                                          
			<div id="chat-friends-list" class="flex-1 overflow-y-auto pr-2 retro-scroll">                                                                          
				<div class="text-center text-[var(--muted)] py-4">Loading...</div>                                                                    
			</div>                                                                                                                                    
			</div>

			<!-- Chat -->
			<div class="md:col-span-3 pixel-frame pixel-frame--clear p-4 flex flex-col min-h-0 relative">

				<!-- Banner -->
				<div id="chat-banner" class="border-b-2 border-[var(--fg)] pb-2 mb-2">
					<div class="font-bold text-[var(--fg)]">SELECT A FRIEND TO CHAT</div>
				</div>

				<!-- Blocked warning -->
				<div
				id="blocked-warning"
				class="hidden bg-[var(--error)] text-white p-3 mb-2 border-2 border-[var(--fg)]"
				></div>

				<!-- Messages -->
				<div
				id="chat-messages"
				class="flex-1 overflow-y-auto space-y-2 pr-2 retro-scroll"
				></div>

				<!-- Input -->
				<div class="mt-3 flex gap-2">
					<input
						id="chat-input"
						type="text"
						placeholder="Type a message..."
						class="flex-1 rounded-lg border bg-[rgba(255,255,255,0.03)] px-3 py-2 text-[var(--fg)]"
					/>
					<button id="chat-send-button" class="retro-btn">Send</button>
					<button id="broadcast-button" class="retro-btn">All</button>
				</div>

			</div>
		</div>
	<!-- Tournament Match Banner -->                                                                                                           
	<div id="tournament-banner" 
			class="hidden absolute top-[55%] left-[55%] -translate-x-1/2 -translate-y-1/2 w-[80%] md:w-[400px] z-50">
			
			<div class="pixel-frame bg-[#FFB84D] text-black py-4 px-5 rounded-xl shadow-2xl relative animate-pulse-slow border-2 border-black/20">
				
				<button id="close-tournament-banner" 
						class="absolute -top-3 -right-3 bg-white text-black border-2 border-black w-7 h-7 flex items-center justify-center rounded-full hover:scale-110 transition-transform z-50 shadow-md"
						style="font-family: sans-serif;">
					<span class="font-bold text-base leading-none pt-[1px]">X</span>
				</button>

				<div class="flex flex-col items-center justify-center gap-2 text-center">
					<div class="w-full overflow-hidden">
						<div class="text-sm uppercase font-bold tracking-widest opacity-75 mb-1">Tournament</div>
						
						<div class="text-xl font-extrabold truncate leading-tight">
							Ready for the next game?
						</div>
						
						<div id="tournament-match-players" class="text-lg font-bold opacity-90 truncate mt-2">
							P1 vs P2
						</div>
					</div>
					
					<button 
						id="tournament-join-btn" 
						class="retro-btn text-lg font-black px-8 py-2 mt-2 shadow-lg hover:scale-105 active:scale-95 transition-transform"
						style="--btn-color: #FFD700; --pixel-outline: #000; color: black;"
					>
						JOIN
					</button>
				</div>
			</div>    
		</div>
	</section>
	`;

	const onMount = () => {
		/* ---------------- Session ---------------- */
		const me = sessionService.loadUser();
		if (!me) {
			window.location.href = "/login";
			return;
		}
		const myUserId = me.id;

		/* ---------------- DOM ---------------- */
		const friendsList = document.getElementById("chat-friends-list")!;
		const chatBanner = document.getElementById("chat-banner")!;
		const chatMessages = document.getElementById("chat-messages")!;
		const chatInput = document.getElementById(
			"chat-input"
		) as HTMLInputElement;
		const sendButton = document.getElementById("chat-send-button")!;
		const broadcastButton = document.getElementById("broadcast-button")!;
		const blockedWarning = document.getElementById("blocked-warning")!;
		const tournamentBanner = document.getElementById("tournament-banner")!;
		const tournamentMatchPlayers = document.getElementById(
			"tournament-match-players"
		);
		const tournamentJoinBtn = document.getElementById(
			"tournament-join-btn"
		);
		const closeTournamentBtn = document.getElementById(
			"close-tournament-banner"
		);
		let blockButton: HTMLButtonElement | null = null;
		let chatActions: HTMLDivElement | null = null;

		chatInput.disabled = true;
		sendButton.classList.add("disabled");

		/* ---------------- State ---------------- */
		let activeFriend: Friend | null = null;
		const messages: Record<number, ChatMessage[]> = {};
		const blockedUsers = new Set<number>();
		const activeGameInvites = new Set<number>();
		const friendAvatars: Record<number, string | null> = {};
		let friends: Friend[] = [];
		let pendingTournamentMatch: {
			matchId: number;
			tournamentId: number;
		} | null = null;

		/* ---------------- Navigation Helper ---------------- */
		const navigate = (path: string) => {
			const event = new CustomEvent("navigate", { detail: { path } });
			window.dispatchEvent(event);
			window.location.href = path;
		};

		/* ---------------- Banner ---------------- */
		const renderBanner = (friend: Friend, avatar?: string | null) => {
			const initials = friend.username.slice(0, 2).toUpperCase();
			const normalizedAvatar = avatar?.trim() || null;
			const resolvedAvatar = normalizedAvatar
				? resolveAvatarUrl(normalizedAvatar)
				: null;
			const avatarHtml = resolvedAvatar
				? `<img id="chat-banner-avatar" src="${resolvedAvatar}"
						class="h-full w-full object-cover" />`
				: `<span class="flex h-full w-full items-center justify-center font-bold text-[var(--fg)]">
					${initials}
				</span>`;

			const inviteDisabled = activeGameInvites.has(friend.id);

			chatBanner.innerHTML = `
				<div class="flex justify-between items-center">
					<div class="flex items-center gap-4">
						<a data-link href="/dashboard?user=${friend.id}">
							<img src="${dashboardIcon}" class="h-10" />
						</a>
						<div
							class="h-12
							w-12 rounded-full
							overflow-hidden
							border">
							${avatarHtml}
						</div>
						<div class="header-font-sm text-[100px]">${friend.username}</div>
					</div>
					<div id="chat-actions" class="flex items-center gap-2">
						<button
							id="invite-button"
							class="
							retro-btn
							retro-btn--sm
							${inviteDisabled ? "opacity-50 cursor-not-allowed" : ""}
							"
							${inviteDisabled ? "disabled" : ""}
						>
							INVITE
						</button>
						<button
							id="block-user-button"
							class="retro-btn retro-btn--sm"
						>
							Block
						</button>
					</div>
				</div>
				`;

			const bannerAvatar = chatBanner.querySelector<HTMLImageElement>(
				"#chat-banner-avatar"
			);
			if (bannerAvatar) {
				bannerAvatar.addEventListener(
					"error",
					() => {
						const fallback = document.createElement("span");
						fallback.className =
							"flex h-full w-full items-center justify-center font-bold text-[var(--fg)]";
						fallback.textContent = initials;
						bannerAvatar.replaceWith(fallback);
					},
					{ once: true }
				);
			}

			chatActions = document.getElementById(
				"chat-actions"
			) as HTMLDivElement | null;
			blockButton = document.getElementById(
				"block-user-button"
			) as HTMLButtonElement | null;
			blockButton?.addEventListener("click", toggleBlockUser);

			const inviteButton = document.getElementById("invite-button");
			inviteButton?.addEventListener("click", handleInviteClick);
		};

		/* ---------------- Pong Challenge ---------------- */
		const renderMessageContent = (m: ChatMessage) => {
			if (m.messageType === "GAME_INVITE") {
				if (m.sender === "me") {
					return `
					<div class="space-y-2 text-center">
						<div>!!! Pong Challenge Sent !!!</div>
						<button
							class="retro-btn retro-btn--danger cancel-challenge"
						>
							CANCEL
						</button>
					</div>
					`;
				} else {
					return `
					<div class="space-y-2 text-center">
						<div>!!! Pong Challenge !!!</div>
						<button
							class="retro-btn accept-challenge"
						>
							ACCEPT
						</button>
						<button
							class="retro-btn retro-btn--danger decline-challenge"
						>
							DECLINE
						</button>
					</div>
						`;
				}
			}

			if (m.messageType === "GAME_INVITE_CANCELLED") {
				return "<div class='opacity-60 text-center'>Challenge cancelled</div>";
			}

			if (m.messageType === "GAME_INVITE_DECLINED") {
				return "<div class='opacity-60 text-center'>Challenge declined</div>";
			}

			if (m.messageType === "GAME_INVITE_ACCEPTED") {
				return "<div class='opacity-60 text-center'>Challenge accepted</div>";
			}

			return m.content;
		};

		const applyInviteStatus = (
			friendMessages: ChatMessage[],
			nextType: string,
			nextContent: string,
			metadata?: any
		) => {
			for (let i = friendMessages.length - 1; i >= 0; i--) {
				if (friendMessages[i].messageType === "GAME_INVITE") {
					friendMessages[i].messageType = nextType;
					friendMessages[i].content = nextContent;
					if (metadata !== undefined) {
						friendMessages[i].metadata = metadata;
					}
					return true;
				}
			}
			return false;
		};

		const markInviteStatus = (
			friendId: number,
			nextType: string,
			nextContent: string,
			metadata?: any
		) => {
			const friendMessages = messages[friendId];
			if (!friendMessages) return false;
			return applyInviteStatus(
				friendMessages,
				nextType,
				nextContent,
				metadata
			);
		};

		const normalizeInviteHistory = (friendId: number) => {
			const friendMessages = messages[friendId];
			if (!friendMessages) return;
			const normalized: ChatMessage[] = [];
			for (const msg of friendMessages) {
				if (msg.messageType === "GAME_INVITE_ACCEPTED") {
					const applied = applyInviteStatus(
						normalized,
						"GAME_INVITE_ACCEPTED",
						"Challenge accepted",
						msg.metadata
					);
					if (!applied) normalized.push(msg);
					continue;
				}
				if (msg.messageType === "GAME_INVITE_DECLINED") {
					const applied = applyInviteStatus(
						normalized,
						"GAME_INVITE_DECLINED",
						"Challenge declined",
						msg.metadata
					);
					if (!applied) normalized.push(msg);
					continue;
				}
				if (msg.messageType === "GAME_INVITE_CANCELLED") {
					const applied = applyInviteStatus(
						normalized,
						"GAME_INVITE_CANCELLED",
						"Challenge cancelled",
						msg.metadata
					);
					if (!applied) normalized.push(msg);
					continue;
				}
				normalized.push(msg);
			}
			messages[friendId] = normalized;
		};

		const syncPendingInviteState = (friendId: number) => {
			const friendMessages = messages[friendId];
			if (!friendMessages) return;
			const hasPendingInvite = friendMessages.some(
				(msg) => msg.messageType === "GAME_INVITE"
			);
			if (hasPendingInvite) {
				activeGameInvites.add(friendId);
			} else {
				activeGameInvites.delete(friendId);
			}
			if (!activeFriend || activeFriend.id !== friendId) return;
			const inviteButton = document.getElementById(
				"invite-button"
			) as HTMLButtonElement | null;
			if (!inviteButton) return;
			inviteButton.disabled = hasPendingInvite;
			inviteButton.classList.toggle("opacity-50", hasPendingInvite);
			inviteButton.classList.toggle(
				"cursor-not-allowed",
				hasPendingInvite
			);
		};

		/* ---------------- Presence / WS ---------------- */
		connectChat((msg) => {
			if (msg.type !== "RECEIVE_MESSAGE") return;
			const { fromUserId, content, timestamp, messageType, metadata } =
				msg;

			if (blockedUsers.has(fromUserId)) return;

			if (
				msg.messageType === "GAME_INVITE_ACCEPTED" ||
				msg.messageType === "GAME_INVITE_DECLINED" ||
				msg.messageType === "GAME_INVITE_CANCELLED"
			) {
				activeGameInvites.delete(msg.fromUserId);
				if (activeFriend && activeFriend.id === msg.fromUserId) {
					renderBanner(activeFriend, friendAvatars[activeFriend.id]);
				}
			}

			if (messageType === "GAME_INVITE_ACCEPTED" && metadata?.matchId) {
				markInviteStatus(
					fromUserId,
					"GAME_INVITE_ACCEPTED",
					"Challenge accepted",
					metadata
				);
				if (activeFriend?.id === fromUserId) {
					renderMessages();
				}
				sessionStorage.setItem(
					"pong:autostart",
					JSON.stringify({ mode: "remote" })
				);

				navigate(`/pong/${metadata.matchId}`);
				return;
			}

			if (msg.messageType === "GAME_INVITE_DECLINED") {
				activeGameInvites.delete(fromUserId);
				if (messages[fromUserId]) {
					const inviteMsg = messages[fromUserId].find(
						(m) => m.messageType === "GAME_INVITE"
					);
					if (inviteMsg) {
						inviteMsg.content = "Challenge declined";
						inviteMsg.messageType = "GAME_INVITE_DECLINED";
					}
				}
				if (activeFriend?.id === fromUserId) {
					renderBanner(activeFriend, friendAvatars[activeFriend.id]);
					renderMessages();
				}
				return;
			}

			if (msg.messageType === "GAME_INVITE_CANCELLED") {
				activeGameInvites.delete(fromUserId);
				if (messages[fromUserId]) {
					const inviteMsg = messages[fromUserId].find(
						(m) =>
							m.messageType === "GAME_INVITE" &&
							m.sender === "friend"
					);
					if (inviteMsg) {
						inviteMsg.content = "Challenge cancelled";
						inviteMsg.messageType = "GAME_INVITE_CANCELLED";
					}
				}
				if (activeFriend?.id === fromUserId) {
					renderBanner(activeFriend, friendAvatars[activeFriend.id]);
					renderMessages();
				}
				return;
			}

			if (!messages[fromUserId]) {
				messages[fromUserId] = [];
			}

			messages[fromUserId].push({
				sender: "friend",
				content,
				timestamp,
				messageType,
				metadata,
			});

			if (messageType === "GAME_INVITE") {
				activeGameInvites.add(fromUserId);
				if (activeFriend?.id === fromUserId) {
					renderBanner(activeFriend, friendAvatars[activeFriend.id]);
				}
			}

			if (messageType === "TOURNAMENT_MATCH_READY" && metadata) {
				const { player1Name, player2Name, matchId, tournamentId } =
					metadata;
				tournamentMatchPlayers!.textContent = `${player1Name} vs ${player2Name}`;
				pendingTournamentMatch = { matchId, tournamentId };
				tournamentBanner.classList.remove("hidden");
				return;
			}

			if (activeFriend?.id === fromUserId) {
				renderMessages();
			}
		});

		/* ----------- Load Blocked Users-------------------- */
		const loadBlockedUsers = async () => {
			try {
				const result = await chatService.getBlockedUsers(myUserId);
				if (result.success && result.data) {
					result.data.forEach((user: any) =>
						blockedUsers.add(user.id)
					);
					//console.log(`[LiveChat] Loaded ${blockedUsers.size} blocked users`);
				}
			} catch (err) {
				console.error("[LiveChat] Error loading blocked users:", err);
			}
		};

		/* --------- Check if User is Blocked---------------- */
		const checkIfBlocked = async (
			userId: number
		): Promise<{ isBlocked: boolean; iAmBlocker: boolean }> => {
			try {
				const result = await chatService.checkBlockStatus(
					myUserId,
					userId
				);
				if (result.success && result.data) {
					const { isBlocked, iAmBlocker } = result.data;

					if (iAmBlocker && isBlocked) {
						blockedUsers.add(userId);
					} else {
						blockedUsers.delete(userId);
					}

					return { isBlocked, iAmBlocker };
				}
				return { isBlocked: false, iAmBlocker: false };
			} catch (err) {
				console.error("[LiveChat] Error checking block status:", err);
				return { isBlocked: false, iAmBlocker: false };
			}
		};

		/* ---------- Block/Unblock User ---------------- */
		const toggleBlockUser = async () => {
			if (!activeFriend) return;

			const iAmBlocker = blockedUsers.has(activeFriend.id);

			try {
				if (iAmBlocker) {
					const result = await chatService.unblockUser(
						myUserId,
						activeFriend.id
					);
					if (result.success) {
						blockedUsers.delete(activeFriend.id);
						updateBlockUI(false, false);
						//console.log(`[LiveChat] Unblocked user ${activeFriend.id}`);
					}
				} else {
					const result = await chatService.blockUser(
						myUserId,
						activeFriend.id
					);
					if (result.success) {
						blockedUsers.add(activeFriend.id);
						updateBlockUI(true, true);
						//console.log(`[LiveChat] Blocked user ${activeFriend.id}`);
					}
				}
			} catch (err) {
				console.error("[LiveChat] Error toggling block:", err);
			}
		};

		/* ------------- Update Block UI State ------------------ */
		const updateBlockUI = (isBlocked: boolean, iAmBlocker: boolean) => {
			if (!chatActions || !blockButton) return;

			if (!iAmBlocker && isBlocked) {
				chatActions.classList.add("hidden");
				blockedWarning.innerHTML = "This user has blocked you.";
				blockedWarning.classList.remove("hidden");
				chatInput.disabled = true;
				sendButton.classList.add("disabled");
			} else if (isBlocked && iAmBlocker) {
				blockButton.textContent = "Unblock";
				chatActions.classList.remove("hidden");
				blockedWarning.innerHTML =
					"You have blocked this user. Unblock to send messages.";
				blockedWarning.classList.remove("hidden");
				chatInput.disabled = true;
				sendButton.classList.add("disabled");
			} else {
				blockButton.textContent = "Block";
				chatActions.classList.remove("hidden");
				blockedWarning.classList.add("hidden");
				chatInput.disabled = false;
				sendButton.classList.remove("disabled");
			}
		};

		/* ---------------- Friends ---------------- */
		const renderFriendsList = () => {
			if (friends.length === 0) {
				friendsList.innerHTML = `<div class="text-center text-[var(--muted)] py-4">
					You have no friends yet
				</div>`;
				return;
			}

			friendsList.innerHTML = friends
				.map(
					(friend) =>
						`<button
					class="
						w-full
						text-left
						pixel-frame
						bg-[var(--bg)]
						p-2 mb-2
						hover:border-[var(--accent)]"
					data-friend-id="${friend.id}"
				>
					<div class="flex items-center gap-2 text-[var(--fg)] font-bold">
						<span class="w-2 h-2 rounded-full ${
							presenceService.isOnline(friend.id)
								? "bg-green-500"
								: "bg-gray-500"
						}"></span>
						<span>${friend.username}</span>
					</div>
				</button>
				`
				)
				.join("");

			friendsList
				.querySelectorAll("button[data-friend-id]")
				.forEach((btn) => {
					btn.addEventListener("click", () => {
						const id = Number(
							(btn as HTMLButtonElement).dataset.friendId
						);
						const friend = friends.find((f) => f.id === id);
						if (friend) selectFriend(friend);
					});
				});
		};

		presenceService.subscribe(() => {
			if (friends.length) renderFriendsList();
		});

		const loadFriends = async () => {
			try {
				friendsList.innerHTML = `<div class="text-center text-[var(--muted)] py-4">
						Loading...
					</div>`;
				const response = await getRelationsList();
				if (!response.success || !response.data?.relationsWithStatus) {
					friendsList.innerHTML = `<div class="text-center text-[var(--error)] py-4">
						Failed to load friends
					</div>`;
					return;
				}

				friends = response.data.relationsWithStatus;
				renderFriendsList();
			} catch (err) {
				console.error("[LiveChat] Load friends error:", err);
				friendsList.innerHTML = `<div class="text-center text-[var(--error)] py-4">
					Error loading friends
				</div>`;
			}
		};

		/* ------------- Select Friend -------------------- */
		const selectFriend = async (friend: Friend) => {
			activeFriend = friend;
			const avatar =
				friendAvatars[friend.id] ??
				(await userService.getUserByID(friend.id)).data?.avatar_url ??
				null;
			friendAvatars[friend.id] = avatar;

			renderBanner(friend, avatar);
			chatActions?.classList.remove("hidden");

			const { isBlocked, iAmBlocker } = await checkIfBlocked(friend.id);
			updateBlockUI(isBlocked, iAmBlocker);

			try {
				const threads = await chatService.getUserThreads(myUserId);
				const directThread = threads?.data?.find(
					(t) => !t.is_group && t.participants?.includes(friend.id)
				);

				if (!directThread) {
					messages[friend.id] = [];
					renderMessages();
					return;
				}

				const history = await chatService.getMessages(directThread.id);
				if (history?.data && Array.isArray(history.data)) {
					messages[friend.id] = history.data.map((m) => ({
						sender: m.sender_id === friend.id ? "friend" : "me",
						content: m.content,
						timestamp: m.created_at,
						messageType: m.message_type,
						metadata: m.metadata ? JSON.parse(m.metadata) : null,
					}));
					normalizeInviteHistory(friend.id);
				} else {
					messages[friend.id] = [];
				}

				syncPendingInviteState(friend.id);

				renderMessages();
			} catch (err) {
				messages[friend.id] = [];
				renderMessages();
			}
		};

		/* ---------------- Messages ---------------- */
		const renderMessages = () => {
			if (!activeFriend) return;
			chatMessages.innerHTML = (messages[activeFriend.id] || [])
				.map(
					(m) => `
				<div class=
					"flex ${m.sender === "me" ? "justify-end" : "justify-start"}"
				>
					<div class="pixel-frame px-3 py-2 max-w-[70%]">
						${renderMessageContent(m)}
					</div>
				</div>`
				)
				.join("");
			chatMessages.scrollTop = chatMessages.scrollHeight;
		};

		const sendMessage = () => {
			if (!activeFriend) return;
			if (blockedUsers.has(activeFriend.id)) return;

			const text = chatInput.value.trim();
			if (!text) return;

			if (!messages[activeFriend.id]) {
				messages[activeFriend.id] = [];
			}

			messages[activeFriend.id].push({
				sender: "me",
				content: text,
				timestamp: Date.now(),
			});

			sendDirectMessage(activeFriend.id, text);
			chatInput.value = "";
			renderMessages();
		};

		/* ---------------- Invite Handler ---------------- */
		const handleInviteClick = () => {
			if (!activeFriend) return;
			if (activeGameInvites.has(activeFriend.id)) return;

			activeGameInvites.add(activeFriend.id);
			renderBanner(activeFriend, friendAvatars[activeFriend.id]);

			const invitePayload = {
				senderId: myUserId,
				receiverId: activeFriend.id,
				content: "Pong invitation",
				messageType: "GAME_INVITE",
				metadata: { game: "pong" },
			};

			sendDirectMessage(activeFriend.id, JSON.stringify(invitePayload));

			messages[activeFriend.id].push({
				sender: "me",
				content: "Pong invitation",
				timestamp: Date.now(),
				messageType: "GAME_INVITE",
				metadata: { game: "pong" },
			});

			renderMessages();
		};

		/* ---------------- Challenge Response Handler ---------------- */
		chatMessages.addEventListener("click", async (e) => {
			const acceptBtn = (e.target as HTMLElement).closest(
				".accept-challenge"
			);
			const declineBtn = (e.target as HTMLElement).closest(
				".decline-challenge"
			);
			const cancelBtn = (e.target as HTMLElement).closest(
				".cancel-challenge"
			);

			if (!activeFriend) return;

			if (acceptBtn) {
				const challengerId = activeFriend.id;

				activeGameInvites.delete(challengerId);

				const created = await matchService.createMatch({
					gameTypeId: 1,
				});
				if (!created.success || !created.data) {
					alert("Failed to create match");
					return;
				}

				const matchId = created.data;

				await matchService.addParticipant(matchId, myUserId);
				await matchService.addParticipant(matchId, challengerId);

				markInviteStatus(
					challengerId,
					"GAME_INVITE_ACCEPTED",
					"Challenge accepted",
					{ game: "pong", matchId }
				);
				if (activeFriend?.id === challengerId) {
					renderMessages();
				}

				const acceptPayload = {
					senderId: myUserId,
					receiverId: challengerId,
					content: "Challenge accepted",
					messageType: "GAME_INVITE_ACCEPTED",
					metadata: { game: "pong", matchId },
				};

				sendDirectMessage(challengerId, JSON.stringify(acceptPayload));

				const inviteMsg = messages[activeFriend.id].find(
					(m) =>
						m.messageType === "GAME_INVITE" && m.sender === "friend"
				);
				if (inviteMsg) {
					inviteMsg.content = "Challenge accepted";
					inviteMsg.messageType = "GAME_INVITE_ACCEPTED";
					inviteMsg.metadata = { game: "pong", matchId };
				}

				renderMessages();

				sessionStorage.setItem(
					"pong:autostart",
					JSON.stringify({ mode: "remote" })
				);
				navigate(`/pong/${matchId}`);
				return;
			}

			if (declineBtn) {
				const declinePayload = {
					senderId: myUserId,
					receiverId: activeFriend.id,
					content: "Challenge declined",
					messageType: "GAME_INVITE_DECLINED",
					metadata: { game: "pong" },
				};

				sendDirectMessage(
					activeFriend.id,
					JSON.stringify(declinePayload)
				);

				activeGameInvites.delete(activeFriend.id);
				renderBanner(activeFriend, friendAvatars[activeFriend.id]);

				const inviteMsg = messages[activeFriend.id].find(
					(m) =>
						m.messageType === "GAME_INVITE" && m.sender === "friend"
				);

				if (inviteMsg) {
					inviteMsg.content = "Challenge declined";
					inviteMsg.messageType = "GAME_INVITE_DECLINED";
				}

				renderMessages();
				return;
			}

			if (cancelBtn) {
				const cancelPayload = {
					senderId: myUserId,
					receiverId: activeFriend!.id,
					content: "Challenge cancelled",
					messageType: "GAME_INVITE_CANCELLED",
					metadata: { game: "pong" },
				};

				sendDirectMessage(
					activeFriend!.id,
					JSON.stringify(cancelPayload)
				);

				const inviteMsg = messages[activeFriend!.id].find(
					(m) => m.messageType === "GAME_INVITE" && m.sender === "me"
				);
				if (inviteMsg) {
					inviteMsg.content = "Challenge cancelled";
					inviteMsg.messageType = "GAME_INVITE_CANCELLED";
				}

				if (activeFriend) {
					activeGameInvites.delete(activeFriend.id);
					renderBanner(activeFriend, friendAvatars[activeFriend.id]);
				}
				renderMessages();
				return;
			}
		});

		/* --------Check Pending Tournament Match--------------*/
		const checkPendingTournamentMatch = async () => {
			try {
				const result = await tournamentService.getPendingMatch(
					myUserId
				);
				if (result.success && result.data) {
					const { player1Name, player2Name, matchId, tournamentId } =
						result.data;
					tournamentMatchPlayers!.textContent = `${player1Name} vs ${player2Name}`;
					pendingTournamentMatch = { matchId, tournamentId };
					tournamentBanner.classList.remove("hidden");
				}
			} catch (error) {
				console.error(
					"[LiveChat] Error checking pending match:",
					error
				);
			}
		};

		/* ---------------- Event Listeners ---------------- */
		sendButton.addEventListener("click", sendMessage);

		broadcastButton.addEventListener("click", () => {
			const text = chatInput.value.trim();
			if (!text) return;

			const friendButtons = friendsList.querySelectorAll(
				"button[data-friend-id]"
			);
			friendButtons.forEach((btn) => {
				const friendId = Number((btn as HTMLElement).dataset.friendId);
				if (!blockedUsers.has(friendId)) {
					if (!messages[friendId]) {
						messages[friendId] = [];
					}
					messages[friendId].push({
						sender: "me",
						content: text,
						timestamp: Date.now(),
					});
				}
			});

			sendBroadcast(text);
			chatInput.value = "";

			if (activeFriend) {
				renderMessages();
			}
		});

		chatInput.addEventListener("keypress", (e) => {
			if (e.key === "Enter") sendMessage();
		});

		tournamentJoinBtn!.addEventListener("click", () => {
			if (pendingTournamentMatch)
				navigate(`/pong/${pendingTournamentMatch.matchId}`);
		});

		closeTournamentBtn?.addEventListener("click", () => {
			tournamentBanner.classList.add("hidden");
			pendingTournamentMatch = null;
		});

		/* ---------------- Initialize ---------------- */
		checkPendingTournamentMatch();
		loadFriends();
		loadBlockedUsers();

		return () => {
			disconnectChat();
		};
	};

	return { html, onMount };
};

export default LiveChat;
