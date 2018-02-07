#pragma once
#include <map>
#include <queue>
#include <string>
#include "Message.h"
#include "Postman.h"

class MessageStorage : public Postman
{
public:
	MessageStorage();
	~MessageStorage();

	void addMessageBuffer(const std::string& bufferName) override;
	void removeMessageBuffer(const std::string& bufferName);
	std::queue<Message>* getMessageBufferByName(const std::string& bufferName) override;

	void sendMessage(const Message& message) override;
	void clearMessageStorage() override;

private:
	void clearMessageBuffer(const std::string& bufferName);
	void clearMessageBuffer(std::map<std::string, std::queue<Message>>::iterator iter);

	std::map<std::string, std::queue<Message>> activeMessageBuffers;
};