#pragma once

#include "../Graphics/Meshes/Mesh.h"
#include "../Resource Management/XMLParser.h"

#include <vector>
#include "ButtonActionCreator.h"
#include "../Graphics/Meshes/TextMesh.h"

class Database;

struct Button
{
	Button(Vector4 colour, Vector2 position, Vector2 scale,
		ButtonAction action, TextMesh* textMesh, Mesh* mesh)
	{
		this->colour = colour;
		this->position = position;
		this->scale = scale;
		this->action = action;
		this->textMesh = textMesh;
		this->UIMesh = mesh;

		selected = false;
	}

	Vector4 colour;
	Vector2 position;
	Vector2 scale;

	bool selected;

	ButtonAction action;
	TextMesh* textMesh;

	Mesh* UIMesh;
};

class UserInterfaceBuilder
{
public:
	static std::vector<Button> buildButtons(std::string UIFile, Database* database);

private:
	static Button buildButton(Node* node, Database* database, ButtonActionCreator& actionCreator);

	static Vector4 getColour(Node* node);
	static Vector2 getTransformInformation(Node* node);
};

