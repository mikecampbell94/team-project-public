#pragma once

#include "../GraphicsModule.h"

#define SHADOWSIZE 4096

#include "../../ASSIMP/Model.h"

#include <vector>

class Shadows : public GraphicsModule
{
public:
	Shadows(const std::string identifier, const Matrix4 projmatrix,
		const Vector2 resolution, int numShadowCastingLights, Light** lights, std::vector<Model*>** models);

	~Shadows()
	{
		delete shadowShader;
		delete shadowData;
		delete shadowFBOs;

		glDeleteTextures(shadowData->NUM_LIGHTS, &shadowData->shadows[0]);
	}

	void linkShaders() override;
	void initialise() override;
	void apply() override;

	void regenerateShaders() override;

	ShadowData* getShadowData() const
	{
		return shadowData;
	}

private:
	void locateUniforms() override
	{}

	//Shadow prep
	void initShadowTex();
	void initShadowBuffer();

	//Application
	void drawShadowScene();

	GLuint* shadowFBOs;
	ShadowData* shadowData;
	Shader* shadowShader;

	Light** lights;
	std::vector<Model*>** models;
};

