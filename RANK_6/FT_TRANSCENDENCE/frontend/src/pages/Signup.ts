import signupTitle from "../assets/images/page_titles/SIGNUP_PAGE.png";
import arrowBack from "../assets/images/page_assets/ARROW_BACK.png";
import UsernameSlot from "../components/UsernameSlot";
import {
	validatePassword,
	getPasswordStrengthColor,
	getPasswordStrengthLabel,
	validatePasswordConfirmation,
} from "../utils/validation";
import { setFormStatus, setInputError, hideError } from "../utils/formHelpers";
import { validateEmailField } from "../utils/formValidators";
import { userService } from "../services/user.service";
import { sessionService } from "../services/session.service";
import { presenceService } from "../services/presence.service";
import { verifyOTP } from "../services/api";

const Signup = () => {
	const usernameSlots = ["first", "second", "third", "fourth"]
		.map((ordinal, index) =>
			UsernameSlot(index, ordinal, {
				gapClass: "gap-3",
				imageClass: "h-12 w-12",
				slotClass: "text-5xl",
			}),
		)
		.join("");
	const html = `
    <section class="retro-card crt space-y-6 p-8">
      <a
        id="signup-back"
        data-link
        href="/login"
        class="fixed left-6 top-6 z-20 flex items-center gap-3 text-base font-semibold uppercase text-[var(--fg)]"
        aria-label="Back"
      >
        <img src="${arrowBack}" alt="" class="h-7 w-auto" />
        <span>Back</span>
      </a>
      <div class="space-y-2">
        <div class="flex justify-center">
          <img src="${signupTitle}" alt="Signup" class="mx-auto h-auto max-w-full" />
        </div>
      </div>
      <form id="signup-form" class="space-y-6" novalidate>
        <div id="signup-step-1" class="space-y-4">
          <div class="flex justify-center">
            ${usernameSlots}
          </div>
          <input type="hidden" name="username" id="username-hidden" value="AAAA" />
          <button type="button" id="signup-next" class="retro-btn w-full justify-center">
            Next
          </button>
        </div>

        <div id="signup-step-2" class="hidden space-y-4">
          <label class="block space-y-1">
            <span class="text-sm font-semibold text-[var(--fg)]">Email</span>
            <input
              type="email"
              name="email"
              id="signup-email"
              class="w-full rounded-lg border border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.03)] px-3 py-2 text-[var(--fg)] placeholder:text-[rgba(141,161,197,0.75)] focus:border-[var(--fg)] focus:outline-none focus:ring-2 focus:ring-[rgba(230,240,255,0.25)]"
              placeholder="you@example.com"
            />
            <div id="email-error" class="hidden text-xs text-[#ffd6d6]"></div>
          </label>
          
          <label class="block space-y-1">
            <span class="text-sm font-semibold text-[var(--fg)]">Password</span>
            <input
              type="password"
              name="password"
              id="signup-password"
              class="w-full rounded-lg border border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.03)] px-3 py-2 text-[var(--fg)] placeholder:text-[rgba(141,161,197,0.75)] focus:border-[var(--fg)] focus:outline-none focus:ring-2 focus:ring-[rgba(230,240,255,0.25)]"
              placeholder="••••••••"
            />
            <div id="password-strength" class="hidden rounded px-2 py-1 text-xs font-semibold uppercase"></div>
            <div id="password-error" class="hidden text-xs text-[#ffd6d6]"></div>
            <div class="text-xs text-[var(--muted)]">
              <p class="font-semibold mb-1">Required:</p>
              <ul class="list-disc list-inside space-y-0.5 ml-1">
                <li>8+ characters</li>
                <li>One uppercase (A-Z)</li>
                <li>One lowercase (a-z)</li>
                <li>One number (0-9)</li>
              </ul>
            </div>
          </label>
          
		  <label class="block space-y-1">
			  <span class="text-sm font-semibold text-[var(--fg)]">Confirm Password</span>
		  <input
			  type="password"
			  name="confirmPassword"
			  id="signup-confirm-password"
			  class="w-full rounded-lg border border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.03)] px-3 py-2 text-[var(--fg)] placeholder:text-[rgba(141,161,197,0.75)] focus:border-[var(--fg)] focus:outline-none focus:ring-2 focus:ring-[rgba(230,240,255,0.25)]"
			  placeholder="••••••••"
		  />
		  <div id="confirm-password-error" class="hidden text-xs text-[#ffd6d6]"></div>
		  </label>
		  <label class="flex items-center gap-2 text-[var(--fg)] cursor-pointer">
			  <input type="checkbox" name="twoFactor" id="signup-twoFactor" value="1" class="h-4 w-4 rounded border
				  border-[rgba(230,240,255,0.35)]
				  bg-transparent
				  text-[var(--fg)]
				  focus:ring-0 focus:outline-none
				  accent-[var(--fg)]"
			  />

			  <span class="text-sm select-none">
				  Enable double authentication
			  </span>
		  </label>
          
          <div class="flex justify-center">
            <button
              type="submit"
              class="retro-btn justify-center disabled:cursor-not-allowed disabled:opacity-70"
            >
              Create account
            </button>
          </div>
        </div>
      </form>
      <div id="signup-status" class="hidden rounded-lg border px-3 py-2 text-sm font-medium shadow-[0_10px_35px_rgba(0,0,0,0.4)]" role="status"></div>
      <p class="text-sm text-[var(--muted)]">
        Already registered?
        <a data-link href="/login" class="retro-link font-semibold">Sign in</a>.
      </p>
	  <div id="otp-modal" class="hidden fixed inset-0 z-50 flex items-center justify-center bg-black">
		  <div class="retro-card space-y-4 p-6 w-full max-w-sm">
			  <h2 class="text-lg font-semibold text-center">
				  Two-Factor Authentication
			  </h2>

			  <p class="text-sm text-center text-[var(--muted)]">
				  Enter the verification code sent to your email.
			  </p>

			  <form id="otp-form" class="space-y-3">
				  <input
					  type="password"
					  id="otp-input"
					  maxlength="6"
					  class="w-full rounded-lg border px-3 py-2 text-center tracking-widest text-black"
					  placeholder="••••••••"
					  autocomplete="one-time-code"
				  />

				  <div id="otp-error" class="hidden text-xs text-[#ffd6d6] text-center"></div>

				  <button type="submit" class="retro-btn w-full justify-center">Verify</button>
				  <button type="button" id="otp-cancel" class="retro-btn w-full justify-center bg-[rgba(255,114,114,0.8)] hover:bg-[rgba(255,114,114,1)]">
					  Cancel
				  </button>
			  </form>
		  </div>
	  </div>
    </section>
  `;

	const onMount = () => {
		const form = document.querySelector<HTMLFormElement>("#signup-form");
		const status = document.querySelector<HTMLDivElement>("#signup-status");
		const submitButton = form?.querySelector<HTMLButtonElement>(
			'button[type="submit"]',
		);
		const step1 = document.querySelector<HTMLDivElement>("#signup-step-1");
		const step2 = document.querySelector<HTMLDivElement>("#signup-step-2");
		const nextButton =
			document.querySelector<HTMLButtonElement>("#signup-next");
		const usernameHidden =
			document.querySelector<HTMLInputElement>("#username-hidden");
		const backLink =
			document.querySelector<HTMLAnchorElement>("#signup-back");

		const emailInput =
			document.querySelector<HTMLInputElement>("#signup-email");
		const passwordInput =
			document.querySelector<HTMLInputElement>("#signup-password");
		const confirmPasswordInput = document.querySelector<HTMLInputElement>(
			"#signup-confirm-password",
		);
		const twoFactorInput =
			document.querySelector<HTMLInputElement>("#signup-twoFactor");

		const emailError =
			document.querySelector<HTMLDivElement>("#email-error");
		const passwordError =
			document.querySelector<HTMLDivElement>("#password-error");
		const confirmPasswordError = document.querySelector<HTMLDivElement>(
			"#confirm-password-error",
		);
		const passwordStrength =
			document.querySelector<HTMLDivElement>("#password-strength");
		const otpModal = document.querySelector<HTMLDivElement>("#otp-modal");
		const cancelBtn =
			document.querySelector<HTMLButtonElement>("#otp-cancel");

		const setShowOtpForm = (show: boolean) => {
			if (!otpModal) return;
			otpModal.classList.toggle("hidden", !show);
		};

		let challengeId: string | null = null;

		cancelBtn?.addEventListener("click", () => {
			setShowOtpForm(false);
			challengeId = null;

			emailInput && (emailInput.disabled = false);
			passwordInput && (passwordInput.disabled = false);
			confirmPasswordInput && (confirmPasswordInput.disabled = false);
			twoFactorInput && (twoFactorInput.disabled = false);
			if (submitButton) submitButton.disabled = false;

			const otpInput =
				document.querySelector<HTMLInputElement>("#otp-input");
			const otpError =
				document.querySelector<HTMLDivElement>("#otp-error");
			otpInput && (otpInput.value = "");
			otpError?.classList.add("hidden");

			setStatus("Signup cancelled", "idle");
		});

		let savedUsername = "AAAA";
		let savedEmail = "";
		let savedPassword = "";
		let isCheckingUsername = false;

		if (
			!form ||
			!status ||
			!submitButton ||
			!step1 ||
			!step2 ||
			!usernameHidden
		)
			return;

		const setStatus = (
			message: string,
			variant: "idle" | "loading" | "success" | "error" = "idle",
		) => {
			const baseClasses =
				"rounded-lg border px-3 py-2 text-sm font-medium shadow-[0_10px_35px_rgba(0,0,0,0.4)] transition";
			const variants: Record<typeof variant, string> = {
				idle: "hidden",
				loading:
					"block border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.12)] text-[var(--fg)]",
				success:
					"block border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.18)] text-[var(--fg)]",
				error: "block border-[rgba(255,114,114,0.4)] bg-[rgba(255,114,114,0.12)] text-[#ffd6d6]",
			};

			status.className = `${baseClasses} ${variants[variant]}`;
			status.textContent = message;
		};

		const showError = (element: HTMLDivElement | null, message: string) => {
			if (!element) return;
			element.textContent = message;
			element.classList.remove("hidden");
		};

		const alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789".split("");
		let activeSlot = 0;
		let slots = ["A", "A", "A", "A"];

		const renderSlots = () => {
			const slotEls =
				form.querySelectorAll<HTMLElement>(".username-slot");
			slotEls.forEach((el, idx) => {
				el.textContent = slots[idx];
				if (idx === activeSlot) {
					el.classList.add("active");
				} else {
					el.classList.remove("active");
				}
			});
			usernameHidden.value = slots.join("");
			savedUsername = usernameHidden.value;
		};

		const adjustSlot = (delta: number) => {
			const current = alphabet.indexOf(slots[activeSlot]);
			const next = (current + delta + alphabet.length) % alphabet.length;
			slots[activeSlot] = alphabet[next];
			renderSlots();
		};

		const setActiveSlot = (idx: number) => {
			activeSlot = Math.max(0, Math.min(slots.length - 1, idx));
			renderSlots();
		};

		form.querySelectorAll<HTMLButtonElement>(".username-up").forEach(
			(btn) =>
				btn.addEventListener("click", () => {
					const idx = Number(btn.dataset.index ?? "0");
					setActiveSlot(idx);
					adjustSlot(1);
				}),
		);

		form.querySelectorAll<HTMLButtonElement>(".username-down").forEach(
			(btn) =>
				btn.addEventListener("click", () => {
					const idx = Number(btn.dataset.index ?? "0");
					setActiveSlot(idx);
					adjustSlot(-1);
				}),
		);

		form.querySelectorAll<HTMLElement>(".username-slot").forEach((slot) =>
			slot.addEventListener("click", () => {
				const idx = Number(slot.dataset.index ?? "0");
				setActiveSlot(idx);
			}),
		);

		const handleSignupKeydown = (event: KeyboardEvent) => {
			if (!document.body.contains(step1)) return;
			if (step1.classList.contains("hidden")) return;
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

		document.addEventListener("keydown", handleSignupKeydown);

		renderSlots();

		const showStep = (step: 1 | 2) => {
			if (step === 1) {
				step1.classList.remove("hidden");
				step2.classList.add("hidden");
			} else {
				step1.classList.add("hidden");
				step2.classList.remove("hidden");
			}
		};

		const handleBackClick = (event: MouseEvent) => {
			event.preventDefault();
			event.stopPropagation();

			if (!step2.classList.contains("hidden")) {
				showStep(1);
				setFormStatus(status, "", "idle");
				return;
			}

			if (window.history.length > 1) {
				history.back();
			} else {
				history.pushState({}, "", "/login");
				window.dispatchEvent(new PopStateEvent("popstate"));
			}
		};

		backLink?.addEventListener("click", handleBackClick);

		emailInput?.addEventListener("blur", () => {
			if (!emailInput.value.trim()) return;
			validateEmailField(emailInput, emailError);
		});

		passwordInput?.addEventListener("input", () => {
			if (!passwordInput.value) {
				hideError(passwordError);
				setInputError(passwordInput, false);
				passwordStrength?.classList.add("hidden");
				return;
			}

			const passwordValidation = validatePassword(passwordInput.value);

			if (passwordStrength) {
				const strengthColor = getPasswordStrengthColor(
					passwordValidation.strength,
				);
				const strengthLabel = getPasswordStrengthLabel(
					passwordValidation.strength,
				);
				passwordStrength.className = `rounded px-2 py-1 text-xs font-semibold uppercase ${strengthColor}`;
				passwordStrength.textContent = `Strength: ${strengthLabel}`;
				passwordStrength.classList.remove("hidden");
			}

			if (
				!passwordValidation.isValid &&
				passwordValidation.errors.length > 0
			) {
				showError(passwordError, passwordValidation.errors[0]);
				passwordInput.classList.add("border-[rgba(255,114,114,0.4)]");
			} else {
				hideError(passwordError);
				passwordInput.classList.remove(
					"border-[rgba(255,114,114,0.4)]",
				);
			}
		});

		if (confirmPasswordInput?.value && passwordInput) {
			const confirmError = validatePasswordConfirmation(
				passwordInput.value,
				confirmPasswordInput.value,
			);
			if (confirmError) {
				showError(confirmPasswordError, confirmError);
				confirmPasswordInput.classList.add(
					"border-[rgba(255,114,114,0.4)]",
				);
			} else {
				hideError(confirmPasswordError);
				confirmPasswordInput.classList.remove(
					"border-[rgba(255,114,114,0.4)]",
				);
			}
		}

		confirmPasswordInput?.addEventListener("input", () => {
			if (!confirmPasswordInput.value || !passwordInput) return;

			const confirmError = validatePasswordConfirmation(
				passwordInput.value,
				confirmPasswordInput.value,
			);
			if (confirmError) {
				showError(confirmPasswordError, confirmError);
				confirmPasswordInput.classList.add(
					"border-[rgba(255,114,114,0.4)]",
				);
			} else {
				hideError(confirmPasswordError);
				confirmPasswordInput.classList.remove(
					"border-[rgba(255,114,114,0.4)]",
				);
			}
		});

		nextButton?.addEventListener("click", async (e) => {
			e.preventDefault();
			e.stopPropagation();
			if (isCheckingUsername) return;
			isCheckingUsername = true;

			try {
				if (!usernameHidden.value) {
					setFormStatus(status, "Please choose a username.", "error");
					return;
				}
				nextButton.disabled = true;
				setFormStatus(status, "Checking username...", "loading");

				try {
					const check = await userService.isUsernameFree(
						usernameHidden.value,
					);

					if (check.success && check.data == false) {
						setStatus(
							"Username is already taken. Choose another.",
							"error",
						);
						nextButton.disabled = false;
						return;
					}

					if (
						!check.success &&
						check.error &&
						check.error !== "User not found"
					) {
						setStatus(check.error, "error");
						nextButton.disabled = false;
						return;
					}

					savedUsername = usernameHidden.value;
					setFormStatus(status, "", "idle");
					showStep(2);
				} catch (error) {
					setFormStatus(
						status,
						"Network error. Please try again.",
						"error",
					);
					console.error("Username check failed:", error);
				}
			} finally {
				isCheckingUsername = false;
				nextButton.disabled = false;
			}
		});

		form.addEventListener("submit", async (event) => {
			event.preventDefault();

			const username = savedUsername || usernameHidden.value || "";
			const email = savedEmail || emailInput?.value.trim() || "";
			const password = savedPassword || passwordInput?.value.trim() || "";
			const confirmPassword = confirmPasswordInput?.value.trim() || "";
			const twoFactor = twoFactorInput?.checked ? 1 : 0;

			hideError(emailError);
			hideError(passwordError);
			hideError(confirmPasswordError);

			let hasErrors = false;

			const emailValidation = validateEmailField(emailInput, emailError);
			if (!emailValidation.isValid) {
				showError(emailError, emailValidation.error || "Invalid email");
				hasErrors = true;
			}

			const passwordValidation = validatePassword(password);
			if (!passwordValidation.isValid) {
				showError(
					passwordError,
					passwordValidation.errors[0] || "Invalid password",
				);
				hasErrors = true;
			}

			const confirmError = validatePasswordConfirmation(
				password,
				confirmPassword,
			);
			if (confirmError) {
				showError(confirmPasswordError, confirmError);
				hasErrors = true;
			}

			if (hasErrors) {
				setStatus("Please fix the errors above", "error");
				return;
			}

			if (!username) {
				setFormStatus(status, "Please fill in username.", "error");
				showStep(1);
				return;
			}

			setFormStatus(status, "Creating your account...", "loading");
			submitButton.disabled = true;

			try {
				const result = await userService.createUser({
					username,
					email: email,
					password: password,
					twoFactor: twoFactor,
				});
				if (result.success && (result as any).twoFactor) {
					challengeId = (result as any).challengeId;

					setStatus(
						(result as any).message ??
							"Check the verification code sent to your email",
						"success",
					);

					emailInput && (emailInput.disabled = true);
					passwordInput && (passwordInput.disabled = true);
					confirmPasswordInput &&
						(confirmPasswordInput.disabled = true);
					twoFactorInput && (twoFactorInput.disabled = true);
					submitButton.disabled = true;

					setShowOtpForm(true);
					return;
				}
				if (result.success && result.data) {
					sessionService.saveUser(result.data);
					presenceService.connect();
					setStatus("Account created! Redirecting...", "success");

					history.pushState({}, "", "/profile");
					window.dispatchEvent(new PopStateEvent("popstate"));
					return;
				} else {
					setStatus(
						result.error ??
							"Failed to create account. Please try again.",
						"error",
					);
				}

				submitButton.disabled = false;
			} catch (err) {
				console.error(err);
				setStatus(
					"Failed to create account. Please try again.",
					"error",
				);
			} finally {
				submitButton.disabled = false;
			}
		});

		const otpForm = document.querySelector<HTMLFormElement>("#otp-form");
		const otpInput = document.querySelector<HTMLInputElement>("#otp-input");
		const otpError = document.querySelector<HTMLDivElement>("#otp-error");

		otpForm?.addEventListener("submit", async (e) => {
			e.preventDefault();

			if (!otpInput) return;

			const code = otpInput.value.trim();

			if (!/^\d{6}$/.test(code)) {
				if (otpError) {
					otpError.textContent = "Please enter a 6-digit code";
					otpError.classList.remove("hidden");
				}
				return;
			}

			if (!challengeId) {
				if (otpError) {
					otpError.textContent =
						"Session expired. Please signup again.";
					otpError.classList.remove("hidden");
				}
				return;
			}

			otpError?.classList.add("hiddent");
			otpInput.disabled = true;

			try {
				const result = await verifyOTP(challengeId, code);

				if (result.success && result.data) {
					sessionService.saveUser(result.data);
					presenceService.connect();

					setShowOtpForm(false);
					setStatus("Verified! Redirecting...", "success");

					history.pushState({}, "", "/profile");
					window.dispatchEvent(new PopStateEvent("popstate"));
					return;
				}
				if (otpError) {
					otpError.textContent =
						result.error ?? "Invalid verification code";
					otpError.classList.remove("hidden");
				}
				otpInput.disabled = false;
			} catch (err) {
				console.error("OTP verification failed", err);
				if (otpError) {
					otpError.textContent = "Unable to verify code, try again.";
					otpError.classList.remove("hidden");
				}
				otpInput.disabled = false;
			}
		});

		return () => {
			document.removeEventListener("keydown", handleSignupKeydown);
			backLink?.removeEventListener("click", handleBackClick);
		};
	};

	return { html, onMount };
};

export default Signup;
