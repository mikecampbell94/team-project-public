#include "GameObjectLogic.h"
#include "Devices/Keyboard.h"
#include "../../Communication/DeliverySystem.h"

void changeResource(Node** node, std::string id)
{
	if ((*node)->value == "var")
	{
		(*node)->value = id;
	}
	for each (Node* child in (*node)->children)
	{
		changeResource(&child, id);
	}
}

void changeResourceBack(Node** node, std::string id)
{
	if ((*node)->value == id)
	{
		(*node)->value = "var";
	}
	for each (Node* child in (*node)->children)
	{
		changeResourceBack(&child, id);
	}
}

GameObjectLogic::GameObjectLogic(Database* database, MessageProcessor* messages, std::string script)
{
	this->database = database;
	this->messages = messages;

	XMLParser parser;
	parser.loadFile(script);
	parsedScript = parser.parsedXml;
}

GameObjectLogic::~GameObjectLogic()
{
}

void GameObjectLogic::compileParsedXMLIntoScript()
{
	Node* resources = parsedScript->children[0];
	Node* gameLogicNode = parsedScript->children[1];
	

	for each (Node* resource in resources->children)
	{
		GameObject* gObj = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(resource->value));

		changeResource(&gameLogicNode, resource->value);
		
		logicToGameObjects.insert({ gObj, GameLogic(messages) });
		logics.push_back(&(logicToGameObjects.at(gObj)));

		logics[logics.size() - 1]->compileParsedXMLIntoScript(gameLogicNode);

		changeResourceBack(&gameLogicNode, resource->value);








		Node* hardCodedLogic = parsedScript->children[2];

		//changeResource(&hardCodedLogic, resource->value);
		compileFunctions(hardCodedLogic);
		//changeResourceBack(&hardCodedLogic, resource->value);
		
	}

	for each (GameLogic* logic in logics)
	{
		logic->executeActionsOnStart();
	}

	for each (auto func in fucntionsOnStart)
	{
		func.second();
	}
}

void GameObjectLogic::compileFunctions(Node* node)
{
	for each (Node* subNode in node->children)
	{
		compileFunctionsOnStart(subNode);
	}


	if (node->nodeType == "ReceiveMessage")
	{
		for each (Node* action in node->children)
		{
			if (node->value == "PlayerInputMessage")
			{
				functions.push_back([inputMessages = inputMessages, action = action]()
				{
					if (action->children[0]->value == "0x20")
					{
						for each (auto message in inputMessages)
						{
							if (message->data.key == KEYBOARD_SPACE)
							{
								GameObject* gObj = message->player->getGameObject();

								if (gObj->canJump)
								{
									gObj->canJump = false;
								}
							}
						}
					}
				});
			}
			/*else if (node->value == "CollisionMessage")
			{
				functions.push_back([action = action, logicToGameObjects = logicToGameObjects]()
				{
					for each (auto gObj in logicToGameObjects)
					{
						if (!gObj.first->canJump)
						{
							gObj.first->canJump = true;
						}
					}
				});
			}*/
		}
	}


	//if (node->nodeType == "Function")
	//{
	//	if (node->value == "")
	//	{
	//		functions.insert({ node->name, [logicToGameObjects = logicToGameObjects, database = database]()
	//		{
	//			for each (auto object in logicToGameObjects)
	//			{
	//				//object.first->
	//			}
	//		} });
	//	}
	//}
}

void GameObjectLogic::compileFunctionsOnStart(Node* node)
{
	for each (Node* action in node->children)
	{
		//PUT THIS INTO A BUILDER CLASS FOR FUNCTIONS LIKE THE ACTION BUILDER FOR GAME LOGIC
		////////////////////////////////////////////////////////////////////
		if (action->nodeType == "SetJump")
		{
			fucntionsOnStart.insert({ action->nodeType, [logicToGameObjects = logicToGameObjects, action, database = database]()
			{
				GameObject* gObj = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(action->children[0]->value));

				if (action->children[1]->value == "true")
				{
					logicToGameObjects.find(gObj)->first->canJump = true;
				}
				else
				{
					logicToGameObjects.find(gObj)->first->canJump = false;
				}
			} });
		}
		///////////////////////////////////////////////////////////////////
	}
}



void GameObjectLogic::notify(const std::string& messageType, Message* message)
{
	if (messageType == "CollisionMessage")
	{
		for each (GameLogic* logic in logics)
		{
			logic->notifyMessageActions(messageType, message);
		}
	}
	else if (messageType == "InputMessage")
	{
		PlayerInputMessage* input = static_cast<PlayerInputMessage*>(message);

		inputMessages.push_back(input);
	}
}

void GameObjectLogic::updatelogic(const float& deltaTime)
{
	for each (GameLogic* logic in logics)
	{
		logic->executeMessageBasedActions();
		logic->executeTimeBasedActions(deltaTime);
		logic->clearNotifications();
	}

	//	//update hard coded stuff here
	updateHardCodedLogic(deltaTime);

}

void GameObjectLogic::updateHardCodedLogic(const float& deltaTime)
{
	//updateInputMessageLogic();

	for each (auto func in functions)
	{
		func();
	} 
}

void GameObjectLogic::updateInputMessageLogic()
{
	for each (auto message in inputMessages)
	{
		if(message->data.key == KEYBOARD_SPACE)
		{
			GameObject* gObj = message->player->getGameObject();

			if(gObj->canJump)
			{
				gObj->canJump = false;
			}
		}
	}
}

