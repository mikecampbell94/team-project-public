#pragma once

#include "../Graphics/Meshes/Mesh.h"
#include "../Resource Management/XMLParser.h"

#include <vector>
#include "ButtonActionCreator.h"

class Database;

struct Button
{
	Button(Vector4 colour, Vector2 position, Vector2 scale,
		ButtonAction action, std::string text, Mesh* mesh)
	{
		this->colour = colour;
		this->position = position;
		this->scale = scale;
		this->action = action;
		this->text = text;
		this->UIMesh = mesh;
	}

	Vector4 colour;
	Vector2 position;
	Vector2 scale;

	ButtonAction action;
	std::string text;

	Mesh* UIMesh;
};

class UserInterfaceBuilder
{
public:
	static std::vector<Button> buildButtons(std::string UIFile, Database* database, Vector2 resolution);

private:
	static Button buildButton(Node* node, Database* database, Vector2 resolution, ButtonActionCreator& actionCreator);

	static Vector4 getColour(Node* node);
	static Vector2 getTransformInformation(Node* node);
};

