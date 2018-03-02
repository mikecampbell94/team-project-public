#include "GameLoop.h"
#include "../../Input/InputManager.h"


#include "../../Resource Management/XMLParser.h"
#include "../../Resource Management/Level.h"
#include "../../Input/Recorders/KeyboardMouseRecorder.h"
#include <iostream>
#include "Communication/LetterBox.h"
#include "../../Gameplay/GameObject.h"
#include "../../Input/Recorders/KeyboardMouseRecorder.h"
#include "Communication/Messages/PlaySoundMessage.h"
#include "../Startup.h"
#include <iterator>

GameLoop::GameLoop(System* gameSystem, Database* database, Startup* startup)
{
	engine = gameSystem;
	this->database = database;
	//window = new Window("Game Window", 1280, 720);
	//window->lockMouseToWindow(true);


	////MUST BE REMOVED
	//camera = new Camera(0, 0, Vector3(0, 0, 0));

	//rendering = new RenderingSystem(window, camera, Vector2(1280, 720));

	//std::vector<SceneNode*>* nodes = new std::vector<SceneNode*>();
	//scene = new SceneManager(camera, nodes);

	//database = new Database();
	//TableCreation tableCreation(database);


	//audio = new AudioSystem(database, camera);

	//Level level(database, scene);
	//level.loadLevelFile("TestLevel.txt");

	//rendering->initialise(database);

	//SceneNode* node = new SceneNode("../Data/meshes/centeredcube.obj");
	//node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));
	////nodes->push_back(node);

	//rendering->SetSceneToRender(scene);

	//InputRecorder* keyboardAndMouse = new KeyboardMouseRecorder(window->getKeyboard(), window->getMouse());

	//PlayerBase* playerbase = new PlayerBase();
	//playerbase->addNewPlayer(keyboardAndMouse);
	//playerbase->getPlayers()[0]->setSceneNode(node);

	//std::string seperator = "|";
	//std::string keyboard = "KEYBOARD_W|KEYBOARD_A|KEYBOARD_S|KEYBOARD_D";
	//std::string xbox = "XBOX_A|XBOX_B";
	//std::vector<int> kmTestConfig = playerbase->getPlayers()[0]->getInputFilter()->getListenedKeys(keyboard, seperator);

	//playerbase->getPlayers()[0]->getInputRecorder()->addKeysToListen(kmTestConfig);

	//inputManager = new InputManager(playerbase);
	//gameplay = new GameplaySystem(playerbase->getPlayers().size());

	//engine.addSubsystem(gameplay);
	//engine.addSubsystem(inputManager);
	//engine.addSubsystem(rendering);
	//engine.addSubsystem(userInterface);

	/*nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getResource("playerBall"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getResource("wall1"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getResource("wall2"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getResource("wall3"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getResource("wall4"))->getSceneNode());
	nodes->push_back(static_cast<GameObject*>(database.getTable("GameObjects")->getResource("floor"))->getSceneNode());*/
	//engine.addSubsystem(audio);

	DeliverySystem::getPostman()->addDeliveryPoint("GameLoop");
	incomingMessages = MessageProcessor(std::vector<MessageType>{ MessageType::TEXT},
		DeliverySystem::getPostman()->getDeliveryPoint("GameLoop"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::TEXT, [startup = startup, &quit = quit,
		&deltaTimeMultiplier = deltaTimeMultiplier](Message* message)
	{
		TextMessage* textMessage = static_cast<TextMessage*>(message);

		istringstream iss(textMessage->text);
		vector<string> tokens{ istream_iterator<string>{iss},
			std::istream_iterator<string>{} };

		if (tokens[0] == "Quit")
		{
			quit = true;
		}
		else if (tokens[0] == "Start")
		{
			startup->switchLevel();

			if (tokens[1] == "True")
			{
				startup->beginOnlineLobby();
				startup->loadLevel(tokens[2], true);
			}
			else
			{
				startup->loadLevel(tokens[2], false);
			}
		}
		else if (tokens[0] == "deltatime")
		{
			if (tokens[1] == "enable")
			{
				deltaTimeMultiplier = 1.0f;
			}
			else if (tokens[1] == "disable")
			{
				deltaTimeMultiplier = 0.0f;
			}
		}
	});
}

GameLoop::~GameLoop()
{
}

void GameLoop::executeGameLoop()
{
	DeliverySystem::getPostman()->insertMessage(PlaySoundMessage("AudioSystem", PLAY_SOUND, camera->getPosition(), 
		SOUNDPRIORITY_HIGH, 1.0f, 10000.0f, 1.0f, false, false, "mirrorsedge", "BackgroundMusic"));

	camera->setPitch(24.0f);
	camera->setYaw(-133.0f);

	while (window->updateWindow() && !quit)
	{
		float deltaTime = loopTimer->getTimeSinceLastRetrieval() * deltaTimeMultiplier;

		engine->updateNextSystemFrame(deltaTime);
		updateGameObjects();

		DeliverySystem::getPostman()->deliverAllMessages();

		engine->processMessagesForAllSubsystems();
		incomingMessages.processMessagesInBuffer();

		DeliverySystem::getPostman()->clearAllMessages();

		//pitch -= (window->getMouse()->getRelativePosition().y);
		//yaw -= (window->getMouse()->getRelativePosition().x);

		//if (window->getKeyboard()->keyDown(KEYBOARD_W)) {
		//	camera->setPosition(camera->getPosition() +
		//		Matrix4::rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * 1);
		//}

		//if (window->getKeyboard()->keyDown(KEYBOARD_S)) {
		//	camera->setPosition(camera->getPosition() +
		//		Matrix4::rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 1) * 1);
		//}

		//if (window->getKeyboard()->keyDown(KEYBOARD_A)) {
		//	camera->setPosition(camera->getPosition() +
		//		Matrix4::rotation(yaw, Vector3(0, 1, 0)) *  Vector3(-1, 0, 0) * 1);
		//}

		//if (window->getKeyboard()->keyDown(KEYBOARD_D)) {
		//	camera->setPosition(camera->getPosition() +
		//		Matrix4::rotation(yaw, Vector3(0, 1, 0)) *  Vector3(1, 0, 0) * 1);
		//}

		//if (window->getKeyboard()->keyDown(KEYBOARD_SPACE)) {
		//	camera->setPosition(camera->getPosition() + Vector3(0, 1, 0) * 1);
		//}

		//if (window->getKeyboard()->keyDown(KEYBOARD_C)) {
		//	camera->setPosition(camera->getPosition() + Vector3(0, -1, 0) * 1);
		//}

		//camera->setPitch(pitch);
		//camera->setYaw(yaw);
	}

}

void GameLoop::updateGameObjects()
{
	auto gameObjectResources = database->getTable("GameObjects")->getAllResources()->getResourceBuffer();
	for (auto gameObjectIterator = gameObjectResources.begin(); gameObjectIterator != gameObjectResources.end(); ++gameObjectIterator)
	{
		GameObject* gObj = static_cast<GameObject*>((*gameObjectIterator).second);
		if (gObj->getPhysicsNode() != nullptr)
		{
			gObj->updatePosition();
		}

		//DeliverySystem::getPostman()->insertMessage(TextMeshMessage("RenderingSystem", "thing",
		//	gObj->getSceneNode()->GetWorldTransform().getPositionVector(), Vector3(10, 10, 1), false));
	}
}
