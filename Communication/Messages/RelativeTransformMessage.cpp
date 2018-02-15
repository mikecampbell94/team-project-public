#include "RelativeTransformMessage.h"

#include "../Resource Management/XMLParser.h"

RelativeTransformMessage::RelativeTransformMessage(const std::string& destinationName,
	const std::string& resourceName, Matrix4 transform)
	: Message(destinationName, RELATIVE_TRANSFORM)
{
	this->transform = transform;
	this->resourceName = resourceName;
}

RelativeTransformMessage::~RelativeTransformMessage()
{
}

RelativeTransformMessage RelativeTransformMessage::builder(Node* node)
{
	std::string nodeDestination = "";
	std::string nodeResourcename = "";
	Vector3 nodeTranslation(0, 0, 0);
	Vector4 nodeRotation(0, 0, 0, 0);
	Vector3 nodeScale(1, 1, 1);

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
			nodeRotation = Vector4::builder(childNode);
		}
		else if (childNode->nodeType == "scale")
		{
			nodeScale = Vector3::builder(childNode);
		}
		else if (childNode->nodeType == "resource")
		{
			nodeResourcename = childNode->value;
		}
	}

	Matrix4 nodeTransform = Matrix4::translation(nodeTranslation)
	* Matrix4::rotation(nodeRotation.w, Vector3(nodeRotation.x, nodeRotation.y, nodeRotation.z))
	* Matrix4::scale(nodeScale);

	return RelativeTransformMessage(nodeDestination, nodeResourcename, nodeTransform);
}