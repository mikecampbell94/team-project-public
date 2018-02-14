#include "GameLoop.h"
#include "../../Input/InputManager.h"
#include "../../Input/GamePadRecorder.h"
#include "../../Input/KeyboardMouseRecorder.h"
#include "../../Resource Management/XMLParser.h"
#include <iostream>
#include "Communication/LetterBox.h"

/////////////////////////
#include "../../Audio/Sound.h"
#include "../../Audio/SoundSource.h"
////////////////////////////
////////////////////////////////////////
void GameLoop::updateListener()
{
	Matrix4 worldMat = camera->viewMatrix;
	Vector3 worldPos = camera->getPosition();

	Vector3 dirup[2];
	//forward
	dirup[0].x = -worldMat.values[2];
	dirup[0].y = -worldMat.values[6];
	dirup[0].z = -worldMat.values[10];
	//Up
	dirup[1].x = worldMat.values[1];
	dirup[1].y = worldMat.values[5];
	dirup[1].z = worldMat.values[9];

	ALfloat listenerPos[] = { worldPos.x, worldPos.y, worldPos.z };
	ALfloat listenerOri[] = { dirup[0].x, dirup[0].y, dirup[0].z, dirup[1].x, dirup[1].y, dirup[1].z };

	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_ORIENTATION, listenerOri);
}
//////////////////////////////////////////////

GameLoop::GameLoop(System& gameSystem)
{
	engine = gameSystem;
	window = new Window("Game Window", 1280, 720);
	window->lockMouseToWindow(true);

	//MUST BE REMOVED
	camera = new Camera(0, 0, Vector3(0, 0, 0));

	rendering = new RenderingSystem(window, camera, Vector2(1280, 720));

	SceneNode* node = new SceneNode("../Data/meshes/centeredcube.obj");
	node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));
	std::vector<SceneNode*>* nodes = new std::vector<SceneNode*>();
	nodes->push_back(node);
	scene = new SceneManager(camera, nodes);

	rendering->SetSceneToRender(scene);

	InputRecorder* keyboardAndMouse = new KeyboardMouseRecorder(window->getKeyboard(), window->getMouse());

	PlayerBase* playerbase = new PlayerBase();
	playerbase->addNewPlayer(keyboardAndMouse);

	std::string seperator = "|";
	std::string keyboard = "KEYBOARD_W|KEYBOARD_A";
	std::string xbox = "XBOX_A|XBOX_B";
	std::vector<int> kmTestConfig = playerbase->getPlayers()[0]->getInputFilter()->getListenedKeys(keyboard, seperator);

	playerbase->getPlayers()[0]->getInputRecorder()->addKeysToListen(kmTestConfig);

	inputManager = new InputManager(playerbase);
	gameplay = new GameplaySystem();

	engine.addSubsystem(gameplay);
	engine.addSubsystem(inputManager);
	engine.addSubsystem(rendering);
	/////


	//////////////////////////////////////
	ALCdevice *device = alcOpenDevice(NULL);
	ALCcontext *context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

	ALuint source;
	alGenSources(1, &source);
	OALSource *oal = new OALSource(source);

	Sound *sound = new Sound("../Data/SoundFiles/bounce.wav");

	ss = new SoundSource();
	ss->setSound(sound);
	ss->setPosition(Vector3(0.0f, 0.0f, 0.0f));
	ss->attachSource(oal);
	ss->setLooping(true);
	///////////////////////////////////////




	

	






	//-------XML TESTING-------//
	/*XMLParser testParser;
	for (int i = 0; i < 100000; ++i)
	{
		testParser.loadFile("../Resources/Game Objects/gameObject.xml");

		testParser.deleteAllNodes(testParser.parsedXml);
	}*/
	

	int i = 0;
}

GameLoop::~GameLoop()
{
	delete window;
	delete rendering;
	delete inputManager;
}

void GameLoop::executeGameLoop()
{
	int frameCount = 0;

	while(window->updateWindow() && !window->getKeyboard()->keyDown(KEYBOARD_ESCAPE))
	{
		float deltaTime = loopTimer.getTimeSinceLastRetrieval();

		engine.updateNextSystemFrame(deltaTime);

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

		updateListener();

		//////////////////
		ss->update(deltaTime);
		//////////////////
	}

}
