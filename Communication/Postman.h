#pragma once

#include "Message.h"

#include <string>
#include <queue>

//exposed interface
class Postman 
{
public:
	virtual void sendMessage(const Message& message) = 0;
	virtual void addMessageBuffer(const std::string& bufferName) = 0;
	virtual std::queue<Message>* getMessageBufferByName(const std::string& bufferName) = 0;
	virtual void clearMessageStorage() = 0;
};