#include "Renderer.h"

#include "../../Input/Devices/Window.h"
#include "../GraphicsUtility.h"
#include "../Utility/Camera.h"

Renderer::Renderer(Window* window, Camera* camera, Vector2 resolution)
	: OGLRenderer(window->getHandle(), resolution)
{
	this->window = window;
	this->camera = camera;
	this->resolution = resolution;

	pipeline = new GraphicsPipeline();


	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	GraphicsUtility::CheckGLError("Renderer Initialisation");
}

Renderer::~Renderer()
{
	delete pipeline;
	delete graphicsConfig;
}

void Renderer::initialise(SceneManager* sceneManager)
{
	graphicsConfig = new PipelineConfiguration(sceneManager, window, pipeline, camera, resolution);
	graphicsConfig->initialiseModules(Matrix4::perspective(1.0f, 150000.0f, resolution.x / resolution.y, 60.0f));
	graphicsConfig->buildPipeline();

	this->sceneManager = sceneManager;
}

void Renderer::update(const float& deltatime)
{
	updateScene(deltatime);
	renderScene();
}

void Renderer::updateScene(const float& msec)
{
	sceneManager->ClearMeshLists();
	sceneManager->BuildMeshLists();
}

void Renderer::renderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	pipeline->executeModules();
	swapBuffers();
}
