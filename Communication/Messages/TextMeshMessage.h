#pragma once

#include "../Message.h"
#include "../Utilities/Maths/Vector3.h"

class TextMeshMessage : public Message
{
public:
	TextMeshMessage(const std::string& destinationName, const std::string& text, 
		Vector3 position, Vector3 scale, Vector3 colour, bool orthographic);
	~TextMeshMessage();

	bool orthographic;
	std::string text;
	Vector3 position;
	Vector3 scale;
	Vector3 colour;
};

