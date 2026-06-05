import { userService } from "./user.service";
import {
	getPasswordStrengthColor,
	getPasswordStrengthLabel,
	validatePassword,
	validatePasswordConfirmation,
} from "../utils/validation";
import {
	hideError,
	setFormDisabled,
	setFormStatus,
	setInputError,
	showError,
} from "../utils/formHelpers";
import type { PublicUser } from "./types";

type SettingsUserState = {
	getUser: () => PublicUser | null;
	setUser: (user: PublicUser, notify?: boolean) => void;
};

type SettingsPasswordOptions = {
	form: HTMLFormElement;
	status: HTMLDivElement;
	passwordInput: HTMLInputElement;
	confirmPasswordInput: HTMLInputElement;
	passwordError: HTMLDivElement;
	confirmPasswordError: HTMLDivElement;
	passwordStrength: HTMLDivElement;
	userState: SettingsUserState;
};

export const initSettingsPassword = ({
	form,
	status,
	passwordInput,
	confirmPasswordInput,
	passwordError,
	confirmPasswordError,
	passwordStrength,
	userState,
}: SettingsPasswordOptions): void => {
	const currentUser = userState.getUser();

	if (!currentUser) {
		// Disable the form when there is no authenticated user.
		setFormDisabled(form, true);
		setFormStatus(status, "Sign in to update your password.", "error");
	}

	// Live strength + validation feedback for the new password field.
	passwordInput.addEventListener("input", () => {
		if (!passwordInput.value) {
			hideError(passwordError);
			setInputError(passwordInput, false);
			passwordStrength.classList.add("hidden");
			return;
		}

		const validation = validatePassword(passwordInput.value);
		const strengthColor = getPasswordStrengthColor(validation.strength);
		const strengthLabel = getPasswordStrengthLabel(validation.strength);
		passwordStrength.className = `rounded px-2 py-1 text-xs font-semibold uppercase ${strengthColor}`;
		passwordStrength.textContent = `Strength: ${strengthLabel}`;
		passwordStrength.classList.remove("hidden");

		if (!validation.isValid && validation.errors.length > 0) {
			showError(passwordError, validation.errors[0]);
			setInputError(passwordInput, true);
		} else {
			hideError(passwordError);
			setInputError(passwordInput, false);
		}
	});

	// Live confirmation check when the user types the confirmation.
	confirmPasswordInput.addEventListener("input", () => {
		if (!confirmPasswordInput.value) return;
		const confirmError = validatePasswordConfirmation(
			passwordInput.value,
			confirmPasswordInput.value
		);
		if (confirmError) {
			showError(confirmPasswordError, confirmError);
			setInputError(confirmPasswordInput, true);
		} else {
			hideError(confirmPasswordError);
			setInputError(confirmPasswordInput, false);
		}
	});

	// Submit handler: validate inputs, then call the API.
	form.addEventListener("submit", async (event) => {
		event.preventDefault();

		const user = userState.getUser();
		if (!user) {
			setFormStatus(status, "Sign in to update your password.", "error");
			return;
		}

		hideError(passwordError);
		hideError(confirmPasswordError);
		setInputError(passwordInput, false);
		setInputError(confirmPasswordInput, false);

		// Validate password rules and confirmation before sending.
		const password = passwordInput.value.trim();
		const confirmPassword = confirmPasswordInput.value.trim();
		const validation = validatePassword(password);

		if (!password) {
			showError(passwordError, "Password is required");
			setInputError(passwordInput, true);
			setFormStatus(status, "Please fix the errors above.", "error");
			return;
		}

		if (!validation.isValid) {
			showError(passwordError, validation.errors[0] ?? "Invalid password");
			setInputError(passwordInput, true);
			setFormStatus(status, "Please fix the errors above.", "error");
			return;
		}

		const confirmError = validatePasswordConfirmation(
			password,
			confirmPassword
		);
		if (confirmError) {
			showError(confirmPasswordError, confirmError);
			setInputError(confirmPasswordInput, true);
			setFormStatus(status, "Please fix the errors above.", "error");
			return;
		}

		setFormDisabled(form, true);
		setFormStatus(status, "Updating password...", "loading");

		// Persist the password update and refresh session data.
		try {
			const result = await userService.updateUser(user.id, {
				password,
			});
			if (result.success && result.data) {
				userState.setUser(result.data);
				form.reset();
				passwordStrength.classList.add("hidden");
				setFormStatus(status, "Password updated.", "success");
				return;
			}

			setFormStatus(status, result.error ?? "Unable to update password.", "error");
		} catch (error) {
			console.error("Password update failed:", error);
			setFormStatus(status, "Unable to update password.", "error");
		} finally {
			setFormDisabled(form, false);
		}
	});
};
