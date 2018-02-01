#include "BPLighting.h"

#include "../../GraphicsCommon.h"

BPLighting::BPLighting(Camera* cam, GBufferData* gBuffer,
	AmbientTextures* ambientTextures, int numAmbTex)
	: GraphicsModule(identifier, projMatrix, resolution)
{
	camera = cam;

	this->gBuffer = gBuffer;
	this->ambientTextures = ambientTextures;
	this->numAmbTex = numAmbTex;

	lightingPassShader = new Shader(SHADERDIR"/SSAO/ssao_lightingvert.glsl", SHADERDIR"/SSAO/ssao_lightingfrag.glsl", "", true);
}

void BPLighting::linkShaders()
{
	lightingPassShader->LinkProgram();
}

void BPLighting::regenerateShaders()
{
	lightingPassShader->Regenerate();
}

void BPLighting::initialise()
{
	locateUniforms();
}

void BPLighting::locateUniforms()
{
	loc_gPosition = glGetUniformLocation(lightingPassShader->GetProgram(), "gPosition");
	loc_gNormal = glGetUniformLocation(lightingPassShader->GetProgram(), "gNormal");
	loc_gAlbedo = glGetUniformLocation(lightingPassShader->GetProgram(), "gAlbedo");
	loc_shadows = glGetUniformLocation(lightingPassShader->GetProgram(), "shadows");
	loc_ambientTextures = glGetUniformLocation(lightingPassShader->GetProgram(), "ambientTextures");
	loc_texMatrices = glGetUniformLocation(lightingPassShader->GetProgram(), "texMatrices");
	loc_camMatrix = glGetUniformLocation(lightingPassShader->GetProgram(), "camMatrix");

	loc_numShadowCastingLights =
		glGetUniformLocation(lightingPassShader->GetProgram(), "numShadowCastingLights");
}

void BPLighting::apply()
{
	lightingPass();
}

void BPLighting::lightingPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, *FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setCurrentShader(lightingPassShader);

	glUniform1i(loc_gPosition, CommonGraphicsData::GPOSITION);
	glUniform1i(loc_gNormal, CommonGraphicsData::GNORMAL);
	glUniform1i(loc_gAlbedo, CommonGraphicsData::GALBEDO);
	glUniform1f(glGetUniformLocation(lightingPassShader->GetProgram(), "ambientLighting"), ambientLighting);

	glUniform1i(loc_numShadowCastingLights, shadowData->NUM_LIGHTS);

	glUniform1iv(loc_shadows, shadowData->NUM_LIGHTS, shadowData->shadowIndexes);
	glUniform1iv(loc_ambientTextures, numAmbTex, ambientTextures->texUnits);

	glUniformMatrix4fv(loc_texMatrices, shadowData->NUM_LIGHTS, false, (float*)shadowData->textureMatrices);

	viewMatrix = camera->BuildViewMatrix();
	glUniformMatrix4fv(loc_camMatrix, 1, false, (float*)&viewMatrix);

	updateShaderMatrices();

	currentShader->ApplyTexture(CommonGraphicsData::GPOSITION, *gBuffer->gPosition);
	currentShader->ApplyTexture(CommonGraphicsData::GNORMAL, *gBuffer->gNormal);
	currentShader->ApplyTexture(CommonGraphicsData::GALBEDO, *gBuffer->gAlbedo);

	for (int a = 0; a < numAmbTex; ++a)
	{
		glActiveTexture(GL_TEXTURE3 + a);
		glBindTexture(GL_TEXTURE_2D, *ambientTextures->textures[a]);
	}

	for (int s = 0; s < shadowData->NUM_LIGHTS; ++s)
	{
		glActiveTexture(GL_TEXTURE4 + s + (numAmbTex - 1));
		glBindTexture(GL_TEXTURE_2D, shadowData->shadows[s]);
	}

	renderScreenQuad();
}