#include "MessageProcessor.h"

MessageProcessor::MessageProcessor(std::vector<MessageType> typeOfMessagesToListenFor, 
	MessageBuffer* subsystemBuffer)
{
	subsystemMessageBuffer = subsystemBuffer;

	for each (MessageType messageType in typeOfMessagesToListenFor)
	{
		actionsToExecute.insert({ messageType, std::vector<Action>() });
	}
}

MessageProcessor::~MessageProcessor()
{
}

void MessageProcessor::addActionToExecuteOnMessage(const MessageType& typeOfMessageToPerformOn, 
	const Action& action)
{
	actionsToExecute.at(typeOfMessageToPerformOn).push_back(action);
}

void MessageProcessor::processMessagesInBuffer()
{
	while (!subsystemMessageBuffer->empty())
	{
		Message* message = subsystemMessageBuffer->front();

		for each (Action action in actionsToExecute.at(message->getMessageType()))
		{
			action(message);
		}

		subsystemMessageBuffer->pop();
	}
}
