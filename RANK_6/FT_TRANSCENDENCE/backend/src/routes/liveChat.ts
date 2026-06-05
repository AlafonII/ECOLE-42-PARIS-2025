import { FastifyInstance } from "fastify";
import { getQueries } from "../database";
import { parseId } from "../utils/utils";
import { emitMessage } from "../chat/ChatGateway";
import { error } from "console";
import { match } from "assert";

// Request body interfaces
interface SendMessageBody {
	threadId?: number;
	receiverId?: number;
	senderId: number;
	content: string;
	messageType?: string;
	metadata?: any;
}

interface CreateThreadBody {
	userIds: number[];
	isGroup?: number;
}

interface RelationshipBody {
	userId: number;
	contactId: number;
}

interface GameInvitationBody {
	senderId: number;
	receiverId: number;
	gameTypeId: number;
}

interface InvitationResponseBody {
	status: string;
}

interface NotificationBody {
	userId: number;
	message: string;
	type: string;
	metadata?: any;
}

interface CheckBlockBody {
	userId: number;
	contactId: number;
}

export async function liveChatRoutes(fastify: FastifyInstance) {
  const queries = getQueries();

  //##############################################
  //# THREADS
  //##############################################

  fastify.post<{ Body: CreateThreadBody }>(
    "/chat/thread",
    async (request, reply) => {
      try {
        const { userIds, isGroup = 0 } = request.body;

        if (!userIds || userIds.length < 2) {
          return reply.code(400).send({
            success: false,
            error: "At least two users are required",
          });
        }

        const threadId = await queries.createThread(userIds, isGroup);

        return reply.code(201).send({
          success: true,
          data: threadId,
        });
      } catch (error) {
        console.error("Error:", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to create chat thread",
        });
      }
    },
  );

  fastify.get<{ Params: { userId: string } }>(
    "/chat/threads/:userId",
    async (request, reply) => {
      try {
        const userId = parseId(request.params.userId, "user ID");

        const threads = await queries.getUserThreads(userId);
        return { success: true, data: threads };
      } catch (error) {
        console.error("Error:", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to fetch user threads",
        });
      }
    },
  );

  //##############################################
  //# MESSAGES
  //##############################################

  fastify.post<{ Body: SendMessageBody }>(
    "/chat/message",
    async (request, reply) => {
      try {
        const {
          threadId,
          senderId,
          receiverId,
          content,
          messageType = "text",
          metadata,
        } = request.body;

        if (!senderId || !content) {
          return reply.code(400).send({
            success: false,
            error: "senderId and content are required",
          });
        }

        let finalThreadId = threadId;

        // If no threadId given, create/find one for direct chat
        if (!finalThreadId) {
          if (!receiverId) {
            return reply.code(400).send({
              success: false,
              error: "receiverId is required",
            });
          }

          // Check blocking before creating/finding thread
          const isBlocked = await queries.isBlockedBidirectional(
            senderId,
            receiverId,
          );
          if (isBlocked) {
            return reply.code(403).send({
              success: false,
              error: "Cannot send message: The recipient is blocked",
            });
          }

          // Check existing thread
          const existing = await queries.getDirectThread(senderId, receiverId);
          if (existing) {
            finalThreadId = existing;
          } else {
            finalThreadId = await queries.createThread(
              [senderId, receiverId],
              0,
            );
          }
        }

        // Check blocking
        if (receiverId) {
          const blocked = await queries.isBlocked(receiverId, senderId);
          if (blocked) {
            return reply.code(403).send({
              success: false,
              error: "User has blocked you",
            });
          }
        }

        const msgId = await queries.createMessage(
          finalThreadId,
          senderId,
          content,
          messageType,
          metadata,
        );

        return reply.code(201).send({
          success: true,
          data: { messageId: msgId, threadId: finalThreadId },
        });
      } catch (error) {
        console.error("Error:", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to send message",
        });
      }
    },
  );

  fastify.get<{ Params: { threadId: string } }>(
    "/chat/messages/:threadId",
    async (request, reply) => {
      try {
        const threadId = parseId(request.params.threadId, "thread ID");
        const messages = await queries.getMessages(threadId);
        return { success: true, data: messages };
      } catch (error) {
        console.error("Error:", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to fetch messages",
        });
      }
    },
  );

  //##############################################
  //# RELATIONSHIPS (BLOCKING & FRIENDSHIP)
  //##############################################

  fastify.post<{ Body: RelationshipBody }>(
    "/chat/block",
    async (request, reply) => {
      try {
        const { userId, contactId } = request.body;

        if (!userId || !contactId) {
          return reply.code(400).send({
            success: false,
            error: "userId and contactId are required",
          });
        }

        if (userId === contactId) {
          return reply.code(400).send({
            success: false,
            error: "Cannot block yourself",
          });
        }

        await queries.blockUser(userId, contactId);

        return reply.code(201).send({
          success: true,
          data: "User blocked",
        });
      } catch (error) {
        return reply.code(500).send({
          success: false,
          error: "Failed to block user",
        });
      }
    },
  );

  fastify.post<{ Body: RelationshipBody }>(
    "/chat/unblock",
    async (request, reply) => {
      try {
        const { userId, contactId } = request.body;
        if (!userId || !contactId) {
          return reply.code(400).send({
            success: false,
            error: "userId and contactId are required",
          });
        }
        await queries.unblockUser(userId, contactId);

        return reply.code(200).send({
          success: true,
          data: "User unblocked successfully",
        });
      } catch (error) {
        console.error("Error:", error);
        return reply.code(500).send({
          success: false,
          error: "Failed to unblock user",
        });
      }
    },
  );

  //##############################################
  //# GAME INVITATIONS
  //##############################################

  fastify.post<{ Body: GameInvitationBody }>(
    "/chat/invite",
    async (request, reply) => {
      try {
        const { senderId, receiverId, gameTypeId } = request.body;
        if (!senderId || !receiverId || !gameTypeId) {
          return reply.code(400).send({
            success: false,
            error: "senderId, receiverId, and gameTypeId are required",
          });
        }

        // Check blocking
        const isBlocked = await queries.isBlockedBidirectional(
          senderId,
          receiverId,
        );
        if (isBlocked) {
          return reply.code(403).send({
            success: false,
            error: "Cannot send invitation: blocked relationship exists",
          });
        }

        const invId = await queries.createGameInvitation(
          senderId,
          receiverId,
          gameTypeId,
        );

        return reply.code(201).send({
          success: true,
          data: invId,
        });
      } catch (error) {
        console.error(error);
        return reply.code(500).send({
          success: false,
          error: "Failed to create game invitation",
        });
      }
    },
  );

  fastify.post<{ Params: { id: string }; Body: InvitationResponseBody }>(
    "/chat/invite/:id/respond",
    async (request, reply) => {
      try {
        const invitationId = parseId(request.params.id, "invitation ID");
        const { status } = request.body;

        if (!status || !["accepted", "rejected"].includes(status)) {
          return reply.code(400).send({
            success: false,
            error: "Valid status is required (accepted or rejected)",
          });
        }

        // Get the invitation details
        const invitation = await queries.getGameInvitationById(invitationId);
        if (!invitation) {
          return reply.code(404).send({
            success: false,
            error: "Invitation not found",
          });
        }

        if (invitation.status == "pending") {
          return reply.code(400).send({
            success: false,
            error: "Invitation has already been processed",
          });
        }

        await queries.respondToGameInvitation(invitationId, status);

        // If accepted, create match and add participants
        if (status === "accepted") {
          const matchId = await queries.createMatch(
            invitation.game_type_id,
            null,
            null,
            null,
          );
          await queries.addMatchParticipant(matchId, invitation.sender_id);
          await queries.addMatchParticipant(matchId, invitation.receiver_id);

          return reply.code(200).send({
            success: true,
            data: {
              message: "Invitation accepted",
              matchId: matchId,
            },
          });
        }
        return reply.code(200).send({
          success: false,
          data: "Invitation rejected",
        });
      } catch (error) {
        console.error(error);
        return reply.code(500).send({
          success: false,
          error: "Failed to update invitation",
        });
      }
    },
  );

  //##############################################
  //# NOTIFICATIONS
  //##############################################

  fastify.post<{ Body: NotificationBody }>(
    "/chat/notify",
    async (request, reply) => {
      try {
        const { userId, message, type, metadata } = request.body;

        if (!userId || !message || !type) {
          return reply.code(400).send({
            success: false,
            error: "userId, message, and type are required",
          });
        }

        await queries.createNotification(userId, message, type, metadata);

        return reply.code(201).send({
          success: true,
          data: "Notification created",
        });
      } catch (error) {
        console.error(error);
        return reply.code(500).send({
          success: false,
          error: "Failed to create notification",
        });
      }
    },
  );

  fastify.get<{ Params: { userId: string } }>(
    "/chat/notifications/:userId",
    async (request, reply) => {
      try {
        const userId = parseId(request.params.userId, "user ID");
        const notes = await queries.listNotifications(userId);

        return reply.code(200).send({
          success: true,
          data: notes,
        });
      } catch (error) {
        console.error(error);
        return reply.code(500).send({
          success: false,
          error: "Failed to fetch notifications",
        });
      }
    },
  );

  fastify.post<{ Body: CheckBlockBody }>(
	  "/chat/check-block",
	  async (request, reply) => {
		  try {
			  const { userId, contactId } = request.body;

			  if (!userId || !contactId) {
				  return reply.code(400).send({
					  success: false,
					  error: "userId and contactId are required",
				  });
			  }

			  const isBlocked = await queries.isBlockedBidirectional(userId, contactId);

			  const iAmBlocker = await queries.isBlocked(userId, contactId);

			  return reply.code(200).send({
				  success: true,
				  data: { isBlocked, iAmBlocker },
			  });
		  } catch (error) {
			  console.error("Error:", error);
			  return reply.code(500).send({
				  success: false,
				  error: "Failed to check block status",
			  });
		  }
	  }
  );

  fastify.get<{ Params: { userId: string } }>(
	  "/chat/blocked/:userId",
	  async (request, reply) => {
		  try {
			  const userId = parseId(request.params.userId, "user ID");
			  const blocked = await queries.getBlockedFriends(userId);

			  return reply.code(200).send({
				  success: true,
				  data: blocked || [],
			  });
		  } catch (error) {
			  console.error("Error:", error);
			  return reply.code(500).send({
				  success: false,
				  error: "Failed to fetch blocked users",
			  });
		  }
	  }
  );
}
