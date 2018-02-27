#pragma once

#include "../Gameplay/GameObject.h"
#include "../Gameplay/Scripting/ActionBuilder.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/Scripting/GameLogic.h"

class Database;

class GameObjectLogic
{
public:
	GameObjectLogic(Database* database, MessageProcessor* messages);
	~GameObjectLogic();

	void compileParsedXMLIntoScript(Node* xmlNode);
	/*{
		GameLogic.compile[0]...
		MyClass.compile[1]
	}*/

	
	

private:
	GameLogic logic;
	GameObject* gameObject;
	Database* database;
};

