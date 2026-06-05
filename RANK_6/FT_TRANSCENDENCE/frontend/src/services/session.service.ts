import { PublicUser } from "./types";

const USER_STORAGE_KEY = "ft_user";

//THIS SHOULD BE REWRITTEN ONCE WE HAVE THE HANDLING OF AUTHENTICATION TOKENS
export class SessionService {
	private notifySessionChange(): void {
		window.dispatchEvent(new Event("session:changed"));
	}

	saveUser(user: PublicUser, notify = true): void {
		try {
			localStorage.setItem(USER_STORAGE_KEY, JSON.stringify(user));
			if (notify) {
				this.notifySessionChange();
			}
		} catch (error) {
			console.warn("Failed to persist user session", error);
		}
	}

	loadUser(): PublicUser | null {
		try {
			const raw = localStorage.getItem(USER_STORAGE_KEY);
			return raw ? (JSON.parse(raw) as PublicUser) : null;
		} catch (error) {
			console.warn("Failed to load user session", error);
			return null;
		}
	}

	clearUser(): void {
		try {
			localStorage.removeItem(USER_STORAGE_KEY);
			this.notifySessionChange();
		} catch (error) {
			console.warn("Failed to clear user session", error);
		}
	}

	isLoggedIn(): boolean {
		return this.loadUser() !== null;
	}
}

export const sessionService = new SessionService();
