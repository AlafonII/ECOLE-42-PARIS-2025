import { FastifyInstance } from "fastify";
import { authCheck } from "../auth/authCheck.js";
import { disconnectUser } from "./presence.js";

export async function authRoutes(app: FastifyInstance) {
	// Endpoint to get WebSocket token
	app.get("/auth/ws-token", async (request, reply) => {
		const token = request.cookies?.access_token;

		if (!token) {
			//console.log("No token found");
			return reply.code(401).send({
				success: false,
				error: "No authentication"
			});
		}

		try {
			const payload = await request.server.jwt.verify(token);

			return reply.send({ 
				success: true, 
				token 
			});
		} catch (error) {
			//console.log("Token verification failed:", error);
			return reply.code(401).send({
				success: false,
				error: "Invalid token"
			});
		}
	});

	// Logout endpoint
	app.post("/auth/logout", async (request, reply) => {
		try {
			// Get user ID from token before clearing cookie
			const token = request.cookies?.access_token;
			let userId: number | null = null;
			
			if (token) {
				try {
					const payload: any = await request.server.jwt.verify(token);
					userId = payload.id;
				} catch (err) {
					//console.log("Token already invalid");
				}
			}

			// Close all WebSocket connections for this user
			if (userId) {
				await disconnectUser(userId);
			}

			reply.clearCookie("access_token", {
				path: "/",
				httpOnly: true,
				secure: process.env.NODE_ENV === "production",
				sameSite: "lax"
			});

			return reply.send({
				success: true,
				message: "Logged out successfully"
			});
		} catch (error) {
			console.error("Logout error:", error);
			return reply.code(500).send({
				success: false,
				error: "Failed to logout"
			});
		}
	});

	app.get(
		"/auth/me",
		{ config: { public: true } },
		async (request, reply) => {
			const user = request.user as {
				id: number;
				username: string;
				email: string;
			};

			return reply.send({
				success: true,
				data: user,
			});
		}
	);
}
