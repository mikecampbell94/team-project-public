#pragma once

#include "../GraphicsModule.h"
#include "../../Shaders/ComputeShader.h"

struct PlayerScores
{
	float red;
};

class ScoreCounter : public GraphicsModule
{
public:
	ScoreCounter(const std::string identifier, const Matrix4 projmatrix,
		const Vector2 resolution);
	~ScoreCounter();

	void linkShaders() override;
	void regenerateShaders() override;

	void initialise() override;
	void apply() override;

	GLuint* paintTrailTexture;
private:
	void locateUniforms() override;

	ComputeShader* computeShader;
	GLuint playerScoresSSBO;
	GLuint redCounter;
	GLuint yellowCounter;
	PlayerScores scores;
};

