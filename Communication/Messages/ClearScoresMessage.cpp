#include "ClearScoresMessage.h"

#include "../Resource Management/XMLParser.h"

ClearScoresMessage::ClearScoresMessage(const std::string& destinationName, const std::string& moduleName) : Message(destinationName, CLEAR_SCORE)
{
	this->moduleName = moduleName;
}

ClearScoresMessage::~ClearScoresMessage()
{
}
