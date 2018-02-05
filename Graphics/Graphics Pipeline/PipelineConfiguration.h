#pragma once

#include "Graphics Settings/SSAO.h"
#include "Graphics Settings/Shadows.h"
#include "Graphics Settings/BPLighting.h"
#include "Graphics Settings/GBuffer.h"
#include "Graphics Settings/Bloom.h"
#include "Graphics Settings/MotionBlur.h"
#include "../Utilities/Maths/Vector2.h"
#include "Effects/Skybox.h"
#include "Effects/ParticleSystem.h"

#include "GraphicsPipeline.h"
#include "../Scene Management/SceneManager.h"

#include <vector>
#include "Graphics Settings/BasicGeometry.h"

class Camera;
class Window;

class PipelineConfiguration
{
public:
	PipelineConfiguration(SceneManager* sceneManager, Window* window, 
		GraphicsPipeline* pipeline, Camera* camera, Vector2 resolution);
	~PipelineConfiguration();

	void initialiseModules(Matrix4 projmatrix);
	void buildPipeline();

private:
	Vector2	resolution;
	Camera*	camera;
	Window* window;
	GraphicsPipeline* pipeline;
	SceneManager* sceneManager;

	//Settings
	Shadows* shadows;
	GBuffer* SGBuffer;
	SSAO* ssao;
	BPLighting* lighting;
	Bloom* bloom;
	AmbientTextures* ambTex;
	MotionBlur* motionBlur;
	Skybox* skybox;
	ParticleSystem* particles;

	BasicGeometry* basicGeom;
};

