#include "Bloom.h"

#include "../../GraphicsUtility.h"
#include "../../GraphicsCommon.h"

const int IMAGE = 0;
const int SCENE = 0;
const int BLOOM_BLUR = 1;

Bloom::Bloom(const std::string identifier, const Matrix4 projmatrix,
	const Vector2 resolution, int strength) 
	: GraphicsModule(identifier, projMatrix, resolution)
{
	blurStrength = strength;

	blurShader = new Shader(SHADERDIR"/Bloom/blurvert.glsl", SHADERDIR"/Bloom/blurfrag.glsl");
	combineShader = new Shader(SHADERDIR"/Bloom/combinevert.glsl", SHADERDIR"/Bloom/combinefrag.glsl");
}

Bloom::~Bloom()
{
	delete blurShader;
	delete combineShader;
}

void Bloom::linkShaders()
{
	blurShader->LinkProgram();
	combineShader->LinkProgram();
}

void Bloom::regenerateShaders()
{
	blurShader->Regenerate();
	combineShader->Regenerate();
}

void Bloom::initialise()
{
	createTexture();
	initialiseBlur();
	locateUniforms();
}

void Bloom::createTexture()
{
	GraphicsUtility::ClearGLErrorStack();

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//Brightness texture
	glGenTextures(2, colourBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		//Clamp to the edge as the blur filter would otherwise sample repeated texture values!
		GraphicsUtility::CreateScreenTexture(resolution, colourBuffers[i], GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, i, true);
	}

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)resolution.x, (GLsizei)resolution.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	//Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	glDrawBuffers(2, attachments);

	GraphicsUtility::VerifyBuffer("RBO DEPTH", false);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GraphicsUtility::CheckGLError("Bloom texture");
}

void Bloom::initialiseBlur()
{
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColourBuffers);

	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		GraphicsUtility::CreateScreenTexture(resolution, pingpongColourBuffers[i], GL_RGB16F, GL_RGB, GL_FLOAT, GL_LINEAR, 0, true);
		GraphicsUtility::VerifyBuffer("Bloom Blur " + std::to_string(i), false);
	}

	GraphicsUtility::CheckGLError("Bloom blur");
}

void Bloom::locateUniforms()
{
	loc_horizontal = glGetUniformLocation(blurShader->GetProgram(), "horizontal");
	loc_image = glGetUniformLocation(blurShader->GetProgram(), "image");

	loc_scene = glGetUniformLocation(combineShader->GetProgram(), "scene");
	loc_bloomBlur = glGetUniformLocation(combineShader->GetProgram(), "bloomBlur");
}

void Bloom::apply()
{
	glEnable(GL_DEPTH_TEST);

	horizontal = true;
	first_iteration = true;
	
	applyBlur();
	combine();
}

void Bloom::applyBlur()
{
	setCurrentShader(blurShader);

	for (int i = 0; i < blurStrength; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		glUniform1i(loc_horizontal, horizontal);

		glUniform1i(loc_image, IMAGE);

		//Bind texture of other framebuffer (or scene if first iteration)
		glActiveTexture(GL_TEXTURE0 + IMAGE);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colourBuffers[1] :
			pingpongColourBuffers[!horizontal]);

		renderScreenQuad();

		horizontal = !horizontal;
		if (first_iteration) first_iteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Bloom::combine()
{
	glBindFramebuffer(GL_FRAMEBUFFER, *motionBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setCurrentShader(combineShader);

	glUniform1i(loc_scene, SCENE);
	glUniform1i(loc_bloomBlur, BLOOM_BLUR);

	glActiveTexture(GL_TEXTURE0 + SCENE);
	glBindTexture(GL_TEXTURE_2D, colourBuffers[0]);

	glActiveTexture(GL_TEXTURE0 + BLOOM_BLUR);
	glBindTexture(GL_TEXTURE_2D, pingpongColourBuffers[!horizontal]);

	renderScreenQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
