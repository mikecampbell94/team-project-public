#include "GameLoop.h"
#include "../../Input/InputManager.h"


#include "../../Resource Management/XMLParser.h"
#include "../../Resource Management/Level.h"
#include "../../Input/Recorders/KeyboardMouseRecorder.h"
#include <iostream>
#include "Communication/LetterBox.h"
#include "../../Gameplay/GameObject.h"
#include "../../Input/Recorders/KeyboardMouseRecorder.h"

GameLoop::GameLoop(System& gameSystem)
{
	engine = gameSystem;
	//needs changing once Startup is done
	XMLParser windowconfigParser;
	windowconfigParser.loadFile("../Data/Resources/Config/Graphics/windowConfigXML.xml");
	Node* parsednode = windowconfigParser.parsedXml;
	bool fullscreenIsEnabled;
	std::string enabled = parsednode->children[1]->value;

	if (enabled == "Enabled")
	{
		fullscreenIsEnabled = true;
	}
	else
	{
		fullscreenIsEnabled = false;
	}
	window = new Window("overwatch haha lol :)", stoi(parsednode->children[0]->children[0]->value), stoi(parsednode->children[0]->children[1]->value), fullscreenIsEnabled);
	//window = new Window("Game Window", 1280, 720);
	window->lockMouseToWindow(true);

	//MUST BE REMOVED
	camera = new Camera(0, 0, Vector3(0, 0, 0));

	//change so only window and camera are passed in and resolution is retreived from window class
	rendering = new RenderingSystem(window, camera);
	//Database database;
	database = new Database();

	physics = new PhysicsEngine();

	//AUDIO MUST BE CREATED BEFORE TABLE CREATION AND AFTER DATABASE CREATION!!!!!!!!!!!!
	audio = new AudioSystem(database, camera);

	TableCreation tableCreation(database);

	std::vector<SceneNode*>* nodes = new std::vector<SceneNode*>();
	scene = new SceneManager(camera, nodes);
	Level level(database, scene, physics);
	level.loadLevelFile("TestLevel.txt");

	rendering->initialise(database);

	SceneNode* node = new SceneNode("../Data/meshes/centeredcube.obj");
	node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));
	//nodes->push_back(node);

	rendering->SetSceneToRender(scene);

	InputRecorder* keyboardAndMouse = new KeyboardMouseRecorder(window->getKeyboard(), window->getMouse());

	PlayerBase* playerbase = new PlayerBase();
	playerbase->addNewPlayer(keyboardAndMouse);
	playerbase->getPlayers()[0]->setSceneNode(node);

	std::string seperator = "|";
	std::string keyboard = "KEYBOARD_W|KEYBOARD_A|KEYBOARD_S|KEYBOARD_D";
	std::string xbox = "XBOX_A|XBOX_B";
	std::vector<int> kmTestConfig = playerbase->getPlayers()[0]->getInputFilter()->getListenedKeys(keyboard, seperator);

	playerbase->getPlayers()[0]->getInputRecorder()->addKeysToListen(kmTestConfig);

	inputManager = new InputManager(playerbase);
	gameplay = new GameplaySystem(playerbase->getPlayers().size(),*playerbase);

	engine.addSubsystem(gameplay);
	engine.addSubsystem(inputManager);
	engine.addSubsystem(rendering);
	engine.addSubsystem(audio);

	engine.addSubsystem(physics);
}

GameLoop::~GameLoop()
{
	delete window;
	delete rendering;
	delete inputManager;
	delete physics;
}

void GameLoop::executeGameLoop()
{
	int frameCount = 0;

	while(window->updateWindow() && !window->getKeyboard()->keyDown(KEYBOARD_ESCAPE))
	{
		float deltaTime = loopTimer.getTimeSinceLastRetrieval();

		engine.updateNextSystemFrame(deltaTime);

		auto gameObjectResources = database->getTable("GameObjects")->getAllResources()->getResourceBuffer();
		for (auto gameObjectIterator = gameObjectResources.begin(); gameObjectIterator != gameObjectResources.end(); gameObjectIterator++)
		{
			GameObject* gObj = static_cast<GameObject*>((*gameObjectIterator).second);
			if (gObj->getPhysicsNode() != nullptr)
			{
				gObj->updatePosition();
			}
				
		}

		DeliverySystem::getPostman()->deliverAllMessages();
		engine.processMessagesForAllSubsystems();

		DeliverySystem::getPostman()->clearAllMessages();

		pitch -= (window->getMouse()->getRelativePosition().y);
		yaw -= (window->getMouse()->getRelativePosition().x);

		if (window->getKeyboard()->keyDown(KEYBOARD_W)) {
			camera->setPosition(camera->getPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_S)) {
			camera->setPosition(camera->getPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 1) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_A)) {
			camera->setPosition(camera->getPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) *  Vector3(-1, 0, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_D)) {
			camera->setPosition(camera->getPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) *  Vector3(1, 0, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_SPACE)) {
			camera->setPosition(camera->getPosition() + Vector3(0, 1, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_C)) {
			camera->setPosition(camera->getPosition() + Vector3(0, -1, 0) * 1);
		}

		camera->setPitch(pitch);
		camera->setYaw(yaw);
	}

}
