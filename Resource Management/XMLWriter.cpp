#include "XMLWriter.h"

#include "FilePaths.h"
#include "Database/Database.h"
//#include "../Graphics/Meshes/Mesh.h"
//#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsNode.h"
#include "../Extrernal Libs/rapidxml-1.13/rapidxml_print.hpp"

//#include <stdio.h>
#include <fstream>
#include <array>
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
	saveLevelDetails(s1);

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
	const LevelGameObjectsState state = getGameObjectStates();

	rapidxml::xml_document<> levelDetails;
	rapidxml::xml_node<>* root = levelDetails.allocate_node(rapidxml::node_element, "LevelDetails");
	levelDetails.append_node(root);

	//std::string r;
	//std::string g;
	//std::string b;
	//std::string a;

	std::vector<std::vector<std::array<std::string, 4>>> stringVectors(state.hasPhysicsNode.size());

	int physicsNodeIndex = 0;
	for (int i = 0; i < state.hasPhysicsNode.size(); ++i)
	{
		rapidxml::xml_node<>* gameObjectNode = levelDetails.allocate_node(rapidxml::node_element, "GameObjects");
		gameObjectNode->append_attribute(levelDetails.allocate_attribute("name", state.gameObjectNames[i].c_str()));
		root->append_node(gameObjectNode);

		rapidxml::xml_node<>* sceneNodeNode = levelDetails.allocate_node(rapidxml::node_element, "SceneNode");
		gameObjectNode->append_node(sceneNodeNode);

		rapidxml::xml_node<>* meshNode = levelDetails.allocate_node(rapidxml::node_element, "Mesh", state.gameObjectMeshNames[i].c_str());
		sceneNodeNode->append_node(meshNode);

		rapidxml::xml_node<>* colourNode = levelDetails.allocate_node(rapidxml::node_element, "Colour");
		sceneNodeNode->append_node(colourNode);

		rapidxml::xml_node<>* rNode = levelDetails.allocate_node(rapidxml::node_element, "R", state.gameObjectColours[i][0].c_str());
		colourNode->append_node(rNode);
		rapidxml::xml_node<>* gNode = levelDetails.allocate_node(rapidxml::node_element, "G", state.gameObjectColours[i][1].c_str());
		colourNode->append_node(gNode);
		rapidxml::xml_node<>* bNode = levelDetails.allocate_node(rapidxml::node_element, "B", state.gameObjectColours[i][2].c_str());
		colourNode->append_node(bNode);
		rapidxml::xml_node<>* aNode = levelDetails.allocate_node(rapidxml::node_element, "A", state.gameObjectColours[i][3].c_str());
		colourNode->append_node(aNode);

		rapidxml::xml_node<>* enabledNode = levelDetails.allocate_node(rapidxml::node_element, "Enabled", "True/False");
		gameObjectNode->append_node(enabledNode);

		rapidxml::xml_node<>* positionNode = levelDetails.allocate_node(rapidxml::node_element, "Position");
		gameObjectNode->append_node(positionNode);
		rapidxml::xml_node<>* positionXNode = levelDetails.allocate_node(rapidxml::node_element, "X", state.gameObjectPositions[i][0].c_str());
		positionNode->append_node(positionXNode);
		rapidxml::xml_node<>* positionYNode = levelDetails.allocate_node(rapidxml::node_element, "Y", state.gameObjectPositions[i][1].c_str());
		positionNode->append_node(positionYNode);
		rapidxml::xml_node<>* positionZNode = levelDetails.allocate_node(rapidxml::node_element, "Z", state.gameObjectPositions[i][2].c_str());
		positionNode->append_node(positionZNode);

		rapidxml::xml_node<>* rotationNode = levelDetails.allocate_node(rapidxml::node_element, "Rotation");
		gameObjectNode->append_node(rotationNode);
		rapidxml::xml_node<>* rotationXNode = levelDetails.allocate_node(rapidxml::node_element, "X", state.gameObjectRotations[i][0].c_str());
		rotationNode->append_node(rotationXNode);
		rapidxml::xml_node<>* rotationYNode = levelDetails.allocate_node(rapidxml::node_element, "Y", state.gameObjectRotations[i][1].c_str());
		rotationNode->append_node(rotationYNode);
		rapidxml::xml_node<>* rotationZNode = levelDetails.allocate_node(rapidxml::node_element, "Z", state.gameObjectRotations[i][2].c_str());
		rotationNode->append_node(rotationZNode);
		rapidxml::xml_node<>* rotationWNode = levelDetails.allocate_node(rapidxml::node_element, "W", state.gameObjectRotations[i][3].c_str());
		rotationNode->append_node(rotationWNode);

		rapidxml::xml_node<>* scaleNode = levelDetails.allocate_node(rapidxml::node_element, "Scale");
		gameObjectNode->append_node(scaleNode);
		rapidxml::xml_node<>* scaleXNode = levelDetails.allocate_node(rapidxml::node_element, "X", state.gameObjectScales[i][0].c_str());
		scaleNode->append_node(scaleXNode);
		rapidxml::xml_node<>* scaleYNode = levelDetails.allocate_node(rapidxml::node_element, "Y", state.gameObjectScales[i][1].c_str());
		scaleNode->append_node(scaleYNode);
		rapidxml::xml_node<>* scaleZNode = levelDetails.allocate_node(rapidxml::node_element, "Z", state.gameObjectScales[i][2].c_str());
		scaleNode->append_node(scaleZNode);

		if (state.hasPhysicsNode[i])
		{
			rapidxml::xml_node<>* physicsNodeNode = levelDetails.allocate_node(rapidxml::node_element, "PhysicsNode");
			gameObjectNode->append_node(physicsNodeNode);

			rapidxml::xml_node<>* physicsNodeEnabledNode = levelDetails.allocate_node(rapidxml::node_element, "Enabled", 
				state.physicsNodeEnabled[physicsNodeIndex].c_str());
			physicsNodeNode->append_node(physicsNodeEnabledNode);

			rapidxml::xml_node<>* transmitCollisionNode = levelDetails.allocate_node(rapidxml::node_element, "TransmitCollision", 
				state.physicsNodeTransmitCollision[physicsNodeIndex].c_str());
			physicsNodeNode->append_node(transmitCollisionNode);

			rapidxml::xml_node<>* collisionShapeNode = levelDetails.allocate_node(rapidxml::node_element, "CollisionShape", 
				state.collisionShapes[physicsNodeIndex].c_str());
			physicsNodeNode->append_node(collisionShapeNode);

			rapidxml::xml_node<>* massNode = levelDetails.allocate_node(rapidxml::node_element, "Mass",
				state.mass[physicsNodeIndex].c_str());
			physicsNodeNode->append_node(massNode);

			rapidxml::xml_node<>* isCollisionNode = levelDetails.allocate_node(rapidxml::node_element, "isCollision",
				state.isCollision[physicsNodeIndex].c_str());
			physicsNodeNode->append_node(isCollisionNode);

			rapidxml::xml_node<>* elasticityNode = levelDetails.allocate_node(rapidxml::node_element, "Elasticity",
				state.elasticity[physicsNodeIndex].c_str());
			physicsNodeNode->append_node(elasticityNode);

			rapidxml::xml_node<>* frictionNode = levelDetails.allocate_node(rapidxml::node_element, "Friction",
				state.friction[physicsNodeIndex].c_str());
			physicsNodeNode->append_node(frictionNode);

			rapidxml::xml_node<>* dampingNode = levelDetails.allocate_node(rapidxml::node_element, "Damping",
				state.damping[physicsNodeIndex].c_str());
			physicsNodeNode->append_node(dampingNode);

			rapidxml::xml_node<>* isStaticNode = levelDetails.allocate_node(rapidxml::node_element, "isStatic",
				state.isStatic[physicsNodeIndex].c_str());
			physicsNodeNode->append_node(isStaticNode);

			++physicsNodeIndex;
		}
	}

	std::ofstream file_stored((LEVELDIR + levelDetailsFile).c_str());
	file_stored << levelDetails;
	file_stored.close();
	levelDetails.clear();
}

LevelGameObjectsState XMLWriter::getGameObjectStates()
{
	auto gameObjects = database->getTable("GameObjects")->getAllResources()->getResourceBuffer();

	LevelGameObjectsState state;

	for (auto gameObjectIterator = gameObjects.begin(); gameObjectIterator != gameObjects.end(); ++gameObjectIterator)
	{
		GameObject* gameObject = static_cast<GameObject*>((*gameObjectIterator).second);

		state.gameObjectNames.push_back((*gameObjectIterator).first);
		state.gameObjectMeshNames.push_back(gameObject->getSceneNode()->GetMesh()->getName());
		//state.gameObjectColours.push_back(gameObject->getSceneNode()->getColour());

		state.gameObjectColours.push_back(std::array<std::string, 4>
		{
			std::to_string(gameObject->getSceneNode()->getColour().x),
			std::to_string(gameObject->getSceneNode()->getColour().y),
			std::to_string(gameObject->getSceneNode()->getColour().z),
			std::to_string(gameObject->getSceneNode()->getColour().w)
		});

		state.gameObjectScales.push_back(std::array<std::string, 3>
		{
			std::to_string(gameObject->getScale().x),
			std::to_string(gameObject->getScale().y),
			std::to_string(gameObject->getScale().z)
		});

		if (gameObject->getPhysicsNode() != nullptr)
		{
			PhysicsNode* physicsNode = gameObject->getPhysicsNode();

			state.gameObjectPositions.push_back(std::array<std::string, 3>
			{
				std::to_string(physicsNode->getPosition().x),
				std::to_string(physicsNode->getPosition().y),
				std::to_string(physicsNode->getPosition().z)
			});

			NCLVector4 rotation = Quaternion::quaternionToAxisAngle(physicsNode->getOrientation());

			state.gameObjectRotations.push_back(std::array<std::string, 4>
			{
				std::to_string(rotation.x),
				std::to_string(rotation.y),
				std::to_string(rotation.z),
				std::to_string(rotation.w)
			});

			state.hasPhysicsNode.push_back(true);

			if (physicsNode->getEnabled())
			{
				state.physicsNodeEnabled.push_back("True");
			}
			else
			{
				state.physicsNodeEnabled.push_back("False");
			}

			if (physicsNode->transmitCollision)
			{
				state.physicsNodeTransmitCollision.push_back("True");
			}
			else
			{
				state.physicsNodeTransmitCollision.push_back("False");
			}

			state.collisionShapes.push_back(physicsNode->collisionShapeType);
			state.mass.push_back(std::to_string(physicsNode->getInverseMass()));

			if (physicsNode->getIsCollision())
			{
				state.isCollision.push_back("True");
			}
			else
			{
				state.isCollision.push_back("False");
			}

			state.elasticity.push_back(std::to_string(physicsNode->getElasticity()));

			state.friction.push_back(std::to_string(physicsNode->getFriction()));
			state.damping.push_back(std::to_string(physicsNode->getDamping()));

			if (physicsNode->getIsStatic())
			{
				state.isStatic.push_back("True");
			}
			else
			{
				state.isStatic.push_back("False");
			}
		}
		else
		{
			state.gameObjectPositions.push_back(std::array<std::string, 3>
			{
				std::to_string(gameObject->getSceneNode()->GetTransform().getPositionVector().x),
				std::to_string(gameObject->getSceneNode()->GetTransform().getPositionVector().y),
				std::to_string(gameObject->getSceneNode()->GetTransform().getPositionVector().z)
			});

			state.gameObjectRotations.push_back(std::array<std::string, 4>
			{
				std::to_string(gameObject->getSceneNode()->axisAngleRotation.x),
				std::to_string(gameObject->getSceneNode()->axisAngleRotation.y),
				std::to_string(gameObject->getSceneNode()->axisAngleRotation.z),
				std::to_string(gameObject->getSceneNode()->axisAngleRotation.w)
			});

			state.hasPhysicsNode.push_back(false);
		}
	}

	return state;
}
