#pragma once
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicseNode.h"
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
	static PhysicsNode* buildPhysicsNode(Node* node);


	static Vector3 & buildVector3(Node* node);
	static Vector4 & buildVector4(Node* node);
};

