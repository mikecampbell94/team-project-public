#include "GameLoop.h"
#include <iostream>

GameLoop::GameLoop(System& gameSystem)
{
	this->gameSystem = gameSystem;
	window = new Window("Game Window", 1280, 720);

	//MUST BE REMOVED
	camera = new Camera(0, 0, Vector3(0, 0, 0));

	renderer = new Renderer(window, camera, Vector2(1280, 720));

	SceneNode* node = new SceneNode("../Data/meshes/centeredcube.obj");
	node->SetTransform(Matrix4::translation(Vector3(0, -10, 0)) * Matrix4::scale(Vector3(10, 10, 10)));
	std::vector<SceneNode*>* nodes = new std::vector<SceneNode*>();
	nodes->push_back(node);
	scene = new SceneManager(nodes);

	renderer->initialise(scene);
	/////
}

GameLoop::~GameLoop()
{
	delete window;
}

void GameLoop::executeGameLoop()
{
	int frameCount = 0;

	while(window->updateWindow() && !window->getKeyboard()->keyDown(KEYBOARD_ESCAPE))
	{
		float deltaTime = loopTimer.getTimeSinceLastRetrieval();

		renderer->update(deltaTime);

		gameSystem.updateNextSystemFrame(deltaTime);

		std::cout << "Updated frame " << frameCount << ". Delta time = " << deltaTime << std::endl;
		++frameCount;


		pitch -= (window->getMouse()->getRelativePosition().y);
		yaw -= (window->getMouse()->getRelativePosition().x);

		if (window->getKeyboard()->keyDown(KEYBOARD_W)) {
			camera->SetPosition(camera->GetPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_S)) {
			camera->SetPosition(camera->GetPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 1) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_A)) {
			camera->SetPosition(camera->GetPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) *  Vector3(-1, 0, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_D)) {
			camera->SetPosition(camera->GetPosition() +
				Matrix4::rotation(yaw, Vector3(0, 1, 0)) *  Vector3(1, 0, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_SPACE)) {
			camera->SetPosition(camera->GetPosition() + Vector3(0, 1, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_C)) {
			camera->SetPosition(camera->GetPosition() + Vector3(0, -1, 0) * 1);
		}

		camera->SetPitch(pitch);
		camera->SetYaw(yaw);

		camera->BuildViewMatrix();
	}

}
