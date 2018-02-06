#include "RenderingSystem.h"

RenderingSystem::RenderingSystem(Window* window, Camera* camera, Vector2 resolution)
{
	renderer = std::make_unique<Renderer>(window, camera, resolution);
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::SetSceneToRender(SceneManager* scene)
{
	renderer->initialise(scene);
}

void RenderingSystem::updateSubsystem(const float& deltaTime)
{
	renderer->update(deltaTime);
}
