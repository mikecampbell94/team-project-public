#pragma once

#include "Scene Management/SceneManager.h"
#include "Rendering/RenderingSystem.h"
#include "../Input/InputManager.h"
#include "GameplaySystem.h"
#include "../../Audio/AudioSystem.h"
#include "../Launch/Game/GameLoop.h"
#include "Resource Management/Database/TableCreation.h"
#include "Resource Management/Level.h"

class Database;

class Startup
{

public:
	Startup();
	~Startup();

	void initialiseSubsystems();
	void startGameLoop();

private:
	void initialiseRenderingSystem();
	void initialiseAudioSystem();
	void initialiseInputSystem();
	void initialiseDatabase();
	void initialiseTablesInDatabase();

	void loadLevel();
	
	static void startGame();

	//make all these unique ptrs
	Database* database;
	TableCreation* tableCreation;
	SceneManager* scene;

	Level* level;

	RenderingSystem* rendering;
	InputManager* inputManager;
	GameplaySystem* gameplay;
	AudioSystem* audio;

	GameLoop* game;

	System* engine;
	GameTimer loopTimer;
	Window* window;

	Camera* camera;

	int screenWidth = 1280;
	int screenHeight = 720;
};

