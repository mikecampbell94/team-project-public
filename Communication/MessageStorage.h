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

	void addMessageBuffer(std::string bufferName);

	void removeMessageBuffer(std::string bufferName);

	void sendMessage(Message* message, std::string bufferName);

	std::queue<Message*>* getMessageBufferByName(std::string bufferName);

	void clearMessageStorage();

private:
	void clearMessageBuffer(std::string bufferName);
	void clearMessageBuffer(std::map<std::string, std::queue<Message*>>::iterator iter);

	std::map<std::string, std::queue<Message*>> activeMessageBuffers;
};