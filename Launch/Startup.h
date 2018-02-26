#pragma once

#include "Scene Management/SceneManager.h"
#include "Rendering/RenderingSystem.h"
#include "../Input/InputManager.h"
#include "GameplaySystem.h"
#include "../../Audio/AudioSystem.h"
#include "../Launch/Game/GameLoop.h"
#include "Resource Management/Database/TableCreation.h"
#include "Resource Management/Level.h"
#include "../../Input/Recorders/KeyboardMouseRecorder.h"

class NetworkClient;

class Database;

class Startup
{

public:
	Startup();
	~Startup();

	void initialiseSubsystems();
	void startGameLoop();

	void loadMainMenu();
	void loadLevel(std::string levelFile);
	void switchLevel();
	void unloadLevel();

private:
	void initialiseRenderingSystem();
	void initialiseAudioSystem();
	void initialiseInputSystem();
	void initialiseDatabaseAndTables();
	void initialiseLevelSystem();
	void initialiseGameplaySystem();

	void addSystemsToEngine();


	//make all these unique ptrs
	System* engine;
	GameLoop* game;

	Database* database;
	TableCreation* tableCreation;
	SceneManager* scene;

	Level* level;

	RenderingSystem* rendering;
	InputManager* inputManager;
	GameplaySystem* gameplay;
	AudioSystem* audio;
	PhysicsEngine* physics;
	UserInterface* userInterface;
	NetworkClient* network;

	GameTimer* loopTimer;
	Window* window;

	Camera* camera;

	InputRecorder* keyboardAndMouse;
	PlayerBase* playerbase;

	SceneNode* node;
	std::vector<SceneNode*>* nodes;

	int screenWidth = 1280;
	int screenHeight = 720;
};

