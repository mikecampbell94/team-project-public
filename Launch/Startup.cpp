#include "Startup.h"
#include "Resource Management/Database/Database.h"

Startup::Startup()
{
	engine = new System();
	game = new GameLoop(engine, nullptr);
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
	physics = new PhysicsEngine();
	initialiseLevelSystem();
	initialiseInputSystem();
	initialiseGameplaySystem();
	addSystemsToEngine();

	game->addWindowToGameLoop(window);
	game->addCameraToGameLoop(camera);
	game->addGameTimerToGameLoop(loopTimer);
}

void Startup::initialiseRenderingSystem()
{
	window = new Window("Game Window", screenWidth, screenHeight);
	window->lockMouseToWindow(true);

	camera = new Camera(0, 0, Vector3(0, 0, 0));

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

	node = new SceneNode("../Data/meshes/centeredcube.obj");
	node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));

	//-------------------------------------------
	rendering->SetSceneToRender(scene);
	//-------------------------------------------

	keyboardAndMouse = new KeyboardMouseRecorder(window->getKeyboard(), window->getMouse());

	playerbase = new PlayerBase();
	playerbase->addNewPlayer(keyboardAndMouse);
	playerbase->getPlayers()[0]->setSceneNode(node);

	std::string seperator = "|";
	std::string keyboard = "KEYBOARD_W|KEYBOARD_A|KEYBOARD_S|KEYBOARD_D";
	std::string xbox = "XBOX_A|XBOX_B";
	std::vector<int> kmTestConfig = playerbase->getPlayers()[0]->getInputFilter()->getListenedKeys(keyboard, seperator);

	playerbase->getPlayers()[0]->getInputRecorder()->addKeysToListen(kmTestConfig);

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
	level = new Level(database, scene, physics);
}

void Startup::initialiseGameplaySystem()
{
	gameplay = new GameplaySystem(inputManager->GetPlayerbase()->getPlayers().size(), *inputManager->GetPlayerbase());
}

void Startup::addSystemsToEngine()
{
	engine->addSubsystem(gameplay);
	engine->addSubsystem(inputManager);
	engine->addSubsystem(rendering);
	engine->addSubsystem(audio);
	engine->addSubsystem(physics);
}

void Startup::loadLevel(std::string levelFile)
{
	level->loadLevelFile(levelFile);
	physics->InitialiseOctrees(3);
	gameplay->compileGameplayScript("../Resources/Gameplay/gameplay.xml");
}

void Startup::unloadLevel()
{
	level->unloadLevel();
}

void Startup::startGameLoop()
{
	game->executeGameLoop();
}



