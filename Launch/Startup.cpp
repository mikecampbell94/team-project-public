#include "Startup.h"
#include "Resource Management/Database/Database.h"
#include "Networking/NetworkClient.h"

Startup::Startup()
{
	engine = new System();
	game = new GameLoop(engine, nullptr, this);
	loopTimer = new GameTimer();
}

Startup::~Startup()
{

}

void Startup::initialiseSubsystems()
{
	initialiseRenderingSystem();
	initialiseDatabaseAndTables();
	initialiseAudioSystem();
	physics = new PhysicsEngine(database);
	userInterface = new UserInterface(window->getKeyboard(), resolution);
	initialiseLevelSystem();
	initialiseInputSystem();
	initialiseGameplaySystem();
	network = new NetworkClient(keyboardAndMouse, database, inputManager->GetPlayerbase(), gameplay);
	addSystemsToEngine();

	game->addWindowToGameLoop(window);
	game->addCameraToGameLoop(camera);
	game->addGameTimerToGameLoop(loopTimer);
}

void Startup::initialiseRenderingSystem()
{
	XMLParser windowConfiguration;
	windowConfiguration.loadFile("../Data/Resources/Config/Graphics/windowConfigXML.xml");
	Node* node = windowConfiguration.parsedXml;
	resolution.x = std::stof(node->children[0]->children[0]->value);
	resolution.y = std::stof(node->children[0]->children[1]->value);
	bool fullScreen = node->children[1]->value == "Enabled";

	window = new Window("Game Window", resolution.x, resolution.y, fullScreen);
	window->lockMouseToWindow(true);
	window->showOSPointer(false);

	camera = new Camera(0, 90, Vector3(0, 0, 0));

	rendering = new RenderingSystem(window, camera);
	
	nodes = new std::vector<SceneNode*>();
	scene = new SceneManager(camera, nodes);

	//rendering->initialise(database);
	//rendering->SetSceneToRender(scene);
}

void Startup::initialiseAudioSystem()
{
	audio = new AudioSystem(database, camera);
}

void Startup::initialiseInputSystem()
{
	//---------------------------------
	rendering->initialise(database);
	//---------------------------------

	//node = new SceneNode("../Data/meshes/centeredcube.obj");
	//node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));

	//-------------------------------------------
	rendering->SetSceneToRender(scene, database);
	//-------------------------------------------

	keyboardAndMouse = new KeyboardMouseRecorder(window->getKeyboard(), window->getMouse());

	playerbase = new PlayerBase(database);
	//playerbase->getPlayers()[0]->setSceneNode(node);

	//std::string seperator = "|";
	//std::string keyboard = "KEYBOARD_W|KEYBOARD_A|KEYBOARD_S|KEYBOARD_D";
	//std::string xbox = "XBOX_A|XBOX_B";
	//std::vector<int> kmTestConfig = playerbase->getPlayers()[0]->getInputFilter()->getListenedKeys(keyboard, seperator);

	//playerbase->getPlayers()[0]->getInputRecorder()->addKeysToListen(kmTestConfig);

	inputManager = new InputManager(playerbase);
}

void Startup::initialiseDatabaseAndTables()
{
	database = new Database();
	tableCreation = new TableCreation(database);
	game->database = database;
}

void Startup::initialiseLevelSystem()
{
	level = new Level(database, scene, physics, userInterface);
}

void Startup::initialiseGameplaySystem()
{
	gameplay = new GameplaySystem(database);
}

void Startup::addSystemsToEngine()
{
	engine->addSubsystem(gameplay);
	engine->addSubsystem(inputManager);
	engine->addSubsystem(rendering);
	engine->addSubsystem(audio);
	engine->addSubsystem(userInterface);
	engine->addSubsystem(physics);
}

void Startup::loadMainMenu()
{
	level->loadLevelFile("MainMenu.txt", gameplay);
	//gameplay->compileGameplayScript("../Data/Gameplay/mainMenuScript.xml");
	//userInterface->initialise(database);
}

void Startup::loadLevel(std::string levelFile, bool online)
{
	physics->InitialiseOctrees(10);
	level->loadLevelFile(levelFile, gameplay);

	if (!online)
	{
		playerbase->addNewPlayer(keyboardAndMouse, 0);
		gameplay->connectPlayerbase(inputManager->GetPlayerbase());
	}

	//gameplay->compileGameplayScript("../Data/Gameplay/gameplay.xml");
	gameplay->compileGameObjectScripts();
}

void Startup::switchLevel()
{
	level->unloadLevelWhileKeepingUserInterface();
}

void Startup::unloadLevel()
{
	level->unloadLevel();
}

void Startup::beginOnlineLobby()
{
	engine->addSubsystem(network);
	network->waitForOtherClients(3);
	network->connectToServer();
	DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "deltatime disable"));
}

void Startup::startGameLoop()
{
	game->executeGameLoop();
}



