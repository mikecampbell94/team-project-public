#pragma once

#include "../GraphicsModule.h"
#include "../../Meshes/Mesh.h"
#include "../../Meshes/TextMesh.h"
#include "../../Text.h"

class Camera;

class GameText : public GraphicsModule
{
public:
	GameText(const std::string identifier, const Matrix4 orthographicMatrix, Matrix4 perspectiveMatrix,
		const Vector2 resolution, Camera* camera);
	~GameText();

	void initialise() override;
	void apply() override;

	void linkShaders() override;
	void regenerateShaders() override;

	void bufferText(std::string text, Vector3 position, Vector3 scale, bool orthographic);

protected:
	void locateUniforms() override;
	Shader* UIShader;
	Shader* UITextShader;

	Matrix4 orthographicMatrix;
	Matrix4 perspectiveMatrix;

	Camera* camera;

	Font* font;
	GLint tex;
	std::vector<std::string> bufferedText;
	std::vector<Vector3> bufferedPositions;
	std::vector<Vector3> bufferedScales;
	std::vector<bool> bufferedOrthographicUsage;
};

