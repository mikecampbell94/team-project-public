#pragma once

#include "../GraphicsModule.h"

class Camera;

class MotionBlur : public GraphicsModule
{
public:
	MotionBlur(const std::string identifier, const NCLMatrix4 projmatrix,
		const NCLVector2 resolution, GBufferData* gBuffer, Camera* camera);
	~MotionBlur();

	void linkShaders() override;
	void regenerateShaders() override;

	void initialise() override;
	void apply() override;

	GLuint screenTexFBO;

private:
	void createTexture();
	void locateUniforms() override {}
	Camera* camera;
	Shader* blurShader;

	GLuint colourBuffer[1];
	GLuint attachment[1] = { GL_COLOR_ATTACHMENT0 };
	GLuint renderBuffer;

	GBufferData* gBuffer;

	NCLMatrix4 previousView;
	NCLMatrix4 currentView;
	float* fps;
};

