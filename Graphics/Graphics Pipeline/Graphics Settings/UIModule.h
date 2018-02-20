#pragma once

#include "../GraphicsModule.h"
#include "../../Meshes/Mesh.h"
#include "../Interface/UserInterfaceBuilder.h"

class Database;

class UIModule : public GraphicsModule
{
public:
	UIModule(const std::string identifier, const Matrix4 projMatrix, const Vector2 resolution,
		Database* database);
	~UIModule();

	void initialise() override;
	void apply() override;

	void linkShaders() override;
	void regenerateShaders() override;

protected:
	void locateUniforms() override;
	Database* database;
	Shader* UIShader;
	Matrix4 UIprojMatrix;
	std::vector<Button*> UIObjects;
};

