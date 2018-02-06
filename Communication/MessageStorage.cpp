#include "MessageStorage.h"

MessageStorage::MessageStorage()
{

}

MessageStorage::~MessageStorage() 
{
	for (auto iter = activeMessageBuffers.begin(); iter != activeMessageBuffers.end(); iter++) 
	{
		clearMessageBuffer(iter);
	}
	activeMessageBuffers.clear();
}

void MessageStorage::addMessageBuffer(std::string id) 
{
	std::queue<Message*> mq;
	activeMessageBuffers.insert(std::pair<std::string, std::queue<Message*>>(id, mq));
}

bool MessageStorage::removeMessageBuffer(std::string id)
{
	auto iter = activeMessageBuffers.find(id);
	if (iter != activeMessageBuffers.end())
	{
		clearMessageBuffer(iter);
		activeMessageBuffers.erase(iter);
	}
	else
		return false;
}

bool MessageStorage::pushMessage(Message* message, std::string id)
{
	auto iter = activeMessageBuffers.find(id);
	if (iter != activeMessageBuffers.end()) 
	{
		iter->second.push(message);
		return true;
	}
	else
	{
		return false;
	}
}

std::queue<Message*>* MessageStorage::getMessageBufferByID(std::string id)
{
	auto iter = activeMessageBuffers.find(id);
	if (iter != activeMessageBuffers.end())
		return &(iter->second);
	else
		return nullptr;
}

void MessageStorage::clearMessageStorage()
{
	for (auto iter = activeMessageBuffers.begin(); iter != activeMessageBuffers.end(); iter++)
	{
		clearMessageBuffer(iter);
	}
}

void MessageStorage::clearMessageBuffer(std::string id)
{
	auto iter = activeMessageBuffers.find(id);
	if (iter != activeMessageBuffers.end())
		clearMessageBuffer(iter);
}

void MessageStorage::clearMessageBuffer(std::map<std::string, std::queue<Message*>>::iterator iter)
{
	while (!iter->second.empty())
	{
		delete iter->second.front();
		iter->second.pop();
	}
}