import fp from "fastify-plugin";
import { FastifyPluginAsync } from "fastify";
import { authCheck } from "../auth/authCheck";

const authGuardPlugin: FastifyPluginAsync = async (fastify) => {
	fastify.addHook("preHandler", async (request, reply) => {
		if (request.routeOptions?.config?.public === true) {
			return;
		}
		await authCheck(request, reply);
	});
};

export const authGuard = fp(authGuardPlugin);
