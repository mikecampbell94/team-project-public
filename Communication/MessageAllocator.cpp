#include "MessageAllocator.h"



MessageAllocator::MessageAllocator()
{
}


MessageAllocator::~MessageAllocator()
{
}

void MessageAllocator::broadcastMessage(Message * message)
{
	
}

void MessageAllocator::sendMessage(Message * message, std::string bufferID)
{
	storage->pushMessage(message, bufferID);
}

void MessageAllocator::setStorage(MessageStorage * storage)
{
	this->storage = storage;
}
