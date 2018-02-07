#pragma once

#include "Message.h"
#include <queue>
#include <functional>
#include <unordered_map>

typedef std::queue<Message*> MessageBuffer;
typedef std::function<void(Message*)> Action;

class MessageProcessor
{
public:
	MessageProcessor() {}
	MessageProcessor(std::vector<MessageType> typeOfMessagesToListenFor, 
		MessageBuffer* subsystemBuffer);
	~MessageProcessor();

	void addActionToExecuteOnMessage(const MessageType& typeOfMessageToPerformOn, const Action& action);
	void processMessagesInBuffer();

private:
	MessageBuffer* subsystemMessageBuffer;
	std::unordered_map<MessageType, std::vector<Action>> actionsToExecute;
};

