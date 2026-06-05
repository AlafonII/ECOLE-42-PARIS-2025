export type ChatWSMessage = | ChatInitMessage | ChatDirectMessage | ChatBroadcastMessage | ChatReceiveMessage;

export type ChatInitMessage = {
	type: "INIT";
	userId: number;
};

export type ChatDirectMessage = {
	type: "DIRECT_MESSAGE";
	toUserId: number;
	content: string;
};

export type ChatBroadcastMessage = {
	type: "BROADCAST";
	content: string;
};

export type ChatReceiveMessage = {
	type: "RECEIVE_MESSAGE";
	fromUserId: number;
	content: string;
	timestamp: number;
	messageType?: string;
	metadata?: any;
};
