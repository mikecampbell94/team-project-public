#pragma once

#include "../Message.h"
#include "../Utilities/Maths/Vector3.h"

class SceneNodeTranslationMessage : public Message
{
public:
	SceneNodeTranslationMessage(const std::string& destinationName, const std::string& resourceName, Vector3 translation, bool relative);
	~SceneNodeTranslationMessage();

	Vector3 translation;
	bool relative;
	std::string resourceName;
};

