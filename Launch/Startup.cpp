#include "Startup.h"
#include "Resource Management/Database/Database.h"

Startup::Startup()
{
	engine = new System();
	game = new GameLoop(*engine);
}

Startup::~Startup()
{

}

void Startup::initialiseSubsystems()
{
	initialiseRenderingSystem();
	initialiseDatabase();
	initialiseAudioSystem();
	initialiseTablesInDatabase();
}

void Startup::initialiseRenderingSystem()
{
	window = new Window("Game Window", screenWidth, screenHeight);
	window->lockMouseToWindow(true);

	camera = new Camera(0, 0, Vector3(0, 0, 0));

	rendering = new RenderingSystem(window, camera, Vector2(screenWidth, screenHeight));
	scene = new SceneManager(camera, new std::vector<SceneNode*>());

	rendering->initialise(database);
	rendering->SetSceneToRender(scene);
}

void Startup::initialiseAudioSystem()
{
	audio = new AudioSystem(database, camera);
}

void Startup::initialiseInputSystem()
{
	SceneNode* node = new SceneNode("../Data/meshes/centeredcube.obj");
	node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));

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
}

void Startup::loadLevel(std::string levelFile)
{
	//USE A MAIN MENU LEVEL HERE WHICH WOULD JUST BE THE UI AND STUFF
	//THEN CAN JUST USE LEVEL.UNLOADLEVEL AND LOAD IN THE MAIN LEVEL FOR GAME
	level->loadLevelFile(levelFile);
}

void Startup::unloadLevel()
{
	level->unloadLevel();
}

void Startup::initialiseDatabase()
{
	database = new Database();
}

void Startup::initialiseTablesInDatabase()
{
	tableCreation = new TableCreation(database);
	level = new Level(database, scene);
}

void Startup::startGameLoop()
{
	game->executeGameLoop();
}



