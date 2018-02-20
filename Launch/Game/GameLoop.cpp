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

GameLoop::GameLoop(System* gameSystem)
{
	engine = gameSystem;
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
	//engine.addSubsystem(audio);
}

GameLoop::~GameLoop()
{
}

void GameLoop::executeGameLoop()
{
	while(window->updateWindow() && !window->getKeyboard()->keyDown(KEYBOARD_ESCAPE))
	{
		float deltaTime = loopTimer->getTimeSinceLastRetrieval();

		DeliverySystem::getPostman()->insertMessage(PlaySoundMessage("AudioSystem", PLAY_SOUND, Vector3(0.0f, 0.0f, 0.0f), SOUNDPRIORITY_HIGH, 1.0f, 10000.0f, 1.0f, false, false, "mirrorsedge", "BackgroundMusic"));

		engine->updateNextSystemFrame(deltaTime);

		DeliverySystem::getPostman()->deliverAllMessages();
		engine->processMessagesForAllSubsystems();

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
