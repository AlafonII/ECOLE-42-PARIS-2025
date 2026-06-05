const EMAIL_REGEX =
	/^[a-zA-Z0-9](?!.*\.\.)[a-zA-Z0-9._-]*[a-zA-Z0-9]@[a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*\.[a-zA-Z]{2,}$/;

const DEFAULT_PASSWORD_REQUIREMENTS: PasswordRequirements = {
	minLength: 8,
	requireUppercase: true,
	requireLowercase: true,
	requireNumber: true,
	requireSpecialChar: false,
};

export interface EmailValidationResult {
	isValid: boolean;
	error?: string;
}

export interface PasswordRequirements {
	minLength: number;
	requireUppercase: boolean;
	requireLowercase: boolean;
	requireNumber: boolean;
	requireSpecialChar: boolean;
}

export interface PasswordValidationResult {
	isValid: boolean;
	errors: string[];
	strength: "weak" | "medium" | "strong";
}

/**
 * Validate email format
 */
export function validateEmail(email: string): EmailValidationResult {
	const trimmed = email.trim();

	if (!trimmed) {
		return {
			isValid: false,
			error: "Email is required",
		};
	}

	if (!EMAIL_REGEX.test(trimmed)) {
		return {
			isValid: false,
			error: "Invalid email format (e.g., user@example.com)",
		};
	}

	if (trimmed.length > 100) {
		return {
			isValid: false,
			error: "Email is too long (max 100 characters)",
		};
	}
	
	return {
		isValid: true,
	};
}

/**
 * Validate password against requirements
 */
export function validatePassword(
	password: string,
	requirements: PasswordRequirements = DEFAULT_PASSWORD_REQUIREMENTS
): PasswordValidationResult {
	const errors: string[] = [];

	if (password.length < requirements.minLength) {
		errors.push(
			`Password must be at least ${requirements.minLength} characters`
		);
	}

	if (requirements.requireUppercase && !/[A-Z]/.test(password)) {
		errors.push("Password must contain at least one uppercase letter");
	}

	if (requirements.requireLowercase && !/[a-z]/.test(password)) {
		errors.push("Password must contain at least one lowercase letter");
	}

	if (requirements.requireNumber && !/\d/.test(password)) {
		errors.push("Password must contain at least one number");
	}

	if (
		requirements.requireSpecialChar &&
		!/[!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]/.test(password)
	) {
		errors.push("Password must contain at least one special character");
	}

	let strength: "weak" | "medium" | "strong" = "weak";

	if (errors.length === 0) {
		const hasUpperLower = /[A-Z]/.test(password) && /[a-z]/.test(password);
		const hasNumber = /\d/.test(password);
		const hasSpecial = /[!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?]/.test(
			password
		);
		const isLongEnough = password.length >= 12;

		if (hasUpperLower && hasNumber && hasSpecial && isLongEnough) {
			strength = "strong";
		} else if (hasUpperLower && hasNumber) {
			strength = "medium";
		}
	}

	return {
		isValid: errors.length === 0,
		errors,
		strength,
	};
}

/**
 * Validate password confirmation
 */
export function validatePasswordConfirmation(
	password: string,
	confirmPassword: string
): string | null {
	if (password !== confirmPassword) {
		return "Passwords do not match";
	}
	return null;
}

/**
 * Get password strength color classes
 */
export function getPasswordStrengthColor(
	strength: "weak" | "medium" | "strong"
): string {
	const colors = {
		weak: "border-[rgba(255,114,114,0.4)] bg-[rgba(255,114,114,0.12)] text-[#ffd6d6]",
		medium: "border-[rgba(255,200,87,0.4)] bg-[rgba(255,200,87,0.12)] text-[#ffe9c2]",
		strong: "border-[rgba(134,239,172,0.4)] bg-[rgba(134,239,172,0.12)] text-[#d4fce3]",
	};
	return colors[strength];
}

/**
 * Get password strength label
 */
export function getPasswordStrengthLabel(
	strength: "weak" | "medium" | "strong"
): string {
	const labels = {
		weak: "WEAK",
		medium: "MEDIUM",
		strong: "STRONG",
	};
	return labels[strength];
}
