import { API_CONFIG } from "../services/api.config";
import type { PublicUser } from "../services/types";

export const MAX_AVATAR_BYTES = 5 * 1024 * 1024;

export type AvatarChoice = "photo" | "initials";

const AVATAR_CHOICE_PREFIX = "ft_avatar_choice_";
const ALLOWED_AVATAR_TYPES = new Set(["image/png", "image/jpeg", "image/jpg"]);
const DEFAULT_AVATAR_URL = "api/uploads/avatars/default.png";

export const resolveAvatarUrl = (avatarUrl?: string | null): string | null => {
	if (!avatarUrl) return null;
	if (/^https?:\/\//.test(avatarUrl)) return avatarUrl;

	const baseUrl = API_CONFIG.BASE_URL.replace(/\/$/, "");
	const normalizedPath = avatarUrl.startsWith("/")
		? avatarUrl
		: `/${avatarUrl}`;
	const shouldStripApi =
		normalizedPath.startsWith("/api/") && baseUrl.endsWith("/api");
	const base = shouldStripApi ? baseUrl.slice(0, -4) : baseUrl;

	return `${base}${normalizedPath}`;
};

export const getAvatarChoice = (userId: number): AvatarChoice | null => {
	if (typeof window === "undefined") return "initials";
	try {
		const raw = localStorage.getItem(`${AVATAR_CHOICE_PREFIX}${userId}`);
		if (raw === "photo" || raw === "initials") {
			return raw;
		}
		return "initials";
	} catch (error) {
		console.warn("Failed to read avatar choice", error);
		return null;
	}
};

export const setAvatarChoice = (userId: number, choice: AvatarChoice): void => {
	if (typeof window === "undefined") return;
	try {
		localStorage.setItem(`${AVATAR_CHOICE_PREFIX}${userId}`, choice);
	} catch (error) {
		console.warn("Failed to save avatar choice", error);
	}
};

export const resolveAvatarChoice = (user: PublicUser): AvatarChoice => {
	const stored = getAvatarChoice(user.id);
	return stored ?? "photo";
};

export const renderAvatarDisplay = (
	user: PublicUser,
	choice?: AvatarChoice,
): string => {
	const avatarUrl = resolveAvatarUrl(user.avatar_url);
	const initials = user.username.slice(0, 2).toUpperCase();
	const resolvedChoice = choice ?? resolveAvatarChoice(user);

	if (resolvedChoice === "initials" || !user.avatar_url) {
		return `<span class="text-lg font-bold text-[var(--fg)]">${initials}</span>`;
	}

	const src = avatarUrl ?? DEFAULT_AVATAR_URL;

	return `<img src="${src}" alt="${user.username} avatar" class="h-full w-full object-cover" />`;
};

export const validateAvatarFile = (file: File): string | null => {
	if (file.size > MAX_AVATAR_BYTES) {
		return "File is too large. Max size is 5MB.";
	}

	if (ALLOWED_AVATAR_TYPES.has(file.type)) {
		return null;
	}

	const ext = file.name.split(".").pop()?.toLowerCase();
	if (ext && ["png", "jpg", "jpeg"].includes(ext)) {
		return null;
	}

	return "Invalid file type. Use PNG or JPG.";
};
