import { 
    validateEmail, 
    validatePassword, 
    validatePasswordConfirmation,
    type EmailValidationResult,
    type PasswordValidationResult
} from "./validation";
import { setFieldError, clearFormErrors } from "./formHelpers";

/**
 * Result of form validation
 */
export interface FormValidationResult {
    isValid: boolean;
    errors: Array<{ field: string; message: string }>;
    values: {
        email: string;
        password: string;
        confirmPassword?: string;
    };
}

/**
 * Auth form field references
 */
export interface AuthFormFields {
    emailInput: HTMLInputElement | null;
    passwordInput: HTMLInputElement | null;
    confirmPasswordInput?: HTMLInputElement | null;
    emailError: HTMLElement | null;
    passwordError: HTMLElement | null;
    confirmPasswordError?: HTMLElement | null;
}

/**
 * Validate email field and show error if invalid
 * @param emailInput - The email input element
 * @param emailError - The email error message element
 * @returns Validation result
 */
export const validateEmailField = (
    emailInput: HTMLInputElement | null,
    emailError: HTMLElement | null
): EmailValidationResult => {
    const email = emailInput?.value.trim() || "";
    const validation = validateEmail(email);
    
    setFieldError(
        emailInput,
        emailError,
        validation.isValid ? null : (validation.error || "Invalid email")
    );
    
    return validation;
};

/**
 * Validate password field and show error if invalid
 * @param passwordInput - The password input element
 * @param passwordError - The password error message element
 * @returns Validation result
 */
export const validatePasswordField = (
    passwordInput: HTMLInputElement | null,
    passwordError: HTMLElement | null
): PasswordValidationResult => {
    const password = passwordInput?.value || "";
    const validation = validatePassword(password);
    
    setFieldError(
        passwordInput,
        passwordError,
        validation.isValid ? null : validation.errors[0]
    );
    
    return validation;
};

/**
 * Validate password confirmation field and show error if invalid
 * @param passwordInput - The password input element
 * @param confirmPasswordInput - The confirm password input element
 * @param confirmPasswordError - The error message element
 * @returns Error message or null if valid
 */
export const validateConfirmPasswordField = (
    passwordInput: HTMLInputElement | null,
    confirmPasswordInput: HTMLInputElement | null,
    confirmPasswordError: HTMLElement | null
): string | null => {
    const password = passwordInput?.value || "";
    const confirmPassword = confirmPasswordInput?.value || "";
    
    const error = validatePasswordConfirmation(password, confirmPassword);
    
    setFieldError(confirmPasswordInput, confirmPasswordError, error);
    
    return error;
};

/**
 * Validate entire login form
 * @param fields - Form field references
 * @returns Validation result with values
 */
export const validateLoginForm = (fields: AuthFormFields): FormValidationResult => {
    const { emailInput, passwordInput, emailError, passwordError } = fields;
    
    // Clear previous errors
    clearFormErrors(
        [emailInput, passwordInput],
        [emailError, passwordError]
    );
    
    const errors: Array<{ field: string; message: string }> = [];
    
    // Validate email
    const emailValidation = validateEmailField(emailInput, emailError);
    if (!emailValidation.isValid) {
        errors.push({ 
            field: "email", 
            message: emailValidation.error || "Invalid email" 
        });
    }
    
    // Validate password presence (no strength check for login)
    const password = passwordInput?.value.trim() || "";
    if (!password) {
        const message = "Password is required";
        setFieldError(passwordInput, passwordError, message);
        errors.push({ field: "password", message });
    }
    
    return {
        isValid: errors.length === 0,
        errors,
        values: {
            email: emailInput?.value.trim() || "",
            password: password,
        }
    };
};

/**
 * Validate entire signup form
 * @param fields - Form field references
 * @returns Validation result with values
 */
export const validateSignupForm = (fields: AuthFormFields): FormValidationResult => {
    const { 
        emailInput, 
        passwordInput, 
        confirmPasswordInput,
        emailError, 
        passwordError,
        confirmPasswordError 
    } = fields;
    
    // Clear previous errors - explicitly handle undefined
    clearFormErrors(
        [emailInput, passwordInput, confirmPasswordInput ?? null],
        [emailError, passwordError, confirmPasswordError ?? null]
    );
    
    const errors: Array<{ field: string; message: string }> = [];
    
    // Validate email
    const emailValidation = validateEmailField(emailInput, emailError);
    if (!emailValidation.isValid) {
        errors.push({ 
            field: "email", 
            message: emailValidation.error || "Invalid email" 
        });
    }
    
    // Validate password (full strength check)
    const passwordValidation = validatePasswordField(passwordInput, passwordError);
    if (!passwordValidation.isValid) {
        errors.push({ 
            field: "password", 
            message: passwordValidation.errors[0] || "Invalid password" 
        });
    }
    
    // Validate password confirmation
    const confirmError = validateConfirmPasswordField(
        passwordInput,
        confirmPasswordInput ?? null,
        confirmPasswordError ?? null
    );
    if (confirmError) {
        errors.push({ 
            field: "confirmPassword", 
            message: confirmError 
        });
    }
    
    return {
        isValid: errors.length === 0,
        errors,
        values: {
            email: emailInput?.value.trim() || "",
            password: passwordInput?.value.trim() || "",
            confirmPassword: confirmPasswordInput?.value.trim() || "",
        }
    };
};