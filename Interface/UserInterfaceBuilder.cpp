#include "UserInterfaceBuilder.h"

#include "../Resource Management/Database/Database.h"

std::vector<Button> UserInterfaceBuilder::buildButtons(std::string UIFile, Database* database, Vector2 resolution)
{
	XMLParser xmlParser;
	xmlParser.loadFile(UIFile);

	std::vector<Button> buttons;

	for each (Node* buttonNode in xmlParser.parsedXml->children)
	{
		buttons.push_back(buildButton(buttonNode, database, resolution));
	}

	return buttons;
}

Button UserInterfaceBuilder::buildButton(Node* node, Database* database, Vector2 resolution)
{
	const Vector4 colour = getColour(node->children[0]);
	const Vector2 position = resolution * getTransformInformation(node->children[1]);
	const Vector2 scale = getTransformInformation(node->children[2]);

	const std::string action = node->children[3]->value;
	const std::string text = node->children[4]->value;
	const std::string meshName = node->children[5]->value;

	Mesh* mesh = static_cast<Mesh*>(
		database->getTable("Meshes")->getAllResources()->getResource(meshName));

	return Button(colour, position, scale, action, text, mesh);
}

Vector4 UserInterfaceBuilder::getColour(Node* node)
{
	const float r = std::stof(node->children[0]->value);
	const float g = std::stof(node->children[1]->value);
	const float b = std::stof(node->children[2]->value);
	const float a = std::stof(node->children[3]->value);

	return Vector4(r, g, b, a);
}

Vector2 UserInterfaceBuilder::getTransformInformation(Node* node)
{
	const float x = std::stof(node->children[0]->value);
	const float y = std::stof(node->children[1]->value);

	return Vector2(x, y);
}
