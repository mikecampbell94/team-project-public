#include "MoveCameraRelativeToGameObjectMessage.h"

#include "../Resource Management/XMLParser.h"

std::string MoveCameraRelativeToGameObjectMessage::resourceName = "";

MoveCameraRelativeToGameObjectMessage::MoveCameraRelativeToGameObjectMessage(const std::string& destinationName, const std::string& resourceName,
	Vector3 translation, float pitch, float yaw)
	: Message(destinationName, MOVE_CAMERA_RELATIVE_TO_GAMEOBJECT)
{
	//this->resourceName = resourceName;
	this->translation = translation;
	this->pitch = pitch;
	this->yaw = yaw;
}

MoveCameraRelativeToGameObjectMessage::~MoveCameraRelativeToGameObjectMessage()
{
}

MoveCameraRelativeToGameObjectMessage MoveCameraRelativeToGameObjectMessage::builder(Node* node)
{
	std::string nodeDestination = "";
	//std::string nodeResourcename = MoveCameraRelativeToGameObjectMessage::resourceName;
	Vector3 nodeTranslation(0, 0, 0);
	float nodePitch = 0.0f;
	float nodeYaw = 0.0f;

	for each (Node* childNode in node->children)
	{
		if (childNode->nodeType == "destination")
		{
			nodeDestination = childNode->value;
		}
		else if (childNode->nodeType == "translation")
		{
			nodeTranslation = Vector3::builder(childNode);
		}
		else if (childNode->nodeType == "rotation")
		{
			nodePitch = stof(childNode->children[0]->value);
			nodeYaw = stof(childNode->children[1]->value);
		}
		else if (childNode->nodeType == "resource")
		{
			//nodeResourcename = childNode->value;
			MoveCameraRelativeToGameObjectMessage::resourceName = childNode->value;
		}
	}

	return MoveCameraRelativeToGameObjectMessage(nodeDestination, MoveCameraRelativeToGameObjectMessage::resourceName, nodeTranslation, nodePitch, nodeYaw);
}