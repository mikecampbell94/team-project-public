#include "MessageStorage.h"

MessageStorage::MessageStorage()
{

}

MessageStorage::~MessageStorage() 
{
	clearMessageStorage();
	activeMessageBuffers.clear();
}

void MessageStorage::addMessageBuffer(std::string bufferName) 
{
	activeMessageBuffers.insert(std::pair<std::string, std::queue<Message*>>(bufferName, std::queue<Message*> ()));
}

void MessageStorage::removeMessageBuffer(std::string bufferName)
{
	clearMessageBuffer(bufferName);
	activeMessageBuffers.erase(bufferName);
}

void MessageStorage::sendMessage(Message* message, std::string bufferName)
{
	activeMessageBuffers.at(bufferName).push(message);
}

std::queue<Message*>* MessageStorage::getMessageBufferByName(std::string bufferName)
{
	return &activeMessageBuffers.at(bufferName);
}

void MessageStorage::clearMessageStorage()
{
	for (auto iter = activeMessageBuffers.begin(); iter != activeMessageBuffers.end(); iter++)
	{
		clearMessageBuffer(iter);
	}
}

void MessageStorage::clearMessageBuffer(std::string bufferName)
{
	std::queue<Message*> buffer = activeMessageBuffers.at(bufferName);
	while (!buffer.empty())
	{
		delete buffer.front();
		buffer.pop();
	}
}

void MessageStorage::clearMessageBuffer(std::map<std::string, std::queue<Message*>>::iterator iter)
{
	while (!iter->second.empty())
	{
		delete iter->second.front();
		iter->second.pop();
	}
}