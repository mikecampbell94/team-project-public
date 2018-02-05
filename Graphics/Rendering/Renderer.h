#pragma once

#include "OGLRenderer.h"
#include "../Graphics Pipeline/GraphicsPipeline.h"
#include "../Graphics Pipeline/PipelineConfiguration.h"
#include "../Scene Management/SceneManager.h"
#include "../../Launch/Systems/Subsystem.h"

class Window;
class Camera;

class Renderer : OGLRenderer, Subsystem
{
public:
	Renderer(Window* window, Camera* camera, Vector2 resolution);
	~Renderer();

	void initialise(SceneManager* sceneManager);

	void updateSubsystem(const float& deltaTime) override {}
	void update(const float& deltatime);
	void updateScene(const float& msec) override;
	void renderScene() override;

private:
	GraphicsPipeline* pipeline;
	PipelineConfiguration* graphicsConfig;

	SceneManager* sceneManager; 
	Window* window; 
	Camera* camera; 
	Vector2 resolution;
};

