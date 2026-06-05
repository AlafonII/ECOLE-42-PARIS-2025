const links = [
	{ href: "/pong", label: "Pong Game" },
	{ href: "/friends", label: "Friends" },
	{ href: "/chat", label: "Live Chat" },
	{ href: "/profile", label: "Profile" },
	{ href: "/tournaments", label: "Tournaments" },
];

// Shared navbar for quick SPA navigation between placeholder pages.
const Nav = (): string => `
  <header class="fixed bottom-0 left-0 right-0 z-10 flex w-full items-center border border-[rgba(230,240,255,0.18)] bg-[var(--page-bg)] px-5 py-4">
    <nav class="flex w-full items-center text-sm font-medium text-[var(--muted)]">
      ${links
			.map(
				(link) =>
					`<a data-link href="${link.href}" class="retro-link nav-hover flex-1 rounded-lg px-3 py-1 text-center">${link.label}</a>`,
			)
			.join("")}
    </nav>
  </header>
`;

export default Nav;
