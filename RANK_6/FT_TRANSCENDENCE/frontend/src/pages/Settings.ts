import settingsTitle from "../assets/images/page_titles/SETTINGS_PAGE.png";
import arrowBack from "../assets/images/page_assets/ARROW_BACK.png";
import UsernameSlot from "../components/UsernameSlot";
import { sessionService } from "../services/session.service";
import type { PublicUser } from "../services/types";
import { initSettingsAvatar } from "../services/settingsAvatar.service";
import { initSettingsPassword } from "../services/settingsPassword.service";
import { initSettingsUsername } from "../services/settingsUsername.service";

const Settings = () => {
	const usernameSlots = ["first", "second", "third", "fourth"]
		.map((ordinal, index) =>
			UsernameSlot(index, ordinal, {
				slotClass: "username-slot--compact",
			}),
		)
		.join("");
	const html = `
    <section class="retro-card crt space-y-6 p-8">
      <a
        id="settings-back"
        data-link
        href="/profile"
        class="fixed left-6 top-6 z-20 flex items-center gap-3 text-base font-semibold uppercase text-[var(--fg)]"
        aria-label="Back"
      >
        <img src="${arrowBack}" alt="" class="h-7 w-auto" />
        <span>Back</span>
      </a>
      <div class="space-y-2 text-center text-[var(--fg)]">
        <div class="flex justify-center">
          <img src="${settingsTitle}" alt="Settings" class="mx-auto h-auto max-w-full" />
        </div>
      </div>
      <div class="space-y-8">
        <div class="pixel-frame pixel-frame--clear p-6">
          <form id="settings-username-form" class="space-y-4">
            <input type="hidden" name="username" id="settings-username-hidden" value="AAAA" />
            <div class="flex flex-col gap-4 sm:flex-row sm:items-center sm:justify-between">
              <button type="submit" class="retro-btn w-full justify-center sm:w-auto">Update username</button>
              <div id="settings-username-picker" class="username-picker--compact flex justify-center gap-2">
                ${usernameSlots}
              </div>
            </div>
            <div id="settings-username-status" class="hidden rounded-lg border px-3 py-2 text-sm font-medium shadow-[0_10px_35px_rgba(0,0,0,0.4)]" role="status"></div>
          </form>
        </div>

        <div class="pixel-frame pixel-frame--clear p-6">
          <form id="settings-password-form" class="space-y-4">
            <label class="block space-y-1">
              <span class="text-sm font-semibold text-[var(--fg)]">New password</span>
              <input
                type="password"
                name="password"
                id="settings-password"
                class="w-full rounded-lg border border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.03)] px-3 py-2 text-[var(--fg)] placeholder:text-[rgba(141,161,197,0.75)] focus:border-[var(--fg)] focus:outline-none focus:ring-2 focus:ring-[rgba(230,240,255,0.25)]"
                placeholder="••••••••"
              />
              <div id="settings-password-strength" class="hidden rounded px-2 py-1 text-xs font-semibold uppercase"></div>
              <div id="settings-password-error" class="hidden text-xs text-[#ffd6d6]"></div>
            </label>
            <label class="block space-y-1">
              <span class="text-sm font-semibold text-[var(--fg)]">Confirm password</span>
              <input
                type="password"
                name="confirmPassword"
                id="settings-confirm-password"
                class="w-full rounded-lg border border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.03)] px-3 py-2 text-[var(--fg)] placeholder:text-[rgba(141,161,197,0.75)] focus:border-[var(--fg)] focus:outline-none focus:ring-2 focus:ring-[rgba(230,240,255,0.25)]"
                placeholder="••••••••"
              />
              <div id="settings-confirm-password-error" class="hidden text-xs text-[#ffd6d6]"></div>
            </label>
            <button type="submit" class="retro-btn w-full justify-center">Update password</button>
            <div id="settings-password-status" class="hidden rounded-lg border px-3 py-2 text-sm font-medium shadow-[0_10px_35px_rgba(0,0,0,0.4)]" role="status"></div>
          </form>
        </div>

        <div class="pixel-frame pixel-frame--clear p-6">
          <form id="settings-avatar-form" class="space-y-4">
            <div class="flex flex-wrap items-center gap-4 sm:justify-between">
              <div class="flex flex-wrap items-center gap-4">
                <label id="settings-avatar-option-photo" class="flex cursor-pointer items-center justify-center rounded-full p-1 transition focus-within:ring-2 focus-within:ring-[var(--fg)]" aria-label="Use uploaded photo">
                  <input
                    type="radio"
                    id="settings-avatar-choice-photo"
                    name="avatarChoice"
                    value="photo"
                    class="sr-only"
                  />
                  <span id="settings-avatar-display" class="inline-flex h-16 w-16 items-center justify-center overflow-hidden rounded-full border border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.2)]"></span>
                </label>
                <label id="settings-avatar-option-initials" class="flex cursor-pointer items-center justify-center rounded-full p-1 transition focus-within:ring-2 focus-within:ring-[var(--fg)]" aria-label="Use initials">
                  <input
                    type="radio"
                    id="settings-avatar-choice-initials"
                    name="avatarChoice"
                    value="initials"
                    class="sr-only"
                  />
                  <span id="settings-avatar-initials" class="inline-flex h-16 w-16 items-center justify-center overflow-hidden rounded-full border border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.2)] text-lg font-bold text-[var(--fg)]"></span>
                </label>
              </div>
              <button type="submit" class="retro-btn w-full justify-center sm:w-auto sm:ml-auto">Save avatar</button>
            </div>
            <div class="relative w-full">
              <input
                type="file"
                id="settings-avatar-file"
                name="avatar"
                accept="image/png,image/jpeg,image/jpg"
                class="w-full rounded-lg border border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.03)] px-3 py-2 text-sm text-transparent text-[var(--fg)] file:mr-3 file:w-24 file:rounded-md file:border-0 file:bg-[rgba(230,240,255,0.15)] file:px-3 file:py-1 file:text-center file:text-xs file:font-semibold file:text-[var(--fg)]"
              />
              <span id="settings-avatar-file-name" class="pointer-events-none absolute inset-y-0 left-0 flex items-center pl-28 text-sm text-[var(--muted)]">PNG or JPG. Max 5MB.</span>
            </div>
            <div id="settings-avatar-status" class="hidden rounded-lg border px-3 py-2 text-sm font-medium shadow-[0_10px_35px_rgba(0,0,0,0.4)]" role="status"></div>
          </form>
        </div>

      </div>
    </section>
  `;

	const onMount = () => {
		// 1) Collect all required DOM elements for the settings modules.
		// Username update form element.
		const usernameForm = document.querySelector<HTMLFormElement>(
			"#settings-username-form",
		);
		// Container that lists username choices.
		const usernamePicker = document.querySelector<HTMLDivElement>(
			"#settings-username-picker",
		);
		// Status/feedback area for username updates.
		const usernameStatus = document.querySelector<HTMLDivElement>(
			"#settings-username-status",
		);
		// Hidden input that stores the selected username.
		const usernameHidden = document.querySelector<HTMLInputElement>(
			"#settings-username-hidden",
		);
		// Password change form element.
		const passwordForm = document.querySelector<HTMLFormElement>(
			"#settings-password-form",
		);
		// Status/feedback area for password updates.
		const passwordStatus = document.querySelector<HTMLDivElement>(
			"#settings-password-status",
		);
		// New password input field.
		const passwordInput =
			document.querySelector<HTMLInputElement>("#settings-password");
		// Confirmation input for the new password.
		const confirmPasswordInput = document.querySelector<HTMLInputElement>(
			"#settings-confirm-password",
		);
		// Inline error message for password validation.
		const passwordError = document.querySelector<HTMLDivElement>(
			"#settings-password-error",
		);
		// Inline error message for confirmation mismatch.
		const confirmPasswordError = document.querySelector<HTMLDivElement>(
			"#settings-confirm-password-error",
		);
		// Password strength indicator element.
		const passwordStrength = document.querySelector<HTMLDivElement>(
			"#settings-password-strength",
		);
		// Avatar update form element.
		const avatarForm = document.querySelector<HTMLFormElement>(
			"#settings-avatar-form",
		);
		// Label wrapper for the "photo" avatar option.
		const avatarOptionPhoto = document.querySelector<HTMLLabelElement>(
			"#settings-avatar-option-photo",
		);
		// Label wrapper for the "initials" avatar option.
		const avatarOptionInitials = document.querySelector<HTMLLabelElement>(
			"#settings-avatar-option-initials",
		);
		// Radio input for choosing the photo avatar.
		const avatarChoicePhoto = document.querySelector<HTMLInputElement>(
			"#settings-avatar-choice-photo",
		);
		// Radio input for choosing the initials avatar.
		const avatarChoiceInitials = document.querySelector<HTMLInputElement>(
			"#settings-avatar-choice-initials",
		);
		// File input used to upload an avatar image.
		const avatarInput = document.querySelector<HTMLInputElement>(
			"#settings-avatar-file",
		);
		// Text element showing the selected avatar filename.
		const avatarFileName = document.querySelector<HTMLSpanElement>(
			"#settings-avatar-file-name",
		);
		// Status/feedback area for avatar updates.
		const avatarStatus = document.querySelector<HTMLDivElement>(
			"#settings-avatar-status",
		);
		// Avatar preview container for the selected image.
		const avatarDisplay = document.querySelector<HTMLDivElement>(
			"#settings-avatar-display",
		);
		// Avatar initials preview element.
		const avatarInitials = document.querySelector<HTMLSpanElement>(
			"#settings-avatar-initials",
		);
		// Back link for returning to the profile page.
		const backLink =
			document.querySelector<HTMLAnchorElement>("#settings-back");

		// 2) Back navigation: use history when possible, fall back to link href.
		if (backLink) {
			const backFallback = backLink.getAttribute("href") ?? "/profile";
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

		// 3) Guard: if any required element is missing, abort wiring.
		if (
			!usernameForm ||
			!usernamePicker ||
			!usernameStatus ||
			!usernameHidden ||
			!passwordForm ||
			!passwordStatus ||
			!passwordInput ||
			!confirmPasswordInput ||
			!passwordError ||
			!confirmPasswordError ||
			!passwordStrength ||
			!avatarForm ||
			!avatarOptionPhoto ||
			!avatarOptionInitials ||
			!avatarChoicePhoto ||
			!avatarChoiceInitials ||
			!avatarInput ||
			!avatarFileName ||
			!avatarStatus ||
			!avatarDisplay ||
			!avatarInitials
		)
			return;

		// 4) Shared session-backed user state for child controllers.
		let currentUser = sessionService.loadUser();
		const userState = {
			getUser: () => currentUser,
			setUser: (user: PublicUser, notify = true) => {
				currentUser = user;
				sessionService.saveUser(user, notify);
			},
		};

		// 5) Initialize avatar picker, username updater, and password updater.
		const avatarController = initSettingsAvatar({
			form: avatarForm,
			optionPhoto: avatarOptionPhoto,
			optionInitials: avatarOptionInitials,
			choicePhoto: avatarChoicePhoto,
			choiceInitials: avatarChoiceInitials,
			input: avatarInput,
			fileName: avatarFileName,
			status: avatarStatus,
			display: avatarDisplay,
			initials: avatarInitials,
			userState,
		});

		initSettingsUsername({
			form: usernameForm,
			picker: usernamePicker,
			status: usernameStatus,
			hiddenInput: usernameHidden,
			userState,
			onUserUpdated: avatarController.updateUser,
		});

		initSettingsPassword({
			form: passwordForm,
			status: passwordStatus,
			passwordInput,
			confirmPasswordInput,
			passwordError,
			confirmPasswordError,
			passwordStrength,
			userState,
		});
	};

	return { html, onMount };
};

export default Settings;
