#pragma once
#include <map>
#include <queue>
#include <string>
#include "Message.h"

class MessageStorage
{
public:
	MessageStorage();
	~MessageStorage();

	void addMessageBuffer(std::string id);

	bool removeMessageBuffer(std::string id);

	bool pushMessage(Message* message, std::string id);

	std::queue<Message*>* getMessageBufferByID(std::string id);

	void clearMessageStorage();

private:
	void clearMessageBuffer(std::string id);
	void clearMessageBuffer(std::map<std::string, std::queue<Message*>>::iterator iter);

	std::map<std::string, std::queue<Message*>> activeMessageBuffers;
};