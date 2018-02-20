#pragma once

#include "OGLRenderer.h"
#include "../Graphics Pipeline/GraphicsPipeline.h"
#include "../Graphics Pipeline/PipelineConfiguration.h"
#include "../Scene Management/SceneManager.h"
#include "../../Launch/Systems/Subsystem.h"

class Window;
class Camera;
class Matrix4;
class Database;

class Renderer : OGLRenderer
{
public:
	Renderer();
	Renderer(Window* window, Camera* camera);
	~Renderer();

	void initialise(SceneManager* sceneManager, Database* database);
	void update(const float& deltatime);
	void toggleModule(const std::string& moduleName, bool enabled);

private:
	void updateScene(const float& msec) override;
	void renderScene() override;

	GraphicsPipeline pipeline;
	PipelineConfiguration graphicsConfig;

	SceneManager* sceneManager; 
	Window* window; 
	Camera* camera; 
	Vector2 resolution;
	Matrix4 globalProjectionMatrix;
	Matrix4 globalOrthographicMatrix;
};

