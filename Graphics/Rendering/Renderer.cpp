#include "Renderer.h"

#include "../../Input/Devices/Window.h"
#include "../GraphicsUtility.h"
#include "../Utility/Camera.h"
#include "../Utilities/Maths/Matrix4.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/GameObject.h"

Renderer::Renderer() : OGLRenderer(0, NCLVector2())
{
	window = nullptr;
	camera = nullptr;
	resolution = NCLVector2();
	pipeline = GraphicsPipeline(nullptr);
}

Renderer::Renderer(GameTimer* parentTimer, Window* window, Camera* camera)
	: OGLRenderer(window->getHandle(), window->getScreenSize())
{
	this->window = window;
	this->camera = camera;
	this->resolution = window->getScreenSize();
	this->parentTimer = parentTimer;

	parentTimer->addChildTimer("Update Scene Management");
	parentTimer->addChildTimer("Render Modules");
	//parentTimer->addChildTimer("Render");

	//parentTimer->getChildTimer("Render Modules")->addChildTimer("hELLO");

	pipeline = GraphicsPipeline(parentTimer->getChildTimer("Render Modules"));

	//globalProjectionMatrix = Matrix4::perspective(1.0f, 150000.0f, resolution.x / resolution.y, 60.0f);
	globalOrthographicMatrix = NCLMatrix4::orthographic(-1.0f,10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	GraphicsUtility::CheckGLError("Renderer Initialisation");
}

Renderer::~Renderer()
{
}

void Renderer::initialise(SceneManager* sceneManager, Database* database)
{
	graphicsConfig = PipelineConfiguration(sceneManager, window, camera, resolution);
	graphicsConfig.initialiseModules(database);
	graphicsConfig.buildPipeline(&pipeline);

	XMLParser graphicsconfigParser;
	graphicsconfigParser.loadFile("../Data/Resources/Graphics Config/graphicsConfigXML.xml");
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

void Renderer::changeResolution(NCLVector2 resolution)
{
	//Resize(resolution.x, resolution.y);

	//for each (GraphicsModule* module in pipeline.)
}

void Renderer::addSceneNode(SceneNode* sceneNode)
{
	(*sceneManager->getAllNodes())->push_back(sceneNode);
}

void Renderer::removeSceneNodeByResourceName(std::string resourcename)
{
	for (auto sceneNodeIterator = (*sceneManager->getAllNodes())->begin(); sceneNodeIterator != (*sceneManager->getAllNodes())->end(); ++ sceneNodeIterator)
	{
		if ((*sceneNodeIterator)->getParent()->getName() == resourcename)
		{
			(*sceneManager->getAllNodes())->erase(sceneNodeIterator);
			break;
		}
	}
}

void Renderer::toggleModule(const std::string& moduleName, bool enabled)
{
	pipeline.toggleModule(moduleName, enabled);
}

GraphicsModule* Renderer::getGraphicsModule(const std::string& moduleName)
{
	return pipeline.getGraphicsModule(moduleName);
}

void Renderer::updateScene(const float& msec)
{
	parentTimer->beginChildTimedSection("Update Scene Management");

	camera->updateCamera();
	camera->buildViewMatrix();
	camera->updateViewFrustum(CommonGraphicsData::SHARED_PROJECTION_MATRIX);
	pipeline.updateModules(msec);
	sceneManager->clearMeshLists();
	sceneManager->buildMeshLists();

	parentTimer->endChildTimedSection("Update Scene Management");
}

void Renderer::renderScene()
{
	parentTimer->beginChildTimedSection("Render Modules");

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	pipeline.executeModules();
	swapBuffers();

	parentTimer->endChildTimedSection("Render Modules");
}
