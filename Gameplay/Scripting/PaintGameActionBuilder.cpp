#include "PaintGameActionBuilder.h"

#include "../../Resource Management/XMLParser.h"
#include "../../Resource Management/Database/Database.h"
#include "../../Communication/DeliverySystem.h"
#include "../../Communication/Messages/PaintTrailForGameObjectMessage.h"
#include "../../Communication/SendMessageActionBuilder.h"
#include "../../Communication/Messages/ToggleGameObjectMessage.h"
#include "../GameObject.h"
#include "../../Physics/PhysicsNode.h"

#include <iostream>
#include <random>

std::unordered_map<std::string, Builder>PaintGameActionBuilder::builders
	= std::unordered_map<std::string, Builder>();
std::string PaintGameActionBuilder::powerUpBuilders[2] = { "ScalePlayer", "DecreaseMass" };
Database* PaintGameActionBuilder::database = nullptr;
std::string PaintGameActionBuilder::localPlayer = "";
bool PaintGameActionBuilder::online = false;

void PaintGameActionBuilder::initialiseBuilders(Database* database)
{
	PaintGameActionBuilder::database = database;
	std::random_device rd;     // only used once to initialise (seed) engine

	builders.insert({"MoveMinions", [](Node* node)
	{
		Executable sendMessageAction = SendMessageActionBuilder::buildSendMessageAction(node->children[0]);
		return [sendMessageAction]()
		{
			if (PaintGameActionBuilder::localPlayer == "player0"
				|| !PaintGameActionBuilder::online)
			{
				sendMessageAction();
			}
		};
	} });

	builders.insert({ "TransmitMinion", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		
		return [gameObject]()
		{
			
				DeliverySystem::getPostman()->insertMessage(TextMessage("NetworkClient", "insertMinion " + gameObject->getName()));

		};
	} });

	builders.insert({ "PrintText", [](Node* node)
	{
		std::string text = node->children[0]->value;

		return [text]()
		{
			std::cout << text << std::endl;
		};
	} });

	builders.insert({ "Jump", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		float impulse = stof(node->children[1]->value);

		return [gameObject, impulse]()
		{
			if (gameObject->stats.canJump)
			{
				gameObject->getPhysicsNode()->applyImpulse(NCLVector3(0.f, impulse, 0.f));
				gameObject->stats.canJump = false;
			}

		};
	} });

	builders.insert({ "LetJump", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		
		return [gameObject]()
		{
			gameObject->stats.canJump = true;
		};
	} });



	builders.insert({ "CheckPaint", [](Node* node)
	{
		Executable sendMessageAction = SendMessageActionBuilder::buildSendMessageAction(node->children[1]);
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));

		return [sendMessageAction, gameObject]()
		{
			if (gameObject->stats.currentPaint > 0)
			{
				sendMessageAction();
			}
				
		};
	} });

	builders.insert({ "SetMaxPaint", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		int maxPaint = stoi(node->children[1]->value);

		return [gameObject, maxPaint]()
		{
			gameObject->stats.maxPaint = maxPaint;
			gameObject->stats.currentPaint = maxPaint;

		};
	} });

	builders.insert({ "CreateMeteorPool", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		int amount = stoi(node->children[1]->value);
		std::string meshName = node->children[2]->value;
		float size = stof(node->children[3]->value);
		std::string baseName = node->children[0]->value + "Meteor";

		return [gameObject, amount, meshName, size, baseName]()
		{
			Database* database = PaintGameActionBuilder::database;

			gameObject->stats.meteors = amount;

			for (int i = 0; i < amount; i++)
			{
				GameObject* meteor = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(baseName + std::to_string(i)));
				if (meteor == nullptr)
				{
					SceneNode* sceneNode = new SceneNode(static_cast<Mesh*>(database->getTable("Meshes")->getResource(meshName)));
					sceneNode->SetColour(gameObject->stats.colourToPaint);

					meteor = new GameObject();
					meteor->setSize(sizeof(GameObject));
					meteor->setName(baseName + std::to_string(i));
					meteor->setSceneNode(sceneNode);
					meteor->stats.colourToPaint = gameObject->stats.colourToPaint;
					meteor->setScale(NCLVector3(size, size, size));

					PhysicsNode* physicsNode = new PhysicsNode();
					physicsNode->setParent(meteor);
					physicsNode->transmitCollision = true;
					physicsNode->setCollisionShape("Sphere");
					physicsNode->setInverseMass(0.2f);
					physicsNode->setInverseInertia(physicsNode->getCollisionShape()->buildInverseInertia(physicsNode->getInverseMass()));
					physicsNode->setStatic(false);
					meteor->setPhysicsNode(physicsNode);

					meteor->setPosition(gameObject->getPosition() + NCLVector3(i * 10, 50, 0));
					meteor->setRotation(NCLVector4(0.f, 0.f, 0.f, 0.f));
					
					meteor->setEnabled(false);

					PaintGameActionBuilder::database->getTable("GameObjects")->addNewResource(meteor);
					DeliverySystem::getPostman()->insertMessage(TextMessage("RenderingSystem", "addscenenode " + meteor->getName()));
					DeliverySystem::getPostman()->insertMessage(TextMessage("Physics", "addphysicsnode " + meteor->getName()));
					DeliverySystem::getPostman()->insertMessage(TextMessage("RenderingSystem", "setupmesh " + meteor->getName()));

					//DeliverySystem::getPostman()->insertMessage(ToggleGameObjectMessage("RenderingSystem", meteor->getName(), false));
					//DeliverySystem::getPostman()->insertMessage(ToggleGameObjectMessage("Physics", meteor->getName(), false));
				}
			}
			
			
		};
	} });

	builders.insert({ "ReducePaint", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));

		return [gameObject]()
		{

			if (gameObject->getPhysicsNode()->getLinearVelocity().length() > 0.1f)
			{
				int paint = max(--gameObject->stats.currentPaint, 0);

				float massDecrease = 1 - ((float)paint / (float)gameObject->stats.maxPaint);

				if (paint < 25)
				{
					float interpolationFactor = ((float)paint / (float)gameObject->stats.maxPaint) * 4;
					NCLVector3 interpolatedColour = NCLVector3::interpolate(NCLVector3(1.f, 1.f, 1.f), gameObject->stats.colourToPaint.toVector3(), interpolationFactor);
					gameObject->getSceneNode()->SetColour(NCLVector4(interpolatedColour.x, interpolatedColour.y, interpolatedColour.z, 1.f));
				}

				gameObject->getPhysicsNode()->setInverseMass((gameObject->stats.defaultInvMass + massDecrease));

				gameObject->stats.currentPaint = paint;
			}
		};
	} });


	builders.insert({ "RegainPaint", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));

		return [gameObject]()
		{
			gameObject->getPhysicsNode()->setInverseMass(gameObject->stats.defaultInvMass);
			gameObject->stats.currentPaint = gameObject->stats.maxPaint;
			gameObject->getSceneNode()->SetColour(gameObject->stats.colourToPaint);
		};
	} });

	builders.insert({ "PaintMinion", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		GameObject* minion = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[1]->value));

		return [gameObject, minion]()
		{

			if (gameObject->stats.currentPaint > 0)
			{
				minion->getSceneNode()->SetColour(gameObject->stats.colourToPaint);
				minion->stats.colourToPaint = gameObject->stats.colourToPaint;

				if (PaintGameActionBuilder::localPlayer == gameObject->getName()
					&& PaintGameActionBuilder::online)
				{
					DeliverySystem::getPostman()->insertMessage(TextMessage("NetworkClient", "paintMinion " + minion->getName()));
				}
			}
		};
	} });

	builders.insert({ "ScalePlayer", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		float multiplier = stof(node->children[1]->value);
		GameObject* powerup = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[2]->value));
		float duration = stof(node->children[3]->value) * 1000;

		return [gameObject, multiplier, powerup, duration]()
		{
			if (!gameObject->stats.executeAfter)
			{
				gameObject->setPosition(NCLVector3(gameObject->getPosition().x, gameObject->getPosition().y + (gameObject->stats.defaultScale.y * multiplier * .5f), gameObject->getPosition().z));
				gameObject->setScale(gameObject->stats.defaultScale * multiplier);
				powerup->setEnabled(false);


				gameObject->stats.timeToWait = duration;
				gameObject->stats.executeAfter = [gameObject, powerup]()
				{
					gameObject->setScale(gameObject->stats.defaultScale);
					gameObject->stats.executeAfter = std::function<void()>();
					powerup->setEnabled(true);
				};
			}
		};
	} });

	builders.insert({ "DecreaseMass", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		float multiplier = stof(node->children[1]->value); 
		GameObject* powerup = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[2]->value));
		float duration = stof(node->children[3]->value) * 1000;

		return [gameObject, multiplier, powerup, duration]()
		{
			if (!gameObject->stats.executeAfter)
			{
				gameObject->stats.defaultInvMass *= multiplier;
				powerup->setEnabled(false);

				gameObject->stats.timeToWait = duration;
				gameObject->stats.executeAfter = [gameObject, powerup]()
				{
					gameObject->stats.defaultInvMass = 1.f;
					gameObject->stats.executeAfter = std::function<void()>();
					powerup->setEnabled(true);

				};
			}
		};
	} });

	builders.insert({ "MeteorStrike", [&rd = rd](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		GameObject* powerup = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[2]->value));
		float duration = stof(node->children[3]->value) * 1000;

		return [gameObject, powerup, duration, &rd = rd]()
		{
			if (!gameObject->stats.executeAfter)
			{
				powerup->setEnabled(false);

				for (int i = 0; i < gameObject->stats.meteors; ++i)
				{
					GameObject* meteor = static_cast<GameObject*>(
						PaintGameActionBuilder::database->getTable("GameObjects")->getResource(gameObject->getName() + "Meteor" + std::to_string(i)));
					
					std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
					std::uniform_int_distribution<int> uni(-6, 6); // guaranteed unbiased
					auto random_integer1 = uni(rng);
					auto random_integer2 = uni(rng);


					//DeliverySystem::getPostman()->insertMessage(MoveGameObjectMessage("Physics", meteor->getName(), gameObject->getPosition() + NCLVector3(random_integer1 * 10, 100 + (i * 40), random_integer2 * 10)));
					meteor->setPosition(gameObject->getPosition() + NCLVector3(random_integer1 * 10, 100 + (i * 40), random_integer2 * 10));
					meteor->setEnabled(true);
					//DeliverySystem::getPostman()->insertMessage(ToggleGameObjectMessage("RenderingSystem", meteor->getName(), true));
					//DeliverySystem::getPostman()->insertMessage(ToggleGameObjectMessage("Physics", meteor->getName(), true));
				}

				gameObject->stats.timeToWait = duration;
				gameObject->stats.executeAfter = [gameObject, powerup]()
				{
					gameObject->stats.executeAfter = std::function<void()>();
					powerup->setEnabled(true);

				};
			}
		};
	} });


	builders.insert({ "RandomPowerUp", [&rd = rd](Node* node)
	{
		
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		float multiplier = stof(node->children[1]->value);
		GameObject* powerup = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[2]->value));
		float duration = stof(node->children[3]->value) * 1000;

		return [&rd = rd, gameObject, multiplier, powerup, duration]()
		{
			if (!gameObject->stats.executeAfter)
			{
				powerup->setEnabled(false);
				gameObject->stats.timeToWait = duration;

				std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
				std::uniform_int_distribution<int> uni(0, 2); // guaranteed unbiased
				auto random_integer = uni(rng);

				switch (random_integer)
				{
					case SCALE_POWERUP:
					{
						gameObject->setPosition(NCLVector3(gameObject->getPosition().x, gameObject->getPosition().y + (gameObject->stats.defaultScale.y * multiplier * .5f), gameObject->getPosition().z));
						gameObject->setScale(gameObject->stats.defaultScale * multiplier);
						break;
					}
					case SPEED_POWERUP:
					{
						gameObject->stats.defaultInvMass *= multiplier;
						break;
					}
					case METEOR_POWERUP:
					{
						for (int i = 0; i < gameObject->stats.meteors; ++i)
						{
							GameObject* meteor = static_cast<GameObject*>(
								PaintGameActionBuilder::database->getTable("GameObjects")->getResource(gameObject->getName() + "Meteor" + std::to_string(i)));

							std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
							std::uniform_int_distribution<int> uni(-6, 6); // guaranteed unbiased
							auto random_integer1 = uni(rng);
							auto random_integer2 = uni(rng);


							//DeliverySystem::getPostman()->insertMessage(MoveGameObjectMessage("Physics", meteor->getName(), gameObject->getPosition() + NCLVector3(random_integer1 * 10, 100 + (i * 40), random_integer2 * 10)));
							meteor->setPosition(gameObject->getPosition() + NCLVector3(random_integer1 * 10, 100 + (i * 40), random_integer2 * 10));
							meteor->setEnabled(true);

						}
						break;
					}
					default:
						break;
				}

				
				gameObject->stats.executeAfter = [gameObject, powerup]()
				{
					gameObject->stats.defaultInvMass = 1.f;
					gameObject->setScale(gameObject->stats.defaultScale);
					gameObject->stats.executeAfter = std::function<void()>();
					powerup->setEnabled(true);
				};
			}
		};
	} });






}

Executable PaintGameActionBuilder::buildExecutable(Node* node)
{
	return builders.at(node->nodeType)(node);
}
