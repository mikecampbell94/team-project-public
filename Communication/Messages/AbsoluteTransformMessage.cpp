#include "AbsoluteTransformMessage.h"

AbsoluteTransformMessage::AbsoluteTransformMessage(const std::string& destinationName, const std::string& resourceName,
	Matrix4 transform)
	: Message(destinationName, ABSOLUTE_TRANSFORM)
{
	this->transform = transform;
	this->resourceName = resourceName;
}

AbsoluteTransformMessage::~AbsoluteTransformMessage()
{
}

AbsoluteTransformMessage AbsoluteTransformMessage::builder(Node* node)
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

	return AbsoluteTransformMessage(nodeDestination, nodeResourcename, nodeTransform);
}

AbsoluteTransformMessage AbsoluteTransformMessage::tokensToMessage(std::vector<std::string> lineTokens)
{
	std::string nodeDestination = lineTokens[1];
	std::string nodeResourcename = lineTokens[2];
	Vector3 nodeTranslation = Vector3::builder(lineTokens[3].substr(12));
	Vector4 nodeRotation = Vector4::builder(lineTokens[4].substr(9));
	Vector3 nodeScale = Vector3::builder(lineTokens[5].substr(6));

	Matrix4 nodeTransform = Matrix4::translation(nodeTranslation)
		* Matrix4::rotation(nodeRotation.w, Vector3(nodeRotation.x, nodeRotation.y, nodeRotation.z))
		* Matrix4::scale(nodeScale);

	return AbsoluteTransformMessage(nodeDestination, nodeResourcename, nodeTransform);
}