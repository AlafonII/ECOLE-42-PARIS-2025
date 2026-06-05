import { FastifyInstance } from "fastify";
import { parseId } from "../utils/utils";
import { getQueries } from "../database";

export async function dashboardRoutes(fastify: FastifyInstance) {
	fastify.get("/dashboard/:userId", async (request, reply) => {
		try {
			const userId = parseId(
				(request.params as { userId: string }).userId
			);
			const dashboard = await getQueries().getFullUserDashboard(userId);
			return { success: true, data: dashboard };
		} catch (error) {
			console.error("User dashboard error:", error);
			return reply.code(500).send({
				success: false,
				error: "Failed to fetch user dashboard",
			});
		}
	});

	fastify.get("/dashboard/:userId/stats", async (request, reply) => {
		try {
			const userId = parseId(
				(request.params as { userId: string }).userId
			);
			const stats = await getQueries().getUserBasicStats(userId);
			return { success: true, data: stats };
		} catch (error) {
			console.error("Stats error:", error);
			return reply
				.code(500)
				.send({ success: false, error: "Failed to fetch basic stats" });
		}
	});

	fastify.get("/dashboard/:userId/vs/:opponentId", async (request, reply) => {
		try {
			const params = request.params as {
				userId: string;
				opponentId: string;
			};
			const userAId = parseId(params.userId);
			const userBId = parseId(params.opponentId);
			const stats = await getQueries().get1v1Stats(userAId, userBId);
			return { success: true, data: stats };
		} catch (error) {
			console.error("1v1 stats error: ", error);
			return reply
				.code(500)
				.send({ success: false, error: "Failed to fetch 1v1 stats" });
		}
	});
}
