#include "PipelineConfiguration.h"

#include "Graphics Settings/BasicGeometry.h"
#include "../Resource Management/Database/Database.h"

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

void PipelineConfiguration::initialiseModules(Matrix4 projmatrix, Matrix4 orthographicMatrix, Database* database)
{
	paintTrail = new PaintTrail("PaintTrail", projmatrix, resolution, database);
	paintTrail->linkShaders();
	paintTrail->initialise();

	gBuffer = new GBuffer("gbuffer", projmatrix, resolution, window, camera, sceneManager->getSceneNodesInFrustum());
	gBuffer->linkShaders();
	gBuffer->initialise();
	gBuffer->paintTextureMatrix = &paintTrail->textureMatrices;
	gBuffer->paintTrailTexture = &paintTrail->paintTrailTexture;

	skybox = new Skybox("Skybox", projmatrix, resolution, &camera->viewMatrix);
	skybox->linkShaders();
	skybox->initialise();
	skybox->GBufferFBO = &gBuffer->gBuffer;

	ssao = new SSAO("SSAO", projmatrix, resolution, camera, gBuffer->getGBuffer());
	ssao->linkShaders();
	ssao->initialise();

	shadowTex = new Shadows("Shadows", projmatrix, resolution, sceneManager->getAllLights(), sceneManager->getAllNodes());
	shadowTex->linkShaders();
	shadowTex->initialise();

	bpLighting = new BPLighting("BPLighting", projmatrix, resolution, camera, gBuffer->getGBuffer(), 
		sceneManager->getAllLights(), ssao->getSSAOTextures(), shadowTex->getShadowData());
	bpLighting->linkShaders();
	bpLighting->initialise();
	bpLighting->SSAOApplied = &ssao->applied;
	bpLighting->ShadowsApplied = &shadowTex->applied;

	uiModule = new UIModule("UIModule", orthographicMatrix, resolution, database);
	uiModule->linkShaders();
	uiModule->initialise();

	gameText = new GameText("GameText", orthographicMatrix, projmatrix, resolution, camera);
	gameText->linkShaders();
	gameText->initialise();

	scoreCounter = new ScoreCounter("ScoreCounter", projmatrix, resolution);
	scoreCounter->linkShaders();
	scoreCounter->initialise();
	scoreCounter->paintTrailTexture = &paintTrail->paintTrailTexture;
}

void PipelineConfiguration::buildPipeline(GraphicsPipeline* pipeline)
{
	pipeline->addModule(paintTrail);
	pipeline->addModule(gBuffer);
	pipeline->addModule(skybox);
	pipeline->addModule(shadowTex);
	pipeline->addModule(ssao);
	pipeline->addModule(bpLighting);
	pipeline->addModule(uiModule);
	pipeline->addModule(gameText);
	pipeline->addModule(scoreCounter);
}
