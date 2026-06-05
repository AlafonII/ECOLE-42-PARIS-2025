import { userService } from "./user.service";
import { setFormDisabled, setFormStatus } from "../utils/formHelpers";
import type { PublicUser } from "./types";

type SettingsUserState = {
	getUser: () => PublicUser | null;
	setUser: (user: PublicUser, notify?: boolean) => void;
};

type SettingsUsernameOptions = {
	form: HTMLFormElement;
	picker: HTMLDivElement;
	status: HTMLDivElement;
	hiddenInput: HTMLInputElement;
	userState: SettingsUserState;
	onUserUpdated?: (user: PublicUser) => void;
};

export const initSettingsUsername = ({
	form,
	picker,
	status,
	hiddenInput,
	userState,
	onUserUpdated,
}: SettingsUsernameOptions): void => {
	let currentUser = userState.getUser();

	if (!currentUser) {
		// Disable the form when signed out.
		setFormDisabled(form, true);
		setFormStatus(status, "Sign in to update your username.", "error");
	}

	// Allowed characters and active slot index.
	const alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789".split("");
	let activeSlot = 0;

	const normalizeUsername = (username: string) =>
		(username || "AAAA")
			.toUpperCase()
			.replace(/[^A-Z0-9]/g, "")
			.padEnd(4, "A")
			.slice(0, 4);

	let slots = normalizeUsername(currentUser?.username ?? "AAAA").split("");

	// Render the picker UI and sync the hidden input value.
	const renderSlots = () => {
		const slotEls = form.querySelectorAll<HTMLElement>(".username-slot");
		slotEls.forEach((el, idx) => {
			el.textContent = slots[idx];
			if (idx === activeSlot) {
				el.classList.add("active");
			} else {
				el.classList.remove("active");
			}
		});
		hiddenInput.value = slots.join("");
	};

	// Rotate the active slot forward/backward within the alphabet.
	const adjustSlot = (delta: number) => {
		const current = alphabet.indexOf(slots[activeSlot]);
		const next = (current + delta + alphabet.length) % alphabet.length;
		slots[activeSlot] = alphabet[next];
		renderSlots();
	};

	// Move focus to a specific slot (clamped to bounds).
	const setActiveSlot = (idx: number) => {
		activeSlot = Math.max(0, Math.min(slots.length - 1, idx));
		renderSlots();
	};

	// Button controls for slot increment/decrement.
	form
		.querySelectorAll<HTMLButtonElement>(".username-up")
		.forEach((btn) =>
			btn.addEventListener("click", () => {
				const idx = Number(btn.dataset.index ?? "0");
				setActiveSlot(idx);
				adjustSlot(1);
			})
		);

	form
		.querySelectorAll<HTMLButtonElement>(".username-down")
		.forEach((btn) =>
			btn.addEventListener("click", () => {
				const idx = Number(btn.dataset.index ?? "0");
				setActiveSlot(idx);
				adjustSlot(-1);
			})
		);

	// Click to select a slot.
	form
		.querySelectorAll<HTMLElement>(".username-slot")
		.forEach((slot) =>
			slot.addEventListener("click", () => {
				const idx = Number(slot.dataset.index ?? "0");
				setActiveSlot(idx);
			})
		);

	// Keyboard shortcuts (WASD/arrows) for slot navigation.
	const handleUsernameKeydown = (event: KeyboardEvent) => {
		if (!document.body.contains(form)) return;
		const target = event.target as HTMLElement | null;
		if (
			target &&
			(target.tagName === "INPUT" ||
				target.tagName === "TEXTAREA" ||
				target.isContentEditable)
		)
			return;
		const key = event.key.toLowerCase();
		if (["arrowup", "w"].includes(key)) {
			event.preventDefault();
			adjustSlot(-1);
		} else if (["arrowdown", "s"].includes(key)) {
			event.preventDefault();
			adjustSlot(1);
		} else if (["arrowleft", "a"].includes(key)) {
			event.preventDefault();
			setActiveSlot((activeSlot - 1 + slots.length) % slots.length);
		} else if (["arrowright", "d"].includes(key)) {
			event.preventDefault();
			setActiveSlot((activeSlot + 1) % slots.length);
		}
	};

	// Global key handler only affects the active settings form.
	document.addEventListener("keydown", handleUsernameKeydown);

	// Initial render on mount.
	renderSlots();

	// Mouse wheel over the picker cycles the active slot.
	picker.addEventListener(
		"wheel",
		(event) => {
			event.preventDefault();
			const target = event.target as HTMLElement | null;
			const slot = target?.closest<HTMLElement>(".username-slot");
			if (slot?.dataset.index) {
				setActiveSlot(Number(slot.dataset.index));
			}
			const direction = event.deltaY > 0 ? 1 : -1;
			adjustSlot(direction);
		},
		{ passive: false }
	);

	// Submit: validate selection, check availability, then save.
	form.addEventListener("submit", async (event) => {
		event.preventDefault();

		const user = userState.getUser();
		if (!user) {
			setFormStatus(status, "Sign in to update your username.", "error");
			return;
		}

		const newUsername = hiddenInput.value.trim();
		if (!newUsername) {
			setFormStatus(status, "Please choose a username.", "error");
			return;
		}

		if (newUsername === user.username) {
			setFormStatus(status, "Username is unchanged.", "success");
			return;
		}

		setFormDisabled(form, true);
		setFormStatus(status, "Checking availability...", "loading");

		try {
			// Ensure the username isn't already taken before updating.
			const check = await userService.getUserByUsername(newUsername);
			if (check.success && check.data) {
				setFormStatus(status, "Username is already taken.", "error");
				return;
			}

			if (
				!check.success &&
				check.error &&
				check.error !== "User not found" &&
				check.error !== "Ressource not found"
			) {
				setFormStatus(status, check.error, "error");
				return;
			}

			const result = await userService.updateUser(user.id, {
				username: newUsername,
			});

			if (result.success && result.data) {
				currentUser = result.data;
				userState.setUser(result.data);
				slots = normalizeUsername(result.data.username).split("");
				renderSlots();
				onUserUpdated?.(result.data);
				setFormStatus(status, "Username updated.", "success");
				return;
			}

			setFormStatus(
				status,
				result.error ?? "Unable to update username.",
				"error"
			);
		} catch (error) {
			console.error("Username update failed:", error);
			setFormStatus(status, "Unable to update username.", "error");
		} finally {
			setFormDisabled(form, false);
		}
	});
};
