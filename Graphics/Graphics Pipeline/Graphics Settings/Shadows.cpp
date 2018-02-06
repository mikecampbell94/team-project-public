#include "Shadows.h"

#include "../../GraphicsCommon.h"
#include "../../GraphicsUtility.h"
#include "../../Rendering/OGLRenderer.h"

Shadows::Shadows(const std::string identifier, const Matrix4 projmatrix,
	const Vector2 resolution, int numShadowCastingLights, Light** lights,
	std::vector<Mesh*>** models)
	: GraphicsModule(identifier, projMatrix, resolution)
{
	shadowFBOs = new GLuint[numShadowCastingLights];

	shadowData = new ShadowData();
	shadowData->NUM_LIGHTS = numShadowCastingLights;

	shadowData->shadowIndexes = new int[numShadowCastingLights];
	shadowData->shadows = new GLuint[numShadowCastingLights];
	shadowData->textureMatrices = new Matrix4[numShadowCastingLights];

	for (int i = 0; i < numShadowCastingLights; i++)
	{
		shadowData->shadowIndexes[i] = i + 4;
	}

	shadowShader = new Shader(SHADERDIR"shadowvert.glsl", SHADERDIR"shadowfrag.glsl", "",true);

	this->lights = lights;
	this->models = models;
}

void Shadows::linkShaders()
{
	shadowShader->LinkProgram();
}

void Shadows::regenerateShaders()
{
	shadowShader->Regenerate();
}

void Shadows::initialise()
{
	//Shadow prep
	initShadowTex();
	initShadowBuffer();
}

void Shadows::apply()
{
	drawShadowScene();
}

void Shadows::initShadowTex()
{
	for (int i = 0; i < shadowData->NUM_LIGHTS; ++i)
	{
		glGenTextures(1, &shadowData->shadows[i]);
		glBindTexture(GL_TEXTURE_2D, shadowData->shadows[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
			GL_COMPARE_R_TO_TEXTURE);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GraphicsUtility::CheckGLError("Shadow textures");
}

void Shadows::initShadowBuffer()
{
	for (int i = 0; i < shadowData->NUM_LIGHTS; ++i)
	{
		glGenFramebuffers(1, &shadowFBOs[i]);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBOs[i]);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, shadowData->shadows[i], 0);

		glDrawBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		GraphicsUtility::VerifyBuffer("Shadow Buffer " + i, false);
	}
}

void Shadows::drawShadowScene()
{
	glCullFace(GL_FRONT);
	setCurrentShader(shadowShader);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	for (int i = 0; i < shadowData->NUM_LIGHTS; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFBOs[i]);
		glClear(GL_DEPTH_BUFFER_BIT);

		viewMatrix = Matrix4::buildViewMatrix(lights[i]->GetPosition(), Vector3(0, 0, 0));
		shadowData->textureMatrices[i] = biasMatrix * (projMatrix * viewMatrix);

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);

		for each (Mesh* m in **models)
		{
			for each (SubMesh* mesh in m->meshes)
			{
				mesh->DrawShadow();
			}
		}
	}

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, resolution.x, resolution.y);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}