#include "Startup.h"
#include "Resource Management/Database/Database.h"
#include "Networking/NetworkClient.h"
#include "Profiler/FPSCounter.h"
#include "DevConsole\Console.h"
#include "DevConsole/LevelEditor.h"

#include "../Utilities/FilePaths.h"
#include "../Gameplay/Scripting/PaintGameActionBuilder.h"
#include "Resource Management/XMLWriter.h"

Startup::Startup(ThreadPool* threadpool)
{
	engine = new System(threadpool);
	game = new GameLoop(engine, nullptr, this);
	loopTimer = new GameTimer();
}

Startup::~Startup()
{
	
}

void Startup::renderLoadingScreen()
{
	rendering->renderLoadingScreen(15.0f);
}

void Startup::initialiseSubsystems()
{
	initialiseDatabaseAndTables();
	initialiseAudioSystem();
	physics = new PhysicsEngine(database, window->getKeyboard());
	userInterface = new UserInterface(window->getKeyboard(), resolution);
	initialiseLevelSystem();
	initialiseInputSystem();
	PaintGameActionBuilder::initialiseBuilders(database);
	initialiseGameplaySystem();
	network = new NetworkClient(keyboardAndMouse, database, inputManager->GetPlayerbase(), gameplay);
	addSystemsToEngine();

	game->addWindowToGameLoop(window);
	game->addCameraToGameLoop(camera);
	game->addGameTimerToGameLoop(loopTimer);
	LevelEditor::initialiseLevelEditor(database, gameplay);
}

void Startup::initialiseRenderingSystem()
{
	XMLParser windowConfiguration;
	windowConfiguration.loadXMLFile("../Data/Resources/Graphics Config/windowConfigXML.xml");
	Node* node = windowConfiguration.parsedXml;
	resolution.x = std::stof(node->children[0]->children[0]->value);
	resolution.y = std::stof(node->children[0]->children[1]->value);
	bool fullScreen = node->children[1]->value == "Enabled";

	window = new Window("Game Window", resolution.x, resolution.y, fullScreen);
	window->lockMouseToWindow(true);
	window->showOSPointer(false);

	camera = new Camera(0, 90, NCLVector3(0, 0, 0));

	rendering = new RenderingSystem(window, camera);
	
	nodes = new std::vector<SceneNode*>();
	scene = new SceneManager(camera, nodes);
	//rendering->initialise(database);
	//rendering->SetSceneToRender(scene);
}

void Startup::startUserInterface()
{
	userInterface->initialise(database);
}

void Startup::startRenderingSystem()
{
	rendering->initialise(database);
	rendering->SetSceneToRender(scene, database);
}

void Startup::setupMeshes()
{
	rendering->setupMeshes();
}

void Startup::initialiseAudioSystem()
{
	audio = new AudioSystem(database, camera);
}

void Startup::initialiseInputSystem()
{
	//rendering->initialise(database);
	//rendering->SetSceneToRender(scene, database);

	//---------------------------------
	//---------------------------------

	//node = new SceneNode("../Data/meshes/centeredcube.obj");
	//node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));

	//-------------------------------------------
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
	profiler = new Profiler(window->getKeyboard(), database, new FPSCounter());
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
	engine->addConcurrentSubsystem(gameplay);
	engine->addConcurrentSubsystem(inputManager);
	engine->addSubsystem(rendering);
	engine->addConcurrentSubsystem(audio);
	engine->addConcurrentSubsystem(userInterface);
	engine->addConcurrentSubsystem(physics);
	engine->addConcurrentSubsystem(profiler);
	engine->addConcurrentSubsystem(new Console(window->getKeyboard(), camera, window->getMouse()));

	for (Subsystem * subsystem : engine->getSubSystems())
	{
		profiler->addSubsystemTimer(subsystem->getSubsystemName(), subsystem->getTimer());
	}
}

void Startup::loadMainMenu()
{
	XMLParser::deleteAllParsedXML();
	level->loadLevelFile(LEVELDIR + "MainMenu.xml", gameplay);
	XMLParser::deleteAllParsedXML();
	//gameplay->compileGameplayScript("../Data/Gameplay/mainMenuScript.xml");
	//userInterface->initialise(database);
	//gameplay->setUnTimedLevel();
}

void Startup::loadLevel(std::string levelFile, bool online)
{
	XMLParser::deleteAllParsedXML();
	
	gameplay->setDefaultGameplayScript();
	gameplay->deleteGameObjectScripts();
	physics->InitialiseOctrees(8);
	level->loadLevelFile(LEVELDIR + levelFile, gameplay);

	if (!online)
	{
		playerbase->addNewPlayer(keyboardAndMouse, 0);
		gameplay->connectPlayerbase(inputManager->GetPlayerbase());
	}

	//gameplay->compileGameplayScript("../Data/Gameplay/gameplay.xml");
	gameplay->compileGameObjectScripts();

	//XMLWriter writer(database);
	//writer.saveLevelFile("myLevel");
	XMLParser::deleteAllParsedXML();
}

void Startup::switchLevel()
{
	XMLParser::deleteAllParsedXML();
	gameplay->setDefaultGameplayScript();
	gameplay->deleteGameObjectScripts();
	rendering->clearScores();
	rendering->clearPainters();
	audio->clearSoundNodesWhenUnloadingLevel();
	level->unloadLevelWhileKeepingUserInterface();
	XMLParser::deleteAllParsedXML();
}

void Startup::unloadLevel()
{
	XMLParser::deleteAllParsedXML();
	gameplay->setDefaultGameplayScript();
	gameplay->deleteGameObjectScripts();
	rendering->clearScores();
	rendering->clearPainters();
	audio->clearSoundNodesWhenUnloadingLevel();
	level->unloadLevel();
	XMLParser::deleteAllParsedXML();
}

void Startup::beginOnlineLobby()
{
	PaintGameActionBuilder::online = true;
	engine->addSubsystem(network);
	network->waitForOtherClients(2);
	network->connectToServer();
	DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "deltatime disable"));
}

void Startup::startGameLoop()
{
	game->executeGameLoop();
}



