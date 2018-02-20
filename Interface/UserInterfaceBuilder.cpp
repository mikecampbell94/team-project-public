#include "UserInterfaceBuilder.h"

#include "../Resource Management/Database/Database.h"
#include "../Graphics/GraphicsCommon.h"

std::vector<Button> UserInterfaceBuilder::buildButtons(std::string UIFile, Database* database)
{
	XMLParser xmlParser;
	xmlParser.loadFile(UIFile);

	ButtonActionCreator actionCreator;

	std::vector<Button> buttons;

	for each (Node* buttonNode in xmlParser.parsedXml->children)
	{
		buttons.push_back(buildButton(buttonNode, database, actionCreator));
	}

	return buttons;
}

Button UserInterfaceBuilder::buildButton(Node* node, Database* database, ButtonActionCreator& actionCreator)
{
	const Vector4 colour = getColour(node->children[0]);;
	const Vector2 position = getTransformInformation(node->children[1]);
	const Vector2 scale = getTransformInformation(node->children[2]);

	const ButtonAction action = actionCreator.createButtonAction(node->children[3]->value);
	const std::string text = node->children[4]->value;
	const std::string meshName = node->children[5]->value;

	Mesh* mesh = static_cast<Mesh*>(
		database->getTable("UIMeshes")->getAllResources()->getResource(meshName));

	Font font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	Button button(colour, position, scale, action, new TextMesh(text, font), mesh);

	if (node->children.size() >= 7)
	{
		//for each (Node* buttonNode in node->children)
		for (int i = 6; i < node->children.size(); ++i)
		{
			Button childButton = buildButton(node->children[i], database, actionCreator);
			button.childButtons.push_back(childButton);
		}
	}

	return button;
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
