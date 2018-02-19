#include "PipelineConfiguration.h"
#include "Graphics Settings/BasicGeometry.h"

PipelineConfiguration::PipelineConfiguration()
{
	this->sceneManager = nullptr;
	this->window = nullptr;
	this->resolution = Vector2();
	this->camera = nullptr;
}

PipelineConfiguration::PipelineConfiguration(SceneManager* sceneManager, Window* window, 
	Camera* camera, Vector2 resolution)
{
	this->sceneManager = sceneManager;
	this->window = window;
	this->resolution = resolution;
	this->camera = camera;
}

PipelineConfiguration::~PipelineConfiguration()
{
}

void PipelineConfiguration::initialiseModules(Matrix4 projmatrix)
{
	gBuffer = new GBuffer("gbuffer", projmatrix, resolution, window, camera, sceneManager->getSceneNodesInFrustum());
	gBuffer->linkShaders();
	gBuffer->initialise();

	skybox = new Skybox("Skybox", projmatrix, resolution, &camera->viewMatrix);
	skybox->linkShaders();
	skybox->initialise();
	skybox->GBufferFBO = &gBuffer->gBuffer;

	ssao = new SSAO("SSAO", projmatrix, resolution, camera, gBuffer->getGBuffer());
	ssao->linkShaders();
	ssao->initialise();

	bpLighting = new BPLighting("BPLighting", projmatrix, resolution, camera, gBuffer->getGBuffer(), 
		sceneManager->getAllLights(), ssao->getSSAOTextures());
	bpLighting->linkShaders();
	bpLighting->initialise();
	bpLighting->SSAOApplied = &ssao->applied;
}

void PipelineConfiguration::buildPipeline(GraphicsPipeline* pipeline)
{
	pipeline->addModule(gBuffer);
	pipeline->addModule(skybox);
	pipeline->addModule(ssao);
	pipeline->addModule(bpLighting);
}
