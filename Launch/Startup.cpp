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

	std::vector<SceneNode*>* nodes = new std::vector<SceneNode*>();
	scene = new SceneManager(camera, nodes);

	loadLevel();

	rendering->initialise(database);

	SceneNode* node = new SceneNode("../Data/meshes/centeredcube.obj");
	node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));
	//nodes->push_back(node);

	rendering->SetSceneToRender(scene);
}

void Startup::initialiseAudioSystem()
{
	audio = new AudioSystem(database, camera);
}

void Startup::initialiseInputSystem()
{
	
}

void Startup::loadLevel()
{
	Level level(database, scene);
	level.loadLevelFile("TestLevel.txt");
}


void Startup::initialiseDatabase()
{
	database = new Database();
}

void Startup::initialiseTablesInDatabase()
{
	tableCreation = new TableCreation(database);
}

void Startup::startGameLoop()
{
	game->executeGameLoop();
}



