#pragma once
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsNode.h"
#include "Database\Database.h"
#include "XMLParser.h"
#include <unordered_map>
#include <functional>


class GameObjectBuilder
{
public:
	GameObjectBuilder();
	~GameObjectBuilder();

	static GameObject* buildGameObject(Node* node, Database* database);
	static SceneNode* buildSceneNode(Node* node, Database* database);
	static PhysicsNode* buildPhysicsNode(Node* node, GameObject* parent);


	static Vector3 buildVector3(Node* node);
	static Vector4 buildVector4(Node* node);
};

