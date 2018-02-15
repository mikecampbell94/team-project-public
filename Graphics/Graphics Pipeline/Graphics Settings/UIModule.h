#pragma once
#include "../GraphicsModule.h"
#include "../../Meshes/Mesh.h"


struct UIObject 
{
	Vector4 colour;
	Vector2 position;
	Vector2 scale;
	std::string action;
	Mesh* UiMesh;
};


class UIModule : public GraphicsModule
{
public:
	UIModule(const std::string identifier, const Matrix4 projMatrix, const Vector2 resolution);
	~UIModule();

	void initialise() override;
	void apply() override;

	void linkShaders() override;
	void regenerateShaders() override;



protected:
	void locateUniforms() override;

	Matrix4 UIprojMatrix;
	std::vector<UIObject*> UIObjects;
};

