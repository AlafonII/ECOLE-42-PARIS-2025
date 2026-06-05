/**
 * CSS class for error border styling
 */
export const ERROR_BORDER_CLASS = "border-[rgba(255,114,114,0.4)]";

/**
 * Show error message in an element
 * @param element - The HTML element to display the error in
 * @param message - The error message to display
 */
export const showError = (element: HTMLElement | null, message: string): void => {
    if (!element) return;
    element.textContent = message;
    element.classList.remove("hidden");
};

/**
 * Hide error message from an element
 * @param element - The HTML element to hide
 */
export const hideError = (element: HTMLElement | null): void => {
    if (!element) return;
    element.classList.add("hidden");
};

/**
 * Set or remove error styling from an input element
 * @param input - The input element to style
 * @param hasError - Whether to show or hide the error state
 */
export const setInputError = (
    input: HTMLInputElement | null,
    hasError: boolean
): void => {
    if (!input) return;
    
    if (hasError) {
        input.classList.add(ERROR_BORDER_CLASS);
    } else {
        input.classList.remove(ERROR_BORDER_CLASS);
    }
};

/**
 * Combined function to manage both error message and input styling
 * @param input - The input element
 * @param errorEl - The error message element
 * @param message - The error message (null to clear)
 */
export const setFieldError = (
    input: HTMLInputElement | null,
    errorEl: HTMLElement | null,
    message: string | null
): void => {
    if (!input || !errorEl) return;
    
    if (message) {
        showError(errorEl, message);
        setInputError(input, true);
    } else {
        hideError(errorEl);
        setInputError(input, false);
    }
};

/**
 * Form status types
 */
export type FormStatus = "idle" | "loading" | "success" | "error";

/**
 * Set form status message with appropriate styling
 * @param statusElement - The status container element
 * @param message - The status message
 * @param variant - The status type (idle, loading, success, error)
 */
export const setFormStatus = (
    statusElement: HTMLElement | null,
    message: string,
    variant: FormStatus = "idle"
): void => {
    if (!statusElement) return;
    
    const baseClasses = "rounded-lg border px-3 py-2 text-sm font-medium shadow-[0_10px_35px_rgba(0,0,0,0.4)] transition";
    
    const variants: Record<FormStatus, string> = {
        idle: "hidden",
        loading: "block border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.12)] text-[var(--fg)]",
        success: "block border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.18)] text-[var(--fg)]",
        error: "block border-[rgba(255,114,114,0.4)] bg-[rgba(255,114,114,0.12)] text-[#ffd6d6]",
    };
    
    statusElement.className = `${baseClasses} ${variants[variant]}`;
    statusElement.textContent = message;
};

/**
 * Clear all errors from form inputs and error elements
 * @param inputs - Array of input elements to clear
 * @param errors - Array of error message elements to hide
 */
export const clearFormErrors = (
    inputs: (HTMLInputElement | null)[],
    errors: (HTMLElement | null)[]
): void => {
    inputs.forEach(input => setInputError(input, false));
    errors.forEach(error => hideError(error));
};

/**
 * Enable or disable all form inputs and buttons
 * @param form - The form element
 * @param disabled - Whether to disable or enable
 */
export const setFormDisabled = (
    form: HTMLFormElement | null,
    disabled: boolean
): void => {
    if (!form) return;
    
    const elements = form.querySelectorAll<HTMLInputElement | HTMLButtonElement>(
        "input, button"
    );
    elements.forEach(element => {
        element.disabled = disabled;
    });
};