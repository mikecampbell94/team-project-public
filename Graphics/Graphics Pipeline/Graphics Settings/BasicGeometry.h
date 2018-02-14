#pragma once

#include "../GraphicsModule.h"
#include "../../Meshes/Mesh.h"
#include "../../Scene Management/SceneNode.h"

class Camera;

class BasicGeometry : public GraphicsModule
{
public:
	BasicGeometry(const std::string identifier, const Matrix4 projmatrix,
		const Vector2 resolution, Camera* camera, std::vector<SceneNode*>* nodesInFrame);
	~BasicGeometry();

	void linkShaders() override;
	void regenerateShaders() override;

	void initialise() override;
	void apply() override;

private:
	std::vector<SceneNode*>* nodesInFrame;


	void locateUniforms() override;
	Shader* basicShader;

	Camera* camera;
};

