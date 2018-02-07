#pragma once
#include "Message.h"
#include <string>

//exposed interface
class Postman 
{
public:
	virtual void sendMessage(Message* message, std::string bufferName) = 0;
};