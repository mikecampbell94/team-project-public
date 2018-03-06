#include "LevelEditor.h"

#include "GameObject.h"
#include "Resource Management/Database/Database.h"

#include <iterator>
#include <sstream>
#include "Communication/DeliverySystem.h"
#include "Communication/SendMessageActionBuilder.h"
#include "../Physics/PhysicsNode.h"

std::map<std::string, LevelEditorAction> LevelEditor::actions
	= std::map<std::string, LevelEditorAction>();
Database* LevelEditor::database = nullptr;

void LevelEditor::initialiseLevelEditor(Database* providedDatabase)
{
	database = providedDatabase;

	actions.insert({ "spawngameobject", [&database = database](std::vector<std::string> devConsoleTokens)
	{
		GameObject* gameObject = new GameObject();
		gameObject->setSize(sizeof(GameObject));
		gameObject->setName(devConsoleTokens[1]);
		
		database->getTable("GameObjects")->addNewResource(gameObject);
	} });

	actions.insert({ "addscenenode", [&database = database](std::vector<std::string> devConsoleTokens)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			database->getTable("GameObjects")->getResource(devConsoleTokens[1]));

		std::string meshName = devConsoleTokens[2];
		SceneNode* sceneNode = new SceneNode(static_cast<Mesh*>(database->getTable("Meshes")->getResource(meshName)));

		std::string colourString = devConsoleTokens[3].substr(7);
		NCLVector4 colour = NCLVector4::builder(colourString);
		sceneNode->SetColour(colour);

		gameObject->setSceneNode(sceneNode);

		DeliverySystem::getPostman()->insertMessage(TextMessage("RenderingSystem", "addscenenode " + devConsoleTokens[1]));
	} });

	//addphysicsnode obj shape=Box invMass=0.5 static=true
	actions.insert({ "addphysicsnode", [&database = database](std::vector<std::string> devConsoleTokens)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			database->getTable("GameObjects")->getResource(devConsoleTokens[1]));

		PhysicsNode* physicsnode = new PhysicsNode();
		physicsnode->setParent(gameObject);
		physicsnode->setEnabled(true);
		physicsnode->setIsCollision(true);
		physicsnode->transmitCollision = true;
		physicsnode->setCollisionShape(devConsoleTokens[2].substr(6));
		physicsnode->setInverseMass(stof(devConsoleTokens[3].substr(8)));
		physicsnode->setInverseInertia(physicsnode->getCollisionShape()->buildInverseInertia(physicsnode->getInverseMass()));
		physicsnode->setElasticity(0.5f);
		physicsnode->setFriction(0.9f);
		physicsnode->setStatic(devConsoleTokens[4].substr(7) == "true");
		gameObject->setPhysicsNode(physicsnode);

		DeliverySystem::getPostman()->insertMessage(TextMessage("Physics", "addphysicsnode " + devConsoleTokens[1]));
	} });

	//loadmesh mesh=.../.../data/mesh texture=../../texture
	actions.insert({ "loadmesh", [&database = database](std::vector<std::string> devConsoleTokens)
	{
		Mesh* mesh = new Mesh(devConsoleTokens[1].substr(5), 1);
		
		if (devConsoleTokens.size() == 3)
		{
			mesh->loadTexture(devConsoleTokens[2].substr(8));
		}

		database->getTable("Meshes")->addNewResource(mesh);
	} });

	actions.insert({ "debugcamera", [](std::vector<std::string> devConsoleTokens)
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("Console", "togglecamera"));
		DeliverySystem::getPostman()->insertMessage(TextMessage("RenderingSystem", "togglecamera"));
	} });

	actions.insert({ "removescenenode", [](std::vector<std::string> devConsoleTokens)
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("RenderingSystem", "removescenenode " + devConsoleTokens[1]));
	} });

	actions.insert({ "removephysicsnode", [](std::vector<std::string> devConsoleTokens)
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("Physics", "removephysicsnode " + devConsoleTokens[1]));
	} });
}

void LevelEditor::executeDevConsoleLine(std::string devConsoleLine)
{
	std::istringstream iss(devConsoleLine);
	std::vector<std::string> tokens{
		std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>{} };

	bool executedDevConsoleLine = false;

	for (auto actionIterator = actions.begin(); actionIterator != actions.end(); ++actionIterator)
	{
		if (actionIterator->first == tokens[0])
		{
			actions.at(tokens[0])(tokens);
			executedDevConsoleLine = true;
			break;
		}
	}

	if (!executedDevConsoleLine)
	{
		SendMessageActionBuilder::buildSendMessageAction(devConsoleLine)();
	}
}
