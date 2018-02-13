#include "SceneNodeTranslationMessage.h"

SceneNodeTranslationMessage::SceneNodeTranslationMessage(const std::string& destinationName, 
	const std::string& resourceName, Vector3 translation, bool relative) 
	: Message(destinationName, TRANSLATE_SCENE_NODE)
{
	this->translation = translation;
	this->resourceName = resourceName;
	this->relative = relative;
}

SceneNodeTranslationMessage::~SceneNodeTranslationMessage()
{
}
