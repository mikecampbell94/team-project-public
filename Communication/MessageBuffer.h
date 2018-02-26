#pragma once

#include "MessageStorage.h"

template <class MessageType>
class MessageBuffer
{
public:
	MessageBuffer() {}
	~MessageBuffer() {}

	void insertOutgoingMessage(MessageType message)
	{
		outgoingMessages.push(message);
	}

	void sendMessages(MessageStorage& messageStorage)
	{
		while (!outgoingMessages.empty())
		{
			MessageType message = outgoingMessages.front();
			outgoingMessages.pop();

			sentMessages.push(message);
			messageStorage.sendMessage(&sentMessages.back());
		}
	}

	void clearSentMessages()
	{
		while (!sentMessages.empty())
		{
			sentMessages.pop();
		}
	}

private:
	std::queue<MessageType> outgoingMessages;
	std::queue<MessageType> sentMessages;
};
