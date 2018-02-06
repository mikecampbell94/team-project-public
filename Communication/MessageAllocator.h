#pragma once
#include "DeliverySystem.h"
#include "MessageStorage.h"

//Implemented Postman (service provider)
class MessageAllocator :
	public Postman
{
public:
	MessageAllocator();
	~MessageAllocator();

	virtual void broadcastMessage(Message* message);
	virtual void sendMessage(Message* message, std::string bufferID);

	void setStorage(MessageStorage* storage);


private:
	MessageStorage* storage;
};

