#include "Renderer.h"

#include "../../Input/Devices/Window.h"
#include "../GraphicsUtility.h"
#include "../Utility/Camera.h"
#include "../Utilities/Maths/Matrix4.h"

Renderer::Renderer() : OGLRenderer(0, Vector2())
{
	window = nullptr;
	camera = nullptr;
	resolution = Vector2();
}

Renderer::Renderer(Window* window, Camera* camera)
	: OGLRenderer(window->getHandle(), window->getScreenSize())
{
	this->window = window;
	this->camera = camera;
	this->resolution = window->getScreenSize();

	globalProjectionMatrix = Matrix4::perspective(1.0f, 150000.0f, resolution.x / resolution.y, 60.0f);

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	GraphicsUtility::CheckGLError("Renderer Initialisation");
}

Renderer::~Renderer()
{
}

void Renderer::initialise(SceneManager* sceneManager)
{
	graphicsConfig = PipelineConfiguration(sceneManager, window, camera, resolution);
	graphicsConfig.initialiseModules(globalProjectionMatrix);
	graphicsConfig.buildPipeline(&pipeline);

	XMLParser graphicsconfigParser;
	graphicsconfigParser.loadFile("../Data/Resources/Config/Graphics/graphicsConfigXML.xml");
	Node* node = graphicsconfigParser.parsedXml;

	//TO-DO
	//pipeline.intialisePipeline();

	for (int i = 0; i < node->children.size(); i++)
	{
		std::string enabled = node->children[i]->value;
		std::string graphicsModuleName = node->children[i]->name;

		if (enabled == "Enabled")
		{
			pipeline.toggleModule(graphicsModuleName, true);
		}
		else
		{
			pipeline.toggleModule(graphicsModuleName, false);
		}
	}

	this->sceneManager = sceneManager;
}

void Renderer::update(const float& deltatime)
{
	updateScene(deltatime);
	renderScene();
}

void Renderer::updateScene(const float& msec)
{
	camera->updateCamera();
	camera->buildViewMatrix();
	camera->updateViewFrustum(globalProjectionMatrix);

	sceneManager->clearMeshLists();
	sceneManager->buildMeshLists();
}

void Renderer::renderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	pipeline.executeModules();
	swapBuffers();
}
