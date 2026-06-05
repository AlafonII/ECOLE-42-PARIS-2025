import { FastifyInstance } from "fastify";
import { getQueries } from "../database";
import { parseId } from "../utils/utils";
import { authCheck } from "../auth/authCheck";
import { notifyNewFriendship } from "./presence";

interface FriendsLists {
    id: number;
    username: string;
    last_seen_at: string;
}

export async function friendsRoutes(fastify: FastifyInstance) {
    const queries = getQueries();
    fastify.addHook("preHandler", authCheck);

    // Get friends list
    fastify.get<{}>(
		"/friendsList",
		async (request, reply) => {
            try {
                const userId = request.user.id;
                if (!userId) { return reply.status(400).send({success: false, error: 'User is not authenticated' });}

                const friends = await queries.getFriends(userId);
                if (friends == null) {
                    return reply.status(201).send({success: true, message: 'No friends yet' });
                }

                const friendsWithStatus: FriendsLists[] = [];

                for (const friend of friends) {
                    friendsWithStatus.push({
                        id: friend.id,
                        username: friend.username,
                        last_seen_at: friend.last_seen_at,
                    });
                }
                return { success: true, data: { friendsWithStatus } };
            } catch (error) {
                reply.status(500).send({success: false, error: 'Failed to fetch friends' });
            }

    });

    // Get relations list
    fastify.get<{}>(
		"/relationsList",
		async (request, reply) => {
            try {
                const userId = request.user.id;
                if (!userId) { return reply.status(400).send({success: false, error: 'User is not authenticated' });}

                const relations = await queries.getRelations(userId);
                if (relations == null) {
                    return reply.status(201).send({success: true, message: 'No relation yet' });
                }

                const relationsWithStatus: FriendsLists[] = [];

                for (const relation of relations) {
                    relationsWithStatus.push({
                        id: relation.id,
                        username: relation.username,
                        last_seen_at: relation.last_seen_at,
                    });
                }
                return { success: true, data: { relationsWithStatus } };
            } catch (error) {
                reply.status(500).send({success: false, error: 'Failed to fetch relations' });
            }

    });

    // Get received friend requests
    fastify.get("/friend-requests/received", async (request, reply) => {
        try {
            const userId = request.user.id;
            const requests = await queries.getReceivedFriendRequests(userId);
            return { success: true, data: { requests } };
        } catch (error) {
            console.error("Failed to fetch received friend requests:", error);
            return reply.status(500).send({ success: false, error: 'Failed to fetch friend requests' });
        }
    });

    // Get sent friend requests
    fastify.get("/friend-requests/sent", async (request, reply) => {
        try {
            const userId = request.user.id;
            const requests = await queries.getSentFriendRequests(userId);
            return { success: true, data: { requests } };
        } catch (error) {
            console.error("Failed to fetch sent friend requests:", error);
            return reply.status(500).send({ success: false, error: 'Failed to fetch friend requests' });
        }
    });
    
    // Send friend request
    fastify.post<{ Body: { friendId: number } }>(
		"/friend-requests",
		async (request, reply) => {
            try {
                const userId = request.user.id;
                const { friendId } = request.body;

                if (!friendId) {
                    return reply.status(400).send({success: false, error: 'Valid friend identifier is required' });
                }

                const friendExists = await queries.getUserById(friendId);
                if (!friendExists) {
                    return reply.status(404).send({ success: false, error: 'User is not found' });
                }

                if (userId === friendId) {
                    return reply.status(400).send({success: false, error: 'Cannot add yourself as a friend' });
                }

                await queries.sendFriendRequest(userId, friendId);
                return reply.status(201).send({success: true, message: 'Friend request sent' });

            } catch (error: any) {
                console.error("Friend request wasn't created. Error:", error);
                
                if (error.message?.includes('blocked')) {
                    return reply.status(400).send({success: false, error: error.message });
                }
                if (error.message?.includes('Already friends')) {
                    return reply.status(400).send({success: false, error: 'Already a friend' });
                }
                if (error.message?.includes('already sent')) {
                    return reply.status(400).send({success: false, error: 'Friend request already sent' });
                }
                if (error.message?.includes('already sent you')) {
                    return reply.status(400).send({
                        success: false, 
                        error: 'This user already sent you a friend request. Please accept it instead.'
                    });
                }
                
                reply.status(500).send({success: false, error: 'Failed to send friend request' });
            }
    });

    // Accept friend request
    fastify.post<{ Body: { senderId: number } }>(
        "/friend-requests/accept",
        async (request, reply) => {
            try {
                const userId = request.user.id;
                const { senderId } = request.body;

                if (!senderId) {
                    return reply.status(400).send({ success: false, error: 'Sender ID is required' });
                }

                const senderExists = await queries.getUserById(senderId);
                if (!senderExists) {
                    return reply.status(404).send({ success: false, error: 'Sender not found' });
                }

                const success = await queries.acceptFriendRequest(senderId, userId);
                
                if (!success) {
                    return reply.status(404).send({ success: false, error: 'Friend request not found or already processed' });
                }

                // Small delay to allow frontend to update the friends list before WebSocket notification
                setTimeout(async () => {
                    await notifyNewFriendship(userId, senderId);
                }, 100);

                return reply.status(200).send({ success: true, message: 'Friend request accepted' });
            } catch (error) {
                console.error("Failed to accept friend request:", error);
                return reply.status(500).send({ success: false, error: 'Failed to accept friend request' });
            }
        }
    );

    // Reject friend request
    fastify.post<{ Body: { senderId: number } }>(
        "/friend-requests/reject",
        async (request, reply) => {
            try {
                const userId = request.user.id;
                const { senderId } = request.body;

                if (!senderId) {
                    return reply.status(400).send({ success: false, error: 'Sender ID is required' });
                }

                const success = await queries.rejectFriendRequest(senderId, userId);
                
                if (!success) {
                    return reply.status(404).send({ success: false, error: 'Friend request not found or already processed' });
                }

                return reply.status(200).send({ success: true, message: 'Friend request rejected' });
            } catch (error) {
                console.error("Failed to reject friend request:", error);
                return reply.status(500).send({ success: false, error: 'Failed to reject friend request' });
            }
        }
    );

    // Cancel sent friend request
    fastify.delete<{ Params: { friendId: string } }>(
        "/friend-requests/:friendId",
        async (request, reply) => {
            try {
                const userId = request.user.id;
                const friendId = parseId(request.params.friendId, "friend ID");

                const success = await queries.cancelFriendRequest(userId, friendId);
                
                if (!success) {
                    return reply.status(404).send({ success: false, error: 'Friend request not found' });
                }

                return reply.status(200).send({ success: true, message: 'Friend request cancelled' });
            } catch (error: any) {
                if (error.message?.includes('Invalid')) {
                    return reply.status(400).send({ success: false, error: error.message });
                }
                console.error("Failed to cancel friend request:", error);
                return reply.status(500).send({ success: false, error: 'Failed to cancel friend request' });
            }
        }
    );
    
    // Remove a friend
    fastify.delete<{ Params: { friendId: string } }>(
        "/friends/:friendId",
        async (request, reply) => {
            try {
                const userId = request.user.id;
                const friendId = parseId(request.params.friendId, "friend ID");
                
                const friendExists = await queries.getUserById(friendId);
                if (!friendExists) {
                    return reply.status(404).send({ success: false, error: 'Friend not found' });
                }

                if (userId === friendId) {
                    return reply.status(400).send({success: false, error: 'Cannot remove yourself' });
                }
                
                const existingFriendship = await queries.areFriends(userId, friendId);
                if (!existingFriendship) {
                    return reply.status(400).send({success: false, error: 'Cannot remove, users are not friends' });
                }
                
                await queries.removeFriend(userId, friendId);
                return reply.status(200).send({success: true, message: 'Friend removed' });
             } catch (error: any) {
                if (error.message?.includes('Invalid')) {
                    return reply.status(400).send({ success: false, error: error.message });
                }
                console.error("Friendship wasn't removed. Error:", error);
                reply.status(500).send({success: false, error: 'Failed to remove friend' });
            }
    });
}
