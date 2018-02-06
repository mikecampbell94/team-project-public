#pragma once
#include "DeliverySystem.h"

//Implemented Postman (service provider)
class MessageAllocator :
	public Postman
{
public:
	MessageAllocator();
	~MessageAllocator();

	virtual void broadcastMessage(Message* message);
	virtual void sendMessage(Message* message, std::string bufferID);

private:
	MessageStorage storage;
};

