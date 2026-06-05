import profileTitle from "../assets/images/page_titles/PROFILE_PAGE.png";
import dashboardIcon from "../assets/images/page_assets/DASHBOARD.png";
import settingsIcon from "../assets/images/page_assets/SETTINGS.png";
import { sessionService } from "../services/session.service";
import { presenceService } from "../services/presence.service";
import type { PublicUser } from "../services/types";
import { renderAvatarDisplay } from "../utils/avatar";

const Profile = () => {
	const html = `
	<section class="retro-card crt space-y-6 p-8">
		<div class="fixed right-6 top-6 z-20 flex flex-col gap-3">
			<a data-link href="/settings">
				<img src="${settingsIcon}" alt="Settings" class="h-12 w-14" />
			</a>
			<a data-link href="/dashboard">
				<img src="${dashboardIcon}" alt="Dashboard" class="h-12 w-14" />
			</a>
		</div>

		<div class="flex justify-center">
			<img src="${profileTitle}" alt="Profile" class="mx-auto h-auto max-w-full" />
		</div>

		<div id="profile-card" class="pixel-frame pixel-frame--clear p-6">
			<p class="text-sm text-[var(--muted)]">Loading your profile...</p>
		</div>

		<div id="profile-actions" class="flex justify-center"></div>
	</section>
	`;

	const onMount = () => {
		const container = document.querySelector<HTMLDivElement>("#profile-card");
		const actions = document.querySelector<HTMLDivElement>("#profile-actions");
		if (!container || !actions) return;

		const sessionUser = sessionService.loadUser();

		if (!sessionUser) {
			container.innerHTML = `
			<div class="space-y-3 text-center">
				<p class="text-base font-semibold text-[var(--fg)]">You're not signed in.</p>
				<p class="text-sm text-[var(--muted)]">Log in to see your profile details here.</p>
				<a data-link href="/login" class="retro-btn justify-center">Go to Login</a>
			</div>
			`;
			actions.innerHTML = "";
			return;
		}

		const renderProfile = (profileUser: PublicUser) => {
			const title = "Signed in as";
			const emailLine = `<p class="text-sm text-[var(--muted)]">${profileUser.email}</p>`;

			container.innerHTML = `
			<div class="flex flex-col gap-4 sm:flex-row sm:items-center sm:justify-between">
				<div class="space-y-1">
					<p class="text-xs font-semibold uppercase tracking-[0.2em] text-[var(--fg)]">${title}</p>
					<h2 class="profile-username font-bold text-[var(--fg)]">${profileUser.username}</h2>
					${emailLine}
				</div>

				<div class="flex flex-col items-center gap-2">
					<div id="profile-avatar-display"
					class="inline-flex h-16 w-16 items-center justify-center overflow-hidden rounded-full border border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.2)]">
					${renderAvatarDisplay(profileUser)}
					</div>
					<div class="text-xs text-[var(--muted)]">
					Joined: ${new Date(profileUser.created_at).toLocaleDateString()}
					</div>
				</div>
			</div>
			`;

			actions.innerHTML = `
			<button id="profile-logout" class="retro-btn">Log out</button>
			`;

			const logoutButton =
				document.querySelector<HTMLButtonElement>("#profile-logout");

			logoutButton?.addEventListener("click", async () => {
				try {
					presenceService.disconnect();

					// Call logout API to clear cookies
					const response = await fetch("/api/auth/logout", {
						method: "POST",
						credentials: "include"
					});

					if (response.ok) {
						sessionService.clearUser();
						window.location.href = "/#/login";
					} else {
						console.error("Logout failed");
						sessionService.clearUser();
						window.location.href = "/#/login";
					}
				} catch (error) {
					console.error("Logout error:", error);
					sessionService.clearUser();
					window.location.href = "/#/login";
				}
			});
		};

		renderProfile(sessionUser);
	};

	return { html, onMount };
};

export default Profile;
