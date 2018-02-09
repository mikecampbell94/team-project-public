#include "SSAO.h"

#include "../../GraphicsUtility.h"
#include "../../GraphicsCommon.h"

const int KERNEL_SIZE = 32;
const int RESOLUTION_SCALE_X = 640;
const int RESOLUTION_SCALE_Y = 360;

SSAO::SSAO(const std::string identifier, const Matrix4 projmatrix,
	const Vector2 resolution, Camera* cam, AmbientTextures* ambientTextures, GBufferData* SGBuffer)
	: GraphicsModule(identifier, projMatrix, resolution)
{
	this->ambientTextures = ambientTextures;
	this->SGBuffer = SGBuffer;
	camera = cam;

	//SSAO Shaders
	SSAOCol = new Shader(SHADERDIR"/SSAO/ssao_vert.glsl", SHADERDIR"/SSAO/ssao_frag.glsl");
	SSAOBlur = new Shader(SHADERDIR"/SSAO/ssao_vert.glsl", SHADERDIR"/SSAO/ssao_blurfrag.glsl");

	ambientTextures->textures[CommonGraphicsData::SSAO_INDEX] = &ssaoColorBufferBlur;
	ambientTextures->texUnits[CommonGraphicsData::SSAO_INDEX] = 3;

	xSize = 2;// GLConfig::RESOLUTION.x / RESOLUTION_SCALE_X;
	ySize = 2;//GLConfig::RESOLUTION.y / RESOLUTION_SCALE_Y;
}

void SSAO::linkShaders()
{
	SSAOCol->LinkProgram();
	SSAOBlur->LinkProgram();
}

void SSAO::initialise()
{
	initSSAOBuffers();

	//For the SSAO texture
	generateSampleKernel();
	generateNoiseTexture();

	locateUniforms();
}

void SSAO::locateUniforms()
{
	loc_ssaoRadius = glGetUniformLocation(SSAOCol->GetProgram(), "radius");
	loc_ssaoBias = glGetUniformLocation(SSAOCol->GetProgram(), "bias");
	loc_gPosition = glGetUniformLocation(SSAOCol->GetProgram(), "gPosition");
	loc_gNormal = glGetUniformLocation(SSAOCol->GetProgram(), "gNormal");
	loc_texNoise = glGetUniformLocation(SSAOCol->GetProgram(), "texNoise");

	loc_kernel = glGetUniformLocation(SSAOCol->GetProgram(), "samples");

	loc_ssaoInput = glGetUniformLocation(SSAOBlur->GetProgram(), "ssaoInput");
	loc_xSize = glGetUniformLocation(SSAOBlur->GetProgram(), "xSize");
	loc_ySize = glGetUniformLocation(SSAOBlur->GetProgram(), "ySize");
}

void SSAO::apply()
{
	glDepthMask(GL_FALSE);
	//Generate the SSAO texture
	generateSSAOTex();

	//Blur the texture
	SSAOBlurTex();
	glDepthMask(GL_TRUE);
}

void SSAO::regenerateShaders()
{
	SSAOCol->Regenerate();
	SSAOBlur->Regenerate();
}

void SSAO::initSSAOBuffers()
{
	//Create framebuffer to hold SSAO processing stage 
	glGenFramebuffers(1, &ssaoFBO);
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	//SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	GraphicsUtility::CreateScreenTexture(resolution, ssaoColorBuffer, 
		GL_RED, GL_RGB, GL_FLOAT, GL_NEAREST, 0, false);

	GraphicsUtility::VerifyBuffer("SSAO Frame", false);

	//Blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	GraphicsUtility::CreateScreenTexture(resolution, ssaoColorBufferBlur, 
		GL_RED, GL_RGB, GL_FLOAT, GL_NEAREST, 0, false);

	GraphicsUtility::VerifyBuffer("Blur", false);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::generateSampleKernel()
{
	const std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f);
	std::default_random_engine generator;

	for (unsigned int i = 0; i < KERNEL_SIZE; ++i)
	{
		Vector3 sample(randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));

		sample.normalise();
		sample = sample * randomFloats(generator);
		float scale = float(i) / KERNEL_SIZE;

		//Scale samples so they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);//0.1f + (scale * scale) * (1.0f - 0.1f); //Lerp
		sample = sample * scale;
		ssaoKernel.push_back(sample);
	}
}

void SSAO::generateNoiseTexture()
{
	const std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	std::default_random_engine generator;

	const int xSize = 2;// GLConfig::RESOLUTION.x / 640;
	const int ySize = 2;// 720;// GLConfig::RESOLUTION.y / 360;

	const int noiseSize = (xSize * 2) * (ySize * 2);

	//Generate the texture
	for (unsigned int i = 0; i < noiseSize; i++)
	{
		Vector3 noise(
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator) * 2.0f - 1.0f,
			0.0f); // rotate around z-axis (in tangent space)

		ssaoNoise.push_back(noise);
	}

	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, xSize * 2, ySize * 2, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SSAO::generateSSAOTex()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	setCurrentShader(SSAOCol);

	glUniform3fv(loc_kernel, KERNEL_SIZE, (float*)&ssaoKernel[0]);

	viewMatrix = camera->buildViewMatrix();

	//Basic uniforms
	updateShaderMatrices();

	glUniform1i(loc_ssaoRadius, (GLint)ssaoRadius);
	glUniform1i(loc_ssaoBias, (GLint)ssaoBias);

	//Texture units
	glUniform1i(loc_gPosition, CommonGraphicsData::GPOSITION);
	glUniform1i(loc_gNormal, CommonGraphicsData::GNORMAL);
	glUniform1i(loc_texNoise, NOISE_TEX);

	currentShader->ApplyTexture(CommonGraphicsData::GPOSITION, *SGBuffer->gPosition);
	currentShader->ApplyTexture(CommonGraphicsData::GNORMAL, *SGBuffer->gNormal);
	currentShader->ApplyTexture(NOISE_TEX, noiseTexture);

	renderScreenQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAO::SSAOBlurTex()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	setCurrentShader(SSAOBlur);

	glUniform1i(loc_xSize, xSize);
	glUniform1i(loc_ySize, ySize);

	glUniform1i(loc_ssaoInput, SSAO_TEX);
	currentShader->ApplyTexture(SSAO_TEX, ssaoColorBuffer);

	renderScreenQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}