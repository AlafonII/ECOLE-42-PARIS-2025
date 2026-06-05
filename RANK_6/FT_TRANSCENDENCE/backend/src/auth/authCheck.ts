import { FastifyReply, FastifyRequest } from "fastify";

interface JwtPayload {
	id: number;
	username: string;
	email: string;
}

export async function authCheck(
	request: FastifyRequest,
	reply: FastifyReply
) {
	const token = request.cookies?.access_token;

	if (!token) {
		return reply.code(401).send({
			success: false,
			error: "No authentication",
		});
	}

	try {
		const payload = await request.server.jwt.verify(token) as JwtPayload;

		request.user = payload;
	} catch (error) {
		return reply.code(401).send({
			success: false,
			error: "No valid token",
		});
	}
}
