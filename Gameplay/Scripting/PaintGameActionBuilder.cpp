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

void PaintGameActionBuilder::initialiseBuilders(Database* database)
{
	PaintGameActionBuilder::database = database;
	std::random_device rd;     // only used once to initialise (seed) engine

	builders.insert({ "PrintText", [](Node* node)
	{
		std::string text = node->children[0]->value;

		return [text]()
		{
			std::cout << text << std::endl;
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
				std::uniform_int_distribution<int> uni(0, 1); // guaranteed unbiased
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
