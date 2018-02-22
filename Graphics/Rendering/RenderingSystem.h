#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "../Utilities/Maths/Vector4.h"
#include "Renderer.h"
#include <memory>
#include "../Resource Management/XMLParser.h"
#include "../Graphics Pipeline/GraphicsPipeline.h"

class Database;

class RenderingSystem : public Subsystem
{
public:
	RenderingSystem(Window* window, Camera* camera);
	~RenderingSystem();

	void initialise(Database* database);

	void SetSceneToRender(SceneManager* scene, Database* database);
	void updateSubsystem(const float& deltaTime) override;
	void synchronise();

private:
	std::unique_ptr<Renderer> renderer;
	std::map<std::string, bool> graphicsConfig;
	GraphicsPipeline pipeline;

};