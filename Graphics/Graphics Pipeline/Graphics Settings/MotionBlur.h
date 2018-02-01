#pragma once

#include "../GraphicsModule.h"

class MotionBlur : public GraphicsModule
{
public:
	MotionBlur(const std::string identifier, const Matrix4 projmatrix,
		const Vector2 resolution, GBufferData* gBuffer,
		Matrix4* previousView, Matrix4* currentView, float* fps);
	~MotionBlur();

	void linkShaders() override;
	void regenerateShaders() override;

	void initialise() override;
	void apply() override;

	GLuint screenTexFBO;

private:
	void createTexture();
	void locateUniforms() override {}

	Shader* blurShader;

	GLuint colourBuffer[1];
	GLuint attachment[1] = { GL_COLOR_ATTACHMENT0 };
	GLuint renderBuffer;

	GBufferData* gBuffer;

	Matrix4* previousView;
	Matrix4* currentView;
	float* fps;
};

