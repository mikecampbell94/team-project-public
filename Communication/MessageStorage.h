#pragma once
#include <map>
#include <queue>
#include <string>
#include "Message.h"

class MessageStorage
{
public:
	void addMessageBuffer(std::string id);

	void pushMessage(Message* message, std::string id);

	std::queue<Message*>* getMessageBufferByID(std::string id);
};