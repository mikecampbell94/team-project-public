#include "PipelineConfiguration.h"
#include "Graphics Settings/BasicGeometry.h"

PipelineConfiguration::PipelineConfiguration()
{
	this->sceneManager = nullptr;
	this->window = nullptr;
	this->resolution = Vector2();
	this->camera = nullptr;
	this->pipeline = nullptr;
}

PipelineConfiguration::PipelineConfiguration(SceneManager* sceneManager, Window* window, 
	Camera* camera, Vector2 resolution)
{
	this->sceneManager = sceneManager;
	this->window = window;
	this->resolution = resolution;
	this->camera = camera;
	this->pipeline = pipeline;
}

PipelineConfiguration::~PipelineConfiguration()
{
}

void PipelineConfiguration::initialiseModules(Matrix4 projmatrix)
{
	//basicGeom = new BasicGeometry("Basic Geometry Renderer", projmatrix, resolution, camera, sceneManager->getSceneNodesInFrustum());
	//basicGeom->linkShaders();
	gBuffer = new GBuffer("gbuffer", projmatrix, resolution,window,camera, sceneManager->getSceneNodesInFrustum());
	gBuffer->linkShaders();
	gBuffer->initialise();

	vector<LightData> lights;
	lights.push_back(Light(Vector3(-20, 10, -20), Vector4(1, 1, 1, 1), 400, 1).GetData());
	lights.push_back(Light(Vector3(20, 10, 20), Vector4(1, 1, 1, 1), 400, 1).GetData());

	bpLighting = new BPLighting("BPLighting", projmatrix, resolution, camera, gBuffer->getGBuffer(), lights);
	bpLighting->linkShaders();
	bpLighting->initialise();
}

void PipelineConfiguration::buildPipeline(GraphicsPipeline* pipeline)
{
	pipeline->addModule(gBuffer);
	pipeline->addModule(bpLighting);
	//pipeline->addModule(basicGeom);
}
