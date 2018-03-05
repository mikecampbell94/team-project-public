#include "TextMeshMessage.h"

TextMeshMessage::TextMeshMessage(const std::string& destinationName, const std::string& text,
	Vector3 position, Vector3 scale, Vector3 colour, bool orthographic, bool hasBackground)
	: Message(destinationName, TEXT_MESH_MESSAGE)
{
	this->hasBackground = hasBackground;
	this->orthographic = orthographic;
	this->text = text;
	this->position = position;
	this->scale = scale;
	this->colour = colour;
}

TextMeshMessage::~TextMeshMessage()
{
}
