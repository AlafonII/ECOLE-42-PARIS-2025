import { userService } from "./user.service";
import { setFormDisabled, setFormStatus } from "../utils/formHelpers";
import {
	resolveAvatarChoice,
	resolveAvatarUrl,
	setAvatarChoice,
	validateAvatarFile,
} from "../utils/avatar";
import type { PublicUser } from "./types";

type SettingsUserState = {
	getUser: () => PublicUser | null;
	setUser: (user: PublicUser, notify?: boolean) => void;
};

type SettingsAvatarOptions = {
	form: HTMLFormElement;
	optionPhoto: HTMLLabelElement;
	optionInitials: HTMLLabelElement;
	choicePhoto: HTMLInputElement;
	choiceInitials: HTMLInputElement;
	input: HTMLInputElement;
	fileName: HTMLSpanElement;
	status: HTMLDivElement;
	display: HTMLDivElement;
	initials: HTMLSpanElement;
	userState: SettingsUserState;
};

export type SettingsAvatarController = {
	updateUser: (user: PublicUser) => void;
};

export const initSettingsAvatar = ({
	form,
	optionPhoto,
	optionInitials,
	choicePhoto,
	choiceInitials,
	input,
	fileName,
	status,
	display,
	initials,
	userState,
}: SettingsAvatarOptions): SettingsAvatarController => {
	// Local state for current user and avatar preview.
	let currentUser = userState.getUser();
	let previewUrl: string | null = null;

	// UI helpers for selection styles and preview rendering.
	const renderInitials = (username: string) =>
		username.slice(0, 2).toUpperCase();

	const setAvatarSelectionStyles = () => {
		const isPhotoSelected = choicePhoto.checked;
		optionPhoto.classList.toggle("ring-2", isPhotoSelected);
		optionPhoto.classList.toggle("ring-[var(--fg)]", isPhotoSelected);
		optionInitials.classList.toggle("ring-2", !isPhotoSelected);
		optionInitials.classList.toggle("ring-[var(--fg)]", !isPhotoSelected);
	};

	const setFileHintVisible = (visible: boolean) => {
		fileName.classList.toggle("hidden", !visible);
		input.classList.toggle("text-transparent", visible);
	};

	const renderPhotoDisplay = (url: string | null) => {
		if (!url) {
			display.innerHTML = `<span class="flex h-full w-full items-center justify-center bg-black"><svg
    			class="h-8 w-8 text-[var(--fg)] opacity-60"
    			xmlns="http://www.w3.org/2000/svg"
    			fill="none"
    			viewBox="0 0 24 24"
    			stroke="currentColor"
  				>
    			<path
      				stroke-linecap="round"
      				stroke-linejoin="round"
      				stroke-width="1.5"
      				d="M3 16l4-4a3 3 0 014 0l5 5M14 14l1-1a3 3 0 014 0l2 2M9 7h.01M4 19h16a1 1 0 001-1V6a1 1 0 00-1-1H4a1 1 0 00-1 1v12a1 1 0 001 1z"
    			/>
  				</svg></span>`;
			return;
		}
		display.innerHTML = `<img src="${url}" alt="Avatar preview" class="h-full w-full object-cover" />`;
	};

	// Sync the form UI with the current user (or disable when signed out).
	const applyUserState = (user: PublicUser | null) => {
		currentUser = user;
		if (!currentUser) {
			setFormDisabled(form, true);
			setFormStatus(status, "Sign in to update your avatar.", "error");
			return;
		}

		setFormDisabled(form, false);
		initials.textContent = renderInitials(currentUser.username);
		renderPhotoDisplay(resolveAvatarUrl(currentUser.avatar_url));

		const resolvedChoice = resolveAvatarChoice(currentUser);
		if (resolvedChoice === "photo" && currentUser.avatar_url) {
			choicePhoto.checked = true;
		} else {
			choiceInitials.checked = true;
		}
		setAvatarSelectionStyles();
		setFileHintVisible(true);
	};

	// Initial render based on the stored session user.
	applyUserState(currentUser);

	// Revoke any blob URLs when replacing previews.
	const resetPreview = () => {
		if (previewUrl && previewUrl.startsWith("blob:")) {
			URL.revokeObjectURL(previewUrl);
		}
		previewUrl = null;
	};

	// Keep selection ring styles in sync with radio buttons.
	choicePhoto.addEventListener("change", setAvatarSelectionStyles);
	choiceInitials.addEventListener("change", setAvatarSelectionStyles);

	// File input change: preview the selected image and update choice state.
	input.addEventListener("change", () => {
		setFormStatus(status, "", "idle");
		resetPreview();
		const file = input.files?.[0];
		if (file) {
			const reader = new FileReader();
			reader.onload = () => {
				if (typeof reader.result === "string") {
					previewUrl = reader.result;
					renderPhotoDisplay(previewUrl);
					choicePhoto.checked = true;
					setAvatarSelectionStyles();
					setFileHintVisible(false);
				}
			};
			reader.onerror = () => {
				resetPreview();
				renderPhotoDisplay(
					resolveAvatarUrl(currentUser?.avatar_url ?? null),
				);
				setFileHintVisible(true);
				setFormStatus(status, "Unable to preview this file.", "error");
			};
			reader.readAsDataURL(file);
		} else {
			renderPhotoDisplay(
				resolveAvatarUrl(currentUser?.avatar_url ?? null),
			);
			setFileHintVisible(true);
		}
	});

	// Submit: either switch to initials or upload/use a photo.
	form.addEventListener("submit", async (event) => {
		event.preventDefault();

		const user = userState.getUser();
		if (!user) {
			setFormStatus(status, "Sign in to update your avatar.", "error");
			return;
		}
		currentUser = user;

		// Branch 1: use initials instead of a photo.
		if (choiceInitials.checked) {
			setFormStatus(status, "Updating avatar...", "loading");
			setFormDisabled(form, true);

			const result = await userService.updateUser(currentUser.id, {
				avatar_url: null,
			});

			setFormDisabled(form, false);

			if (!result.success || !result.data) {
				setFormStatus(
					status,
					result.error ?? "Failed to update avatar.",
					"error",
				);
				return;
			}

			currentUser = result.data;
			userState.setUser(result.data, false);
			setAvatarChoice(currentUser.id, "initials");
			resetPreview();
			input.value = "";
			setFileHintVisible(true);
			initials.textContent = renderInitials(currentUser.username);
			renderPhotoDisplay(resolveAvatarUrl(currentUser.avatar_url));
			setFormStatus(status, "Initials selected.", "success");
			return;
		}

		// Branch 2: use a photo (existing or new upload).
		const file = input.files?.[0];
		if (!file && !currentUser.avatar_url) {
			setFormStatus(status, "Select an image to upload.", "error");
			return;
		}

		if (file) {
			const validationError = validateAvatarFile(file);
			if (validationError) {
				setFormStatus(status, validationError, "error");
				return;
			}
		}

		// If the user picked "photo" but didn't choose a new file,
		// just persist the choice without uploading.
		if (!file && currentUser.avatar_url) {
			setAvatarChoice(currentUser.id, "photo");
			setFormStatus(status, "Photo selected.", "success");
			return;
		}

		setFormStatus(status, "Uploading avatar...", "loading");
		setFormDisabled(form, true);

		const result = await userService.uploadAvatar(currentUser.id, file!);

		setFormDisabled(form, false);

		if (!result.success || !result.data) {
			setFormStatus(
				status,
				result.error ?? "Failed to upload avatar.",
				"error",
			);
			return;
		}

		currentUser = result.data;
		userState.setUser(result.data, false);
		setAvatarChoice(currentUser.id, "photo");
		resetPreview();
		input.value = "";
		setFileHintVisible(true);
		initials.textContent = renderInitials(currentUser.username);
		renderPhotoDisplay(resolveAvatarUrl(currentUser.avatar_url));
		setFormStatus(status, "Avatar updated.", "success");
	});

	return {
		updateUser: (user: PublicUser) => {
			applyUserState(user);
		},
	};
};
