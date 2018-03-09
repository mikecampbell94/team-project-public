#pragma once

#include "XMLParser.h"
#include <string>
#include <windows.h>
#include "Maths/Vector3.h"
#include "Maths/Vector4.h"

class Database;

struct LevelGameObjectsState
{
	std::vector<bool> hasPhysicsNode;

	std::vector<std::string> gameObjectNames;
	std::vector<std::string> gameObjectMeshNames;
	std::vector<std::array<std::string, 4>> gameObjectColours;
	std::vector<std::array<std::string, 3>> gameObjectPositions;
	std::vector<std::array<std::string, 4>> gameObjectRotations;
	std::vector<std::array<std::string, 3>> gameObjectScales;

	std::vector<std::string> physicsNodeEnabled;
	std::vector<std::string> physicsNodeTransmitCollision;
	std::vector<std::string> collisionShapes;
	std::vector<std::string> mass;
	std::vector<std::string> isCollision;
	std::vector<std::string> elasticity;
	std::vector<std::string> friction;
	std::vector<std::string> damping;
	std::vector<std::string> isStatic;
};

class XMLWriter
{
public:
	XMLWriter(Database* database);
	~XMLWriter();

	void saveLevelFile(std::string levelName);

private:
	void createFolder(const char* path);
	void saveMeshFile(std::string meshFileName);
	void saveLevelDetails(std::string levelDetailsFile);
	LevelGameObjectsState getGameObjectStates();

	Database* database;
};

