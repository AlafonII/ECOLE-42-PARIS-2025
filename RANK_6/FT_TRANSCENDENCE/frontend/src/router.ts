import Nav from "./components/Nav";
import { closePongConnection, initPongClient } from "./game/PongClient";
import { initPongGame } from "./game/PongGameLogic";
import { sessionService } from "./services/session.service";
import { presenceService } from "./services/presence.service";

// A route can return static HTML or HTML with an onMount hook for wiring events
type RouteView = string | { html: string; onMount?: () => void };
type RouteHandler = () => RouteView;

export default class Router {
	// "routes" will store all registered paths and their handler functions
	private routes: Record<string, RouteHandler>;

	// The CSS selector where the router will inject page content
	private rootSelector: string;

	private matchRoute(path: string): RouteHandler | undefined {
		if (this.routes[path]) return this.routes[path];

		const pathParts = path.split("/").filter(Boolean);

		for (const [pattern, handler] of Object.entries(this.routes)) {
			if (!pattern.includes(":")) continue;

			const patternParts = pattern.split("/").filter(Boolean);
			if (patternParts.length !== pathParts.length) continue;

			let ok = true;
			for (let i = 0; i < patternParts.length; i++) {
				const pp = patternParts[i];
				const pv = pathParts[i];

				if (pp.startsWith(":")) continue;

				if (pp !== pv) {
					ok = false;
					break;
				}
			}
			if (ok) return handler;
		}
		return undefined;
	}

	// Sets up route storage, render target, and navigation listeners
	constructor(rootSelector = "#app") {
		this.routes = {};
		this.rootSelector = rootSelector;

		// When the user uses back/forward buttons, re-render the correct page
		window.addEventListener("popstate", () => this.renderRoute());
		window.addEventListener("session:changed", () => this.renderRoute());

		// Intercept clicks on <a data-link> so we can use client-side navigation
		document.addEventListener("click", (event) =>
			this.handleLinkClick(event)
		);
	}

	// Register a new route: e.g. "/profile" → function that returns HTML
	register(path: string, handler: RouteHandler) {
		this.routes[path] = handler;
	}

	// Start the router by rendering the current route
	start() {
		this.renderRoute();
	}

	// change the URL and show the corresponding page
	navigate(path: string) {
		// update history if the user is actually navigating to a new page
		if (window.location.pathname !== path) {
			history.pushState({}, "", path); // Update the URL without reloading
		}
		this.renderRoute(); // Render the new page
	}

	// Detect clicks on <a data-link> and prevent the full page reload
	private handleLinkClick(event: MouseEvent) {
		const target = event.target as HTMLElement;

		// Find the closest anchor tag with the attribute data-link
		const anchor = target.closest(
			"a[data-link]"
		) as HTMLAnchorElement | null;
		if (!anchor) return; // Ignore clicks that are not SPA navigation links

		const href = anchor.getAttribute("href");
		if (!href) return;

		event.preventDefault();
		this.navigate(href); // Use our custom router instead
	}

	private renderRoute() {
		// Find the DOM element where we inject the page HTML
		const root = document.querySelector(this.rootSelector);
		if (!root) return;

		// Get the current path, ensuring it doesn’t end with "/" unless it's the root
		const rawPath = window.location.pathname || "/";
		const path =
			rawPath.endsWith("/") && rawPath !== "/"
				? rawPath.slice(0, -1) // Remove trailing slash
				: rawPath || "/";
		const publicRoutes = new Set(["/", "/login", "/signup", "/pong"]);
		const isLoggedIn = sessionService.isLoggedIn();
		const activePath = !isLoggedIn && !publicRoutes.has(path) ? "/" : path;

		if (activePath !== path) {
			history.replaceState({}, "", activePath);
		}

		// Auto-connect presence service for logged-in users
		if (isLoggedIn && !presenceService.connected()) {
			presenceService.connect();
		}

		// Find the handler for this path (fall back to "/" if missing)
		const render = this.matchRoute(activePath) || this.routes["/"];

		// Call the handler to get the HTML, or show a 404 message
		const renderedView: RouteView = render
			? render()
			: { html: `<main class="p-6">Page not found</main>` };

		const { html, onMount } =
			typeof renderedView === "string"
				? { html: renderedView }
				: renderedView;

		document.body.classList.toggle(
			"page-scroll",
			activePath === "/settings"
		);

		// Replace the existing page content with layout + nav + page content
		const nav = isLoggedIn ? Nav() : "";
		root.innerHTML = `<main class="page-shell">${nav}${html}</main>`;

		if (typeof onMount === "function") {
			onMount();
		}
	}
}
