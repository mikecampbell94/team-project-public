#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "../Utilities/Maths/Vector4.h"
#include "Renderer.h"
#include <memory>

class Database;

class RenderingSystem : public Subsystem
{
public:
	RenderingSystem(Database* database, Window* window, Camera* camera, Vector2 resolution);
	~RenderingSystem();

	void loadingScreen() {}

	void SetSceneToRender(SceneManager* scene);
	void updateSubsystem(const float& deltaTime) override;

	void removeAsset() {}
	void addAsset() {}
	void changeColor(Vector4 c) {}

private:
	std::unique_ptr<Renderer> renderer;

};

