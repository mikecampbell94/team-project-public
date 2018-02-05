#include "PipelineConfiguration.h"
#include "Graphics Settings/BasicGeometry.h"

PipelineConfiguration::PipelineConfiguration(SceneManager* sceneManager, Window* window, 
	GraphicsPipeline* pipeline, Camera* camera, Vector2 resolution)
{
	this->sceneManager = sceneManager;
	this->window = window;
	this->resolution = resolution;
	this->camera = camera;
	this->pipeline = pipeline;
}


PipelineConfiguration::~PipelineConfiguration()
{
	delete ambTex;
	delete SGBuffer;
	delete ssao;
	delete lighting;
	delete bloom;
	delete motionBlur;
	delete skybox;
	delete particles;
}

void PipelineConfiguration::initialiseModules(Matrix4 projmatrix)
{
	basicGeom = new BasicGeometry("Basic Geometry Renderer", projmatrix, resolution, camera, sceneManager->getSubMeshesInFrustum());
	basicGeom->linkShaders();
}

void PipelineConfiguration::buildPipeline()
{
	pipeline->addModule(basicGeom);
}
