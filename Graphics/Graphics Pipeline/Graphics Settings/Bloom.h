#pragma once

#include "../GraphicsModule.h"

class Bloom : public GraphicsModule
{
public:
	Bloom(const std::string identifier, const NCLMatrix4 projmatrix,
		const NCLVector2 resolution, int strength);
	~Bloom();

	void linkShaders() override;
	void regenerateShaders() override;

	void initialise() override;
	void apply() override;

	GLuint FBO;
	GLuint* motionBlurFBO;

private:
	void createTexture();
	void initialiseBlur();

	void locateUniforms() override;

	void applyBlur();
	void combine();

	//Shader* bloomShader;
	Shader* blurShader;
	Shader* combineShader;

	//Brightness tex
	GLuint colourBuffers[2];
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

	//Uniform locations
	GLint loc_horizontal;
	GLint loc_image;
	GLint loc_scene;
	GLint loc_bloomBlur;

	//Blur tex
	GLuint pingpongFBO[2];
	GLuint pingpongColourBuffers[2];
	GLuint rboDepth;

	bool horizontal;
	bool first_iteration;

	int blurStrength;
};

