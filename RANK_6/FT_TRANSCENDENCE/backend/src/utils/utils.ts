import { DBUser, PublicUser } from "../database/types.ts";

export function parseId(idString: string, fieldName: string = "ID"): number {
	const id = parseInt(idString);
	if (isNaN(id)) {
		throw new Error(`Invalid ${fieldName}`);
	}
	return id;
}

export function getJWTToken(
	fastify: FastifyInstance,
	user: DBUser | PublicUser
): string {
	const token = fastify.jwt.sign(
		{
			id: user.id,
			username: user.username,
			email: user.email,
		},
		{
			expiresIn: "12h",
		}
	);
	return token;
}
