import { sessionService } from '../services/session.service';

/**
 * Handle authentication errors (401) by clearing session and redirecting to login
 * @param status - HTTP status code from response
 */
export function handleAuthError(status: number): void {
	if (status === 401) {
		sessionService.clearUser();
		
		// Redirect to login only if not already on login/signup pages
		if (!window.location.pathname.match(/^\/(login|signup)/)) {
			window.location.href = '/login';
		}
	}
}
