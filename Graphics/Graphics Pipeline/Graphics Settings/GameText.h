#pragma once

#include "../GraphicsModule.h"
#include "../../Meshes/Mesh.h"
#include "../../Meshes/TextMesh.h"
#include "../../Text.h"

class Camera;

class GameText : public GraphicsModule
{
public:
	GameText(const std::string identifier, const Vector2 resolution, Camera* camera);
	~GameText();

	void initialise() override;
	void apply() override;

	void linkShaders() override;
	void regenerateShaders() override;

	void bufferText(std::string text, Vector3 position, Vector3 scale, Vector3 colour, bool orthographic, bool hasBackground);

protected:
	void locateUniforms() override;
	Shader* UIShader;
	Shader* UITextShader;
	Shader* textWithBackgrounShader;

	Camera* camera;

	Font* font;
	GLint tex;
	std::vector<std::string> bufferedText;
	std::vector<Vector3> bufferedPositions;
	std::vector<Vector3> bufferedScales;
	std::vector<Vector3> bufferedColours;
	std::vector<bool> bufferedOrthographicUsage;

	std::vector<std::string> bufferedBackgroundText;
	std::vector<Vector3> bufferedBackgroundPositions;
	std::vector<Vector3> bufferedBackgroundScales;
	std::vector<Vector3> bufferedBackgroundColours;
	std::vector<bool> bufferedOBackgroundrthographicUsage;
};

