import bcrypt from "bcrypt";
import crypto from "crypto";
import { FastifyInstance } from "fastify";
import { CreateUserDTO } from "../database/types.ts";
import { UsersOTP } from "../database/types.ts";
import { getQueries } from "../database/index.ts";
import { getJWTToken, parseId } from "../utils/utils";
import { emailOTP } from "../utils/nodemailer";
import { pipeline } from "node:stream/promises";
import * as fs from "node:fs";
import * as path from "node:path";
import { authCheck } from "../auth/authCheck";

type CreateUserRequestBody = Partial<CreateUserDTO> & { password: string };
type LoginRequestBody = { email: string; password: string };
const USERNAME_MAX_LENGTH = 15;
const MAX_AGE = 60 * 60 * 12;
const USERNAME_REGEX = /^[A-Z0-9]{4}$/;
const PASSWORD_REGEX = /^(?=.*[A-Z])(?=.*[a-z])(?=.*\d).{8,}$/;
const EMAIL_REGEX = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

const AVATAR_UPLOAD_DIR = path.join(process.cwd(), "uploads", "avatars");

const removeAvatarFile = async (avatarUrl?: string | null): Promise<void> => {
	if (!avatarUrl) return;
	if (!avatarUrl.startsWith("/uploads/avatars/")) return;

	const relativePath = avatarUrl.replace(/^\/+/, "");
	const absolutePath = path.normalize(path.join(process.cwd(), relativePath));

	if (!absolutePath.startsWith(`${AVATAR_UPLOAD_DIR}${path.sep}`)) return;

	try {
		await fs.promises.unlink(absolutePath);
	} catch (error) {
		if ((error as NodeJS.ErrnoException).code !== "ENOENT") {
			console.warn("Failed to remove old avatar:", error);
		}
	}
};

export async function userRoutes(fastify: FastifyInstance) {
	const queries = getQueries();

	fastify.get("/users", async (request, reply) => {
		try {
			const users = await queries.getAllUsers();
			return { success: true, data: users };
		} catch (error) {
			console.error("Error: ", error);
			return reply.code(500).send({
				success: false,
				error: "Failed to fetch users",
			});
		}
	});

	fastify.get<{ Params: { id: string } }>(
		"/users/by-id/:id",
		async (request, reply) => {
			try {
				const id = parseInt(request.params.id);

				if (isNaN(id)) {
					return reply.code(400).send({
						success: false,
						error: "Invalid user ID",
					});
				}

				const user = await queries.getUserById(id);

				if (!user) {
					return reply.code(404).send({
						success: false,
						error: "User not found",
					});
				}

				return { success: true, data: user };
			} catch (error) {
				console.error("Error:", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to fetch user",
				});
			}
		},
	);

	fastify.get<{ Params: { email: string } }>(
		"/users/by-email/:email",
		async (request, reply) => {
			try {
				const user = await queries.getUserByEmail(request.params.email);

				if (!user) {
					return reply.code(404).send({
						success: false,
						error: "User not found",
					});
				}

				return { success: true, data: user };
			} catch (error) {
				console.error("Error:", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to fetch user",
				});
			}
		},
	);

	fastify.get<{ Params: { username: string } }>(
		"/users/by-username/:username",
		async (request, reply) => {
			try {
				const user = await queries.getUserByUsername(
					request.params.username,
				);

				if (!user) {
					return reply.code(404).send({
						success: false,
						error: "User not found",
					});
				}

				return { success: true, data: user };
			} catch (error) {
				console.error("Error:", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to fetch user",
				});
			}
		},
	);

	fastify.get<{ Params: { username: string } }>(
		"/users/is-username-free/:username",
		{ config: { public: true } },
		async (request, reply) => {
			try {
				const result = await queries.isUsernameFree(
					request.params.username,
				);

				return { success: true, data: result };
			} catch (error) {
				console.error("Error:", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to verify username availability",
				});
			}
		},
	);

	fastify.addHook("preHandler", async (request, reply) => {
		const { method, routeOptions } = request;
		const body = request.body as CreateUserRequestBody | undefined;
		const routeUrl = routeOptions?.url || "";

		// Handle prefixed routes (e.g., /api/users) the same as /users
		const isUserPasswordMutation =
			method === "POST"
				? routeUrl.endsWith("/users")
				: method === "PUT" && routeUrl.includes("/users/");

		if (!isUserPasswordMutation || !body) return;

		const rawPassword = body.password ?? body.password_hash;
		if (typeof rawPassword !== "string" || rawPassword.length === 0) return;

		if (!PASSWORD_REGEX.test(rawPassword)) {
			return reply.code(400).send({
				success: false,
				error: "Invalid password format",
			});
		}

		body.password_hash = await bcrypt.hash(rawPassword, 10);
		const { password, ...rest } = body;
		Object.assign(body, rest);
	});

	fastify.put<{ Params: { id: string }; Body: Partial<CreateUserDTO> }>(
		"/users/:id",
		{ preHandler: authCheck },
		async (request, reply) => {
			try {
				const id = parseInt(request.params.id);
				if (isNaN(id)) {
					return reply.code(400).send({
						success: false,
						error: "Invalid user id",
					});
				}

				if (request.user.id !== id) {
					return reply.code(403).send({
						success: false,
						error: "You can only modify your own account",
					});
				}

				const user = await queries.getUserById(id);
				if (!user) {
					return reply.code(404).send({
						success: false,
						error: "User not found",
					});
				}

				const updates: Partial<CreateUserDTO> = {};

				if (request.body.username !== undefined) {
					const trimmed = request.body.username?.trim() ?? "";
					if (!trimmed) {
						return reply.code(400).send({
							success: false,
							error: "Username cannot be empty",
						});
					}
					if (!USERNAME_REGEX.test(trimmed)) {
						return reply.code(400).send({
							success: false,
							error: "Invalid username",
						});
					}
					updates.username = trimmed;
				}

				if (request.body.email !== undefined) {
					const trimmed = request.body.email?.trim() ?? "";
					if (!trimmed) {
						return reply.code(400).send({
							success: false,
							error: "Email cannot be empty",
						});
					}
					if (!EMAIL_REGEX.test(trimmed)) {
						return reply.code(400).send({
							success: false,
							error: "Invalid email format",
						});
					}
					updates.email = trimmed;
				}

				if (request.body.password_hash !== undefined) {
					updates.password_hash = request.body.password_hash;
				}

				if (request.body.avatar_url !== undefined) {
					updates.avatar_url = request.body.avatar_url;
				}

				const updated = await queries.updateUser(id, updates);
				if (!updated) {
					return reply.code(400).send({
						success: false,
						error: "Nothing to update",
					});
				}

				const newUser = await queries.getUserById(id);
				if (
					request.body.avatar_url === null &&
					user.avatar_url &&
					user.avatar_url !== newUser?.avatar_url
				) {
					await removeAvatarFile(user.avatar_url);
				}

				return reply.send({ success: true, data: newUser });
			} catch (error) {
				console.error(error);
				return reply.code(500).send({
					success: false,
					error: "Failed to update user",
				});
			}
		},
	);

	fastify.delete<{ Params: { id: string } }>(
		"/users/:id",
		{ preHandler: authCheck },
		async (request, reply) => {
			try {
				const id = parseInt(request.params.id);
				if (isNaN(id)) {
					return reply.code(400).send({
						success: false,
						error: "Invalid user id",
					});
				}

				if (request.user.id !== id) {
					return reply.code(403).send({
						success: false,
						error: "You can only delete your own account",
					});
				}

				const user = await queries.getUserById(id);

				if (!user) {
					return reply.code(400).send({
						success: false,
						error: "User not found",
					});
				}

				await queries.deleteUser(id);

				return reply.send({
					success: true,
					message: "User deleted",
				});
			} catch (error) {
				console.error("Error:", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to delete user",
				});
			}
		},
	);

	fastify.post<{ Body: CreateUserRequestBody }>(
		"/users",
		{ config: { public: true } },
		async (request, reply) => {
			try {
				const {
					username,
					email,
					password_hash,
					avatar_url,
					two_factor,
				} = request.body;

				const trimmedUsername = username?.trim();
				const trimmedEmail = email?.trim();

				if (!trimmedUsername || !trimmedEmail || !password_hash) {
					return reply.code(400).send({
						success: false,
						error: "Missing required fields",
					});
				}

				if (!USERNAME_REGEX.test(trimmedUsername)) {
					return reply.code(400).send({
						success: false,
						error: "Invalid username format",
					});
				}

				if (!EMAIL_REGEX.test(trimmedEmail)) {
					return reply.code(400).send({
						success: false,
						error: "Invalid email format",
					});
				}

				const existingEmail =
					await queries.getUserByEmail(trimmedEmail);
				const existingUsername =
					await queries.getUserByUsername(trimmedUsername);
				if (existingEmail || existingUsername) {
					return reply.code(409).send({
						success: false,
						error: existingEmail
							? "Email already exists"
							: "Username already exists",
					});
				}

				const userPayload: CreateUserDTO = {
					username: trimmedUsername,
					email: trimmedEmail,
					password_hash,
					avatar_url: avatar_url ?? null,
					two_factor: two_factor ?? 0,
				};

				await queries.createUser(userPayload);
				const newUser = await queries.getUserWithPasswordByEmail(
					userPayload.email,
				);

				if (!newUser) {
					return reply.code(500).send({
						success: false,
						error: "User was created but couldn't be loaded",
					});
				}

				if (newUser.two_factor) {
					const otp = crypto.randomInt(100000, 999999).toString();
					const otpHash = await bcrypt.hash(otp, 10);
					const expireAt = new Date(Date.now() + 5 * 60 * 1000);
					const challengeId = crypto.randomUUID();

					await queries.saveOTP({
						user_id: newUser.id,
						otp_hash: otpHash,
						expire_at: expireAt,
						challenge_id: challengeId,
					});

					await emailOTP(newUser.email, otp);

					return reply.send({
						success: true,
						twoFactor: true,
						challengeId,
						message: "Verification code sent by email",
					});
				}

				const token = getJWTToken(fastify, newUser);

				reply
					.clearCookie("access_token", { path: "/" })
					.setCookie("access_token", token, {
						httpOnly: true,
						secure: false,
						sameSite: "lax",
						path: "/",
						maxAge: MAX_AGE,
					})
					.send({
						success: true,
						data: newUser,
						message: "Registration successful",
					});
			} catch (error) {
				console.error("Error:", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to create user",
				});
			}
		},
	);

	fastify.post<{ Body: { email: string; password: string } }>(
		"/login",
		{ config: { public: true } },
		async (request, reply) => {
			try {
				const { email, password } = request.body;
				if (!email || !password) {
					return reply.code(400).send({
						success: false,
						error: "Email and password are required",
					});
				}

				const user = await queries.getUserWithPasswordByEmail(
					email.trim(),
				);

				if (!user || !user.password_hash) {
					return reply.code(401).send({
						success: false,
						error: "Invalid email or password",
					});
				}

				const isValid = await bcrypt.compare(
					password,
					user.password_hash,
				);

				if (!isValid) {
					return reply.code(401).send({
						success: false,
						error: "Invalid email or password",
					});
				}

				/* --------------------- TWO FACTOR ---------------------------*/
				if (user.two_factor) {
					const otp = crypto.randomInt(100000, 999999).toString();
					const otpHash = await bcrypt.hash(otp, 10);
					const expireAt = new Date(Date.now() + 5 * 60 * 1000);
					const challengeId = crypto.randomUUID();

					await queries.saveOTP({
						user_id: user.id,
						otp_hash: otpHash,
						expire_at: expireAt,
						challenge_id: challengeId,
					});

					await emailOTP(user.email, otp);

					return reply.send({
						success: true,
						data: {
							twoFactor: true,
							message: "Verification code send by email",
							challengeId,
						},
					});
				}

				/* ---------------------- JWT  ------------------------------*/
				const token = getJWTToken(fastify, user);

				// Remove password_hash from returned user
				const { password_hash, ...safeUser } = user;

				reply
					.clearCookie("access_token", { path: "/" })
					.setCookie("access_token", token, {
						httpOnly: true,
						secure: false,
						sameSite: "lax",
						path: "/",
						maxAge: MAX_AGE,
					})
					.send({
						success: true,
						data: safeUser,
						message: "Login successful",
					});
			} catch (error) {
				console.error("Login error", error);
				return reply.code(500).send({
					success: false,
					error: "Failed to login",
				});
			}
		},
	);

	fastify.post<{ Body: { challengeId: string; otp: string } }>(
		"/verify-otp",
		{ config: { public: true } },
		async (request, reply) => {
			const { challengeId, otp } = request.body;

			const userId = await queries.getUserIdByChallengeId(challengeId);
			if (!userId) {
				return reply.code(400).send({
					success: false,
					error: "verification code failed",
				});
			}

			const savedOTP = await queries.getOTPByUserId(userId);
			if (!savedOTP) {
				return reply.code(400).send({
					success: false,
					error: "verification code failed",
				});
			}

			if (savedOTP.attempts >= 3) {
				await queries.deleteOTP(savedOTP.id);
				return reply.code(429).send({
					success: false,
					error: "Two many attempts",
				});
			}

			if (Date.now() > new Date(savedOTP.expire_at).getTime()) {
				await queries.deleteOTP(savedOTP.id);
				return reply.code(400).send({
					success: false,
					error: "verification code failed",
				});
			}

			const compare = await bcrypt.compare(otp, savedOTP.otp_hash);
			if (!compare) {
				await queries.increaseOTPAttempt(savedOTP.id);
				return reply.code(400).send({
					success: false,
					error: "verification code failed",
				});
			}

			await queries.deleteOTP(savedOTP.id);

			const user = await queries.getUserById(userId);
			if (!user) {
				return reply.code(401).send({ success: false });
			}

			const token = getJWTToken(fastify, user);

			reply
				.clearCookie("access_token", { path: "/" })
				.setCookie("access_token", token, {
					httpOnly: true,
					secure: false,
					sameSite: "lax",
					path: "/",
					maxAge: MAX_AGE,
				})
				.send({
					success: true,
					message: "Login successful",
					data: user,
				});
		},
	);

	fastify.post("/users/:id/avatar", async (request, reply) => {
		try {
			const userId = parseId(
				(request.params as { id: string }).id,
				"user ID",
			);

			if (request.user.id !== userId) {
				return reply.code(403).send({
					success: false,
					error: "You can only update your own avatar",
				});
			}

			const existingUser = await queries.getUserById(userId);
			const parts = request.parts();

			let fileSavedUrl: string | null = null;

			for await (const part of parts) {
				if (part.type === "file" && part.fieldname === "avatar") {
					const mime = part.mimetype;
					const allowed = ["image/png", "image/jpeg", "image/webp"];
					if (!allowed.includes(mime)) {
						return reply.code(400).send({
							success: false,
							error: "Invalid file type. PNG, JPG or WEBP allowed",
						});
					}
					const ext =
						mime === "image/png"
							? "png"
							: mime === "image/webp"
								? "webp"
								: "jpg";

					fs.mkdirSync(AVATAR_UPLOAD_DIR, { recursive: true });

					const filename = `user-${userId}-${Date.now()}.${ext}`;
					const filepath = path.join(AVATAR_UPLOAD_DIR, filename);

					await pipeline(part.file, fs.createWriteStream(filepath));
					fileSavedUrl = `/uploads/avatars/${filename}`;
				}
			}

			if (!fileSavedUrl) {
				return reply.code(400).send({
					success: false,
					error: "No avatar file found in form-data",
				});
			}

			await queries.updateUser(userId, { avatar_url: fileSavedUrl });
			const updatedUser = await queries.getUserById(userId);
			if (
				existingUser?.avatar_url &&
				existingUser.avatar_url !== fileSavedUrl
			) {
				await removeAvatarFile(existingUser.avatar_url);
			}
			return reply.code(200).send({ success: true, data: updatedUser });
		} catch (err) {
			console.error("Avatar upload error:", err);
			return reply
				.code(500)
				.send({ success: false, error: "Failed to upload avatar" });
		}
	});
}
