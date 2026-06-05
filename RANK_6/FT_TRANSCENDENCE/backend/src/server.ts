import fastify from "fastify";
import cors from "@fastify/cors";
import jwt from "@fastify/jwt";
import dotenv from "dotenv";
import path from "path";
import fastifyCookie from "@fastify/cookie";
import websocket from "@fastify/websocket";
import multipart from "@fastify/multipart";
import { authRoutes } from "./routes/auth.js";
import { userRoutes } from "./routes/users.ts";
import { initialize } from "./database/index.ts";
import { matchRoutes } from "./routes/matches.ts";
import { tournamentRoutes } from "./routes/tournaments.ts";
import { liveChatRoutes } from "./routes/liveChat.ts";
import { dashboardRoutes } from "./routes/dashboardRoutes.ts";
import { pongRoutes } from "./routes/pong.ts";
import { presenceRoutes } from "./routes/presence.ts";
import { friendsRoutes } from "./routes/friends.ts";
import { getDb } from "./database/index.ts";
import fastifyStatic from "@fastify/static";
import { ChatGateway, setChatGateway } from "./chat/ChatGateway";
import { authGuard } from "./utils/authGuard";

dotenv.config();

if (!process.env.JWT_SECRET) {
	console.error("FATAL: JWT_SECRET env is not set");
	process.exit(1);
}

if (!process.env.COOKIE_SECRET) {
	console.error("FATAL: COOKIE_SECRET env is not set");
	process.exit(1);
}

const server = fastify({ logger: true });

server.register(cors, {
	origin: ["https://localhost:5173"],
	credentials: true,
	methods: ["GET", "POST", "PUT", "DELETE", "OPTIONS"], // do we need PUT and OPTIONS?
});

await server.register(fastifyCookie, {
	secret: process.env.COOKIE_SECRET,
});

await server.register(jwt, {
	secret: process.env.JWT_SECRET,
});

await server.register(websocket);

const chatGateway = new ChatGateway(server);
chatGateway.register();
setChatGateway(chatGateway);

server.register(multipart, { limits: { fileSize: 5 * 1024 * 1024 } });

const uploadRoot = path.join(process.cwd(), "uploads");
server.register(fastifyStatic, {
	root: uploadRoot,
	prefix: "/uploads/",
});
server.register(fastifyStatic, {
	root: uploadRoot,
	prefix: "/api/uploads/",
	decorateReply: false,
});

async function start() {
	try {
		await initialize();
		const db = getDb();
		server.decorate("db", db);
		//console.log("Database initialized successfully");

		await server.register(async (api) => {
			await api.register(authGuard);

			api.register(authRoutes);
			api.register(userRoutes);
			api.register(matchRoutes);
			api.register(tournamentRoutes);
			api.register(liveChatRoutes);
			api.register(dashboardRoutes);
			api.register(friendsRoutes);
		}, { prefix: "/api" });

		server.register(pongRoutes);
		server.register(presenceRoutes);
		await server.listen({ port: 3000, host: "0.0.0.0" });
		//console.log("Server running");
	} catch (err) {
		console.error("Failed to start server: ", err);
		process.exit(1);
	}
}

start();
