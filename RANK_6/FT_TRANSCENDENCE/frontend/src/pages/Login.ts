import loginTitle from "../assets/images/page_titles/LOGIN_PAGE.png";
import arrowBack from "../assets/images/page_assets/ARROW_BACK.png";
import { isLoginChallenge, isPublicUser, loginUser } from "../services/api";
import { verifyOTP } from "../services/api";
import { sessionService } from "../services/session.service";
import { presenceService } from "../services/presence.service";
import { validateEmailField } from "../utils/formValidators";

const Login = () => {
	const html = `
    <section class="retro-card crt space-y-6 p-8">
      <a
        id="login-back"
        data-link
        href="/"
        class="fixed left-6 top-6 z-20 flex items-center gap-3 text-base font-semibold uppercase text-[var(--fg)]"
        aria-label="Back"
      >
        <img src="${arrowBack}" alt="" class="h-7 w-auto" />
        <span>Back</span>
      </a>
      <div id="login-header" class="space-y-2">
        <div class="flex justify-center">
          <img src="${loginTitle}" alt="Login" class="mx-auto h-auto max-w-full" />
        </div>
      </div>

      <form id="login-form" class="space-y-4">
        <label class="block space-y-1">
          <span class="text-sm font-semibold text-[var(--fg)]">Email</span>
          <input
            type="email"
            name="email"
            id="login-email"
            class="w-full rounded-lg border border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.03)] px-3 py-2 text-[var(--fg)] placeholder:text-[rgba(141,161,197,0.75)] focus:border-[var(--fg)] focus:outline-none focus:ring-2 focus:ring-[rgba(230,240,255,0.25)]"
            placeholder="you@example.com"
          />
          <div id="login-email-error" class="hidden text-xs text-[#ffd6d6]"></div>
        </label>
        <label class="block space-y-1">
          <span class="text-sm font-semibold text-[var(--fg)]">Password</span>
          <input
            type="password"
            name="password"
            id="login-password"
            class="w-full rounded-lg border border-[rgba(230,240,255,0.35)] bg-[rgba(255,255,255,0.03)] px-3 py-2 text-[var(--fg)] placeholder:text-[rgba(141,161,197,0.75)] focus:border-[var(--fg)] focus:outline-none focus:ring-2 focus:ring-[rgba(230,240,255,0.25)]"
            placeholder="••••••••"
          />
          <div id="login-password-error" class="hidden text-xs text-[#ffd6d6]"></div>
        </label>
        <button
          type="submit"
            class="retro-btn w-full justify-center"
          >
          Sign in
        </button>
      </form>
	  <div id="login-status" class="hidden rounded-lg border px-3 py-2 text-sm font-medium shadow-[0_10px_35px_rgba(0,0,0,0.4)]"></div>
	  <p id="signup-cta" class="text-sm text-[var(--muted)]">
	  No account yet?
	  <a data-link href="/signup" class="retro-link font-semibold">Create one</a>.
		  </p>
	  <div
		  id="otp-modal"
		  class="hidden fixed inset-0 z-50 flex items-center justify-center bg-black"
	  >
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

				  <div
					  id="otp-error"
					  class="hidden text-xs text-[#ffd6d6] text-center"
				  ></div>

				  <button type="submit" class="retro-btn w-full justify-center">
					  Verify
				  </button>
				  <button
					  type="button"
					  id="otp-cancel"
					  class="retro-btn w-full justify-center bg-[rgba(255,114,114,0.8)] hover:bg-[rgba(255,114,114,1)]"
				  >
					  Cancel
				  </button>
			  </form>
		  </div>
	  </div>
    </section>
  `;

	const onMount = () => {
		const form = document.querySelector<HTMLFormElement>("#login-form");
		const header = document.querySelector<HTMLDivElement>("#login-header");
		const signupCta =
			document.querySelector<HTMLParagraphElement>("#signup-cta");
		const status = document.querySelector<HTMLDivElement>("#login-status");
		const backLink =
			document.querySelector<HTMLAnchorElement>("#login-back");

		const emailInput =
			form?.querySelector<HTMLInputElement>("#login-email");
		const passwordInput =
			form?.querySelector<HTMLInputElement>("#login-password");
		if (!emailInput || !passwordInput) return;
		const emailError =
			document.querySelector<HTMLDivElement>("#login-email-error");
		const passwordError = document.querySelector<HTMLDivElement>(
			"#login-password-error",
		);

		if (backLink) {
			const backFallback = backLink.getAttribute("href") ?? "/";
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

		/* MODAL FOR OTP */
		const otpModal = document.querySelector<HTMLDivElement>("#otp-modal");

		const setShowOtpForm = (show: boolean) => {
			if (!otpModal) return;
			otpModal.classList.toggle("hidden", !show);
		};

		let challengeId: string | null = null;

		const cancelBtn =
			document.querySelector<HTMLButtonElement>("#otp-cancel");

		cancelBtn?.addEventListener("click", () => {
			setShowOtpForm(false);
			challengeId = null;
			emailInput.disabled = false;
			passwordInput.disabled = false;
			const otpInput =
				document.querySelector<HTMLInputElement>("#otp-input");
			const otpError =
				document.querySelector<HTMLDivElement>("#otp-error");
			otpInput && (otpInput.value = "");
			otpError?.classList.add("hidden");
			setStatus("Login cancelled", "idle");
		});
		/* END MODAL FOR OTP */

		const setStatus = (
			message: string,
			variant: "idle" | "loading" | "success" | "error" = "idle",
		) => {
			if (!status) return;
			const base =
				"rounded-lg border px-3 py-2 text-sm font-medium shadow-[0_10px_35px_rgba(0,0,0,0.4)]";
			const variants: Record<typeof variant, string> = {
				idle: "hidden",
				loading:
					"block border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.12)] text-[var(--fg)]",
				success:
					"block border-[rgba(230,240,255,0.35)] bg-[rgba(230,240,255,0.18)] text-[var(--fg)]",
				error: "block border-[rgba(255,114,114,0.4)] bg-[rgba(255,114,114,0.12)] text-[#ffd6d6]",
			};
			status.className = `${base} ${variants[variant]}`;
			status.textContent = message;
		};

		// Helper to show/hide error messages
		const showError = (element: HTMLDivElement | null, message: string) => {
			if (!element) return;
			element.textContent = message;
			element.classList.remove("hidden");
		};

		const hideError = (element: HTMLDivElement | null) => {
			if (!element) return;
			element.classList.add("hidden");
		};

		const user = sessionService.loadUser();
		if (user) {
			history.pushState({}, "", "/profile");
			window.dispatchEvent(new PopStateEvent("popstate"));
			return;
		}

		// Real-time email validation on blur
		emailInput?.addEventListener("blur", () => {
			if (!emailInput.value.trim()) return;
			validateEmailField(emailInput, emailError);
		});

		// Form submit handler
		if (form) {
			form.addEventListener("submit", async (event) => {
				event.preventDefault();

				const email = emailInput.value.trim();
				const password = passwordInput.value.trim();

				// Clear previous errors
				hideError(emailError);
				hideError(passwordError);
				emailInput.classList.remove("border-[rgba(255,114,114,0.4)]");
				passwordInput.classList.remove(
					"border-[rgba(255,114,114,0.4)]",
				);

				let hasErrors = false;

				// Validate email
				const emailValidation = validateEmailField(
					emailInput,
					emailError,
				);
				if (!emailValidation.isValid) {
					showError(
						emailError,
						emailValidation.error || "Invalid email",
					);
					emailInput.classList.add("border-[rgba(255,114,114,0.4)]");
					hasErrors = true;
				}

				// Validate password presence
				if (!password) {
					showError(passwordError, "Password is required");
					passwordInput.classList.add(
						"border-[rgba(255,114,114,0.4)]",
					);
					hasErrors = true;
				}

				if (hasErrors) {
					setStatus("Please fix the errors above", "error");
					return;
				}

				setStatus("Signing in…", "loading");
				emailInput.disabled = true;
				passwordInput.disabled = true;

				let unlockInputs = true;

				try {
					const result = await loginUser(email, password);

					if (result.success && result.data) {
						if (isLoginChallenge(result.data)) {
							challengeId = result.data.challengeId;
							setShowOtpForm(true);
							unlockInputs = false;
							setStatus(
								"Verification code sent to your email",
								"success",
							);
						} else if (isPublicUser(result.data)) {
							sessionService.saveUser(result.data);
							presenceService.connect();
							setStatus("Signed in successfully!", "success");
							setTimeout(() => {
								history.pushState({}, "", "/profile");
								window.dispatchEvent(
									new PopStateEvent("popstate"),
								);
							}, 200);
						}
					} else {
						setStatus(
							result.error ?? "Invalid email or password.",
							"error",
						);
					}
				} catch (error) {
					setStatus("Unable to sign in. Please try again.", "error");
				} finally {
					if (unlockInputs) {
						emailInput.disabled = false;
						passwordInput.disabled = false;
					}
				}
			});
		}

		/* OTP VERIFY */
		const otpForm = document.querySelector<HTMLFormElement>("#otp-form");
		const otpInput = document.querySelector<HTMLInputElement>("#otp-input");
		const otpError = document.querySelector<HTMLDivElement>("#otp-error");
		otpForm?.addEventListener("submit", async (event) => {
			event.preventDefault();

			if (!otpInput) return;

			const code = otpInput.value.trim();

			// Validation
			if (!code || code.length !== 6 || !/^\d{6}$/.test(code)) {
				if (otpError) {
					otpError.textContent = "Not a valid OTP";
					otpError.classList.remove("hidden");
				}
				return;
			}

			otpError?.classList.add("hidden");
			otpInput.disabled = true;

			try {
				if (!challengeId) {
					otpError!.textContent = "Session expired";
					otpError!.classList.remove("hidden");
					otpInput.disabled = false;
					return;
				}
				const resultVerify = await verifyOTP(challengeId, code);

				if (resultVerify.success && resultVerify.data) {
					sessionService.saveUser(resultVerify.data);
					presenceService.connect();

					const otpModal =
						document.querySelector<HTMLDivElement>("#otp-modal");
					otpModal?.classList.add("hidden");

					history.pushState({}, "", "/profile");
					window.dispatchEvent(new PopStateEvent("popstate"));
				} else {
					otpError?.classList.remove("hidden");
					otpError!.textContent =
						resultVerify.error || "Invalid verification code";
					otpInput.disabled = false;
				}
			} catch (error) {
				console.error("OTP verification failed", error);
				otpError?.classList.remove("hidden");
				otpError!.textContent = "Unable to verify code";
				otpInput.disabled = false;
			}
		});
	};

	return { html, onMount };
};

export default Login;
