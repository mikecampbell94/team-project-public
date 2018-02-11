#pragma once
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Gameplay/GameObject.h"
#include "../Graphics/Meshes/Mesh.h"
#include "Resource.h"
#include "XMLParser.h"
#include <unordered_map>
#include <functional>

class Database;


typedef std::function<void(node*, Database*)> objectbuild;

class ObjectBuilder
{
public:
	ObjectBuilder();
	~ObjectBuilder();

	void addBuilder(std::string className,objectbuild buildFunction);




private:
	unordered_map<std::string, objectbuild> builders;
};

