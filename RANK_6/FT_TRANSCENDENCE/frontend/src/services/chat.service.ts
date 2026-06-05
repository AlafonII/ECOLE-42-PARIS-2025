import {
  ApiResponse,
  ChatMessage,
  ChatThread,
  CreateThreadInput,
  Notification,
  SendMessageInput,
} from "./types";
import { apiService } from "./api.service";
import { API_CONFIG } from "./api.config";

export class ChatService {
  async createThread(input: CreateThreadInput): Promise<ApiResponse<number>> {
    return apiService.post<number>(
      API_CONFIG.ENDPOINTS.CHAT.CREATE_THREAD,
      input,
    );
  }

  async getUserThreads(userId: number): Promise<ApiResponse<ChatThread[]>> {
    return apiService.get<ChatThread[]>(
      API_CONFIG.ENDPOINTS.CHAT.USER_THREADS(userId),
    );
  }

  async sendMessage(
    input: SendMessageInput,
  ): Promise<ApiResponse<{ messageId: number; threadId: number }>> {
    return apiService.post<{ messageId: number; threadId: number }>(
      API_CONFIG.ENDPOINTS.CHAT.SEND_MESSAGE,
      input,
    );
  }

  async getMessages(threadId: number): Promise<ApiResponse<ChatMessage[]>> {
    return apiService.get<ChatMessage[]>(
      API_CONFIG.ENDPOINTS.CHAT.GET_MESSAGES(threadId),
    );
  }

  async blockUser(
    userId: number,
    contactId: number,
  ): Promise<ApiResponse<string>> {
    return apiService.post<string>(API_CONFIG.ENDPOINTS.CHAT.BLOCK, {
      userId,
      contactId,
    });
  }

  async unblockUser(
    userId: number,
    contactId: number,
  ): Promise<ApiResponse<string>> {
    return apiService.post<string>(API_CONFIG.ENDPOINTS.CHAT.UNBLOCK, {
      userId,
      contactId,
    });
  }

  async sendGameInvitation(
    senderId: number,
    receiverId: number,
    gameTypeId: number,
  ): Promise<ApiResponse<number>> {
    return apiService.post<number>(API_CONFIG.ENDPOINTS.CHAT.INVITE, {
      senderId,
      receiverId,
      gameTypeId,
    });
  }

  async respondToInvitation(
    invitationId: number,
    status: string,
  ): Promise<ApiResponse<{ message: string; matchId?: number }>> {
    return apiService.post<{ message: string; matchId?: number }>(
      API_CONFIG.ENDPOINTS.CHAT.RESPOND_INVITE(invitationId),
      {
        status,
      },
    );
  }

  async createNotification(
    userId: number,
    message: string,
    type: string,
    metadata?: any,
  ): Promise<ApiResponse<string>> {
    return apiService.post<string>(API_CONFIG.ENDPOINTS.CHAT.NOTIFY, {
      userId,
      message,
      type,
      metadata,
    });
  }

  async getNotifications(userId: number): Promise<ApiResponse<Notification[]>> {
    return apiService.get<Notification[]>(
      API_CONFIG.ENDPOINTS.CHAT.NOTIFICATIONS(userId),
    );
  }

  async checkBlockStatus(
	  userId: number,
	  contactId: number,
  ): Promise<ApiResponse<{ isBlocked: boolean; iAmBlocker: boolean }>> {
	  return apiService.post<{ isBlocked: boolean; iAmBlocker: boolean }>(
		  API_CONFIG.ENDPOINTS.CHAT.CHECK_BLOCK,
		  {
			  userId,
			  contactId,
		  },
	  );
  }

  async getBlockedUsers(userId: number): Promise<ApiResponse<any[]>> {
	  return apiService.get<any[]>(
		  API_CONFIG.ENDPOINTS.CHAT.BLOCKED_USERS(userId),
	  );
  }
}
