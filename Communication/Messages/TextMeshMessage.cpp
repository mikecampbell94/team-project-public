#include "TextMeshMessage.h"

TextMeshMessage::TextMeshMessage(const std::string& destinationName, const std::string& text,
	Vector3 position, Vector3 scale, bool orthographic)
	: Message(destinationName, TEXT_MESH_MESSAGE)
{
	this->orthographic = orthographic;
	this->text = text;
	this->position = position;
	this->scale = scale;
}

TextMeshMessage::~TextMeshMessage()
{
}
