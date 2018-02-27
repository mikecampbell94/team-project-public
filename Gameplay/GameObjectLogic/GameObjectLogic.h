#pragma once

#include "../Gameplay/GameObject.h"
#include "../Gameplay/Scripting/ActionBuilder.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/Scripting/GameLogic.h"

class Database;

class GameObjectLogic
{
public:
	GameObjectLogic(Database* database);
	~GameObjectLogic();

	void compileParsedXMLIntoScript(Node* xmlNode);
	/*{
		GameLogic.compile[0]...
		MyClass.compile[1]
	}*/

	
	

private:
	GameObject* gameObject;
	Database* database;
	//GameLogic logic;
};

