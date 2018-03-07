#include "XMLWriter.h"

#include "FilePaths.h"
#include "Database/Database.h"
//#include "../Graphics/Meshes/Mesh.h"
//#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsNode.h"
#include "../Extrernal Libs/rapidxml-1.13/rapidxml_print.hpp"

//#include <stdio.h>
#include <fstream>
//#include <fileapi.h>

XMLWriter::XMLWriter(Database* database)
{
	this->database = database;
}

XMLWriter::~XMLWriter()
{
}

void XMLWriter::saveLevelFile(std::string levelName)
{
	createFolder((LEVELDIR + levelName).c_str());

	rapidxml::xml_document<> levelFile;
	rapidxml::xml_node<>* root = levelFile.allocate_node(rapidxml::node_element, "Level");
	levelFile.append_node(root);

	std::string s = levelName + "/Meshes.xml";

	saveMeshFile(s);

	rapidxml::xml_node<>* fileNode = levelFile.allocate_node(rapidxml::node_element, "File", s.c_str());
	root->append_node(fileNode);
	std::string s1 = levelName + "/Details.xml";
	rapidxml::xml_node<>* fileNode1 = levelFile.allocate_node(rapidxml::node_element, "File", s1.c_str());
	root->append_node(fileNode1);
	//rapidxml::xml_node<>* fileNode2 = levelFile.allocate_node(rapidxml::node_element, "File", "file2.xml");
	//root->append_node(fileNode2);
	//rapidxml::xml_node<>* fileNode3 = levelFile.allocate_node(rapidxml::node_element, "File", "file3.xml");
	//root->append_node(fileNode3);

	rapidxml::xml_node<>* UI = levelFile.allocate_node(rapidxml::node_element, "UI");
	root->append_node(UI);
	rapidxml::xml_node<>* uiFile = levelFile.allocate_node(rapidxml::node_element, "File", "../Data/UserInterface/InGameMenu.xml");
	UI->append_node(uiFile);

	//rapidxml::xml_node<>* GamePlay = levelFile.allocate_node(rapidxml::node_element, "GamePlay");
	//root->append_node(GamePlay);
	//rapidxml::xml_node<>* gameplayFile = levelFile.allocate_node(rapidxml::node_element, "File", "gameplayFile.xml");
	//GamePlay->append_node(gameplayFile);

	//rapidxml::xml_node<>* GameLogic = levelFile.allocate_node(rapidxml::node_element, "GameLogic");
	//root->append_node(GameLogic);
	//rapidxml::xml_node<>* gameLogicFile1 = levelFile.allocate_node(rapidxml::node_element, "File", "gameLogicFile1.xml");
	//GameLogic->append_node(gameLogicFile1);
	//rapidxml::xml_node<>* gameLogicFile2 = levelFile.allocate_node(rapidxml::node_element, "File", "gameLogicFile2.xml");
	//GameLogic->append_node(gameLogicFile2);

	std::ofstream file_stored((LEVELDIR + levelName + ".xml").c_str());
	file_stored << levelFile;
	file_stored.close();
	levelFile.clear();
}

void XMLWriter::createFolder(const char* path)
{
	//if (!CreateDirectory(path, NULL))
	//{
	//	throw std::runtime_error("Cannot create directory.");
	//}

	CreateDirectory(path, NULL);
}

void XMLWriter::saveMeshFile(std::string meshFileName)
{
	rapidxml::xml_document<> meshFile;
	rapidxml::xml_node<>* root = meshFile.allocate_node(rapidxml::node_element, "LevelMeshes");
	meshFile.append_node(root);

	auto meshes = database->getTable("Meshes")->getAllResources()->getResourceBuffer();

	std::vector<std::string> meshNames;
	std::vector<std::string> meshFileNames;
	std::vector<std::string> meshTextures;

	for (auto meshIterator = meshes.begin(); meshIterator != meshes.end(); ++meshIterator)
	{
		meshNames.push_back((*meshIterator).second->getName());

		Mesh* mesh = static_cast<Mesh*>((*meshIterator).second);
		meshFileNames.push_back(mesh->file);

		if (mesh->hasTexture)
		{
			meshTextures.push_back(mesh->textureFile);
		}
		else
		{
			meshTextures.push_back("");
		}
	}

	for (int i = 0; i < meshNames.size(); ++i)
	{
		rapidxml::xml_node<>* meshNode = meshFile.allocate_node(rapidxml::node_element, "Meshes");
		meshNode->append_attribute(meshFile.allocate_attribute("name", meshNames[i].c_str()));
		root->append_node(meshNode);

		rapidxml::xml_node<>* meshFileNode = meshFile.allocate_node(rapidxml::node_element, "MeshFile", meshFileNames[i].c_str());
		meshNode->append_node(meshFileNode);

		if (meshTextures[i] != "")
		{
			rapidxml::xml_node<>* textureFileNode = meshFile.allocate_node(rapidxml::node_element, "texture", meshTextures[i].c_str());
			meshNode->append_node(textureFileNode);
		}
	}

	std::ofstream file_stored((LEVELDIR + meshFileName).c_str());
	file_stored << meshFile;
	file_stored.close();
	meshFile.clear();
}

void XMLWriter::saveLevelDetails(std::string levelDetailsFile)
{
	rapidxml::xml_document<> levelDetails;
	rapidxml::xml_node<>* root = levelDetails.allocate_node(rapidxml::node_element, "LevelDetails");
	levelDetails.append_node(root);

	auto gameObjects = database->getTable("GameObjects")->getAllResources()->getResourceBuffer();

	
	std::vector<std::string> gameObjectNames;
	std::vector<std::string> gameObjectMeshNames;
	std::vector<NCLVector4> gameObjectColours;
	std::vector<NCLVector3> gameObjectPositions;
	std::vector<NCLVector4> gameObjectRotations;
	std::vector<NCLVector3> gameObjectScales;

	for (auto gameObjectIterator = gameObjects.begin(); gameObjectIterator != gameObjects.end(); ++gameObjectIterator)
	{
		GameObject* gameObject = static_cast<GameObject*>((*gameObjectIterator).second);

		gameObjectNames.push_back((*gameObjectIterator).first);
		gameObjectMeshNames.push_back(gameObject->getSceneNode()->GetMesh()->getName());
		gameObjectColours.push_back(gameObject->getSceneNode()->getColour());
		gameObjectScales.push_back(gameObject->getScale());
		
		if (gameObject->getPhysicsNode() != nullptr)
		{
			gameObjectPositions.push_back(gameObject->getPhysicsNode()->getPosition());
			gameObjectRotations.push_back(Quaternion::quaternionToAxisAngle(gameObject->getPhysicsNode()->getOrientation()));
		}
		else
		{
			gameObjectPositions.push_back(gameObject->getSceneNode()->GetWorldTransform().getPositionVector());
			gameObjectRotations.push_back(gameObject->getSceneNode()->axisAngleRotation);
		}
	}
}
