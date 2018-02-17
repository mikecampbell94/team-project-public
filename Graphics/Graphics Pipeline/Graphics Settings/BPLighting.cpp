#include "BPLighting.h"

#include "../../GraphicsCommon.h"
#include "../../GraphicsUtility.h"

BPLighting::BPLighting(const std::string identifier, const Matrix4 projmatrix,
	const Vector2 resolution, Camera* cam, GBufferData* gBuffer, std::vector<Light*>** lights,
	AmbientTextures* ssaoTextures)
	: GraphicsModule(identifier, projMatrix, resolution)
{
	camera = cam;

	this->gBuffer = gBuffer;
	this->numAmbTex = 0;

	for each (Light* light in **lights)
	{
		lightDatas.push_back(light->GetData());
	}

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
	glGenFramebuffers(1, &FBO);
	locateUniforms();

	lightDataBuffer = GraphicsUtility::InitSSBO(1, 1, lightDataBuffer, sizeof(LightData) * lightDatas.size(), &lightDatas, GL_STATIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightDataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightData) * lightDatas.size(),
		&lightDatas[0], GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lightDataBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void BPLighting::locateUniforms()
{
	loc_gPosition = glGetUniformLocation(lightingPassShader->GetProgram(), "gPosition");
	loc_gNormal = glGetUniformLocation(lightingPassShader->GetProgram(), "gNormal");
	loc_gAlbedo = glGetUniformLocation(lightingPassShader->GetProgram(), "gAlbedo");
	//loc_shadows = glGetUniformLocation(lightingPassShader->GetProgram(), "shadows");
	loc_ambientTextures = glGetUniformLocation(lightingPassShader->GetProgram(), "ambientTextures");
	loc_texMatrices = glGetUniformLocation(lightingPassShader->GetProgram(), "texMatrices");
	loc_camMatrix = glGetUniformLocation(lightingPassShader->GetProgram(), "camMatrix");

	loc_numShadowCastingLights = glGetUniformLocation(lightingPassShader->GetProgram(), "numShadowCastingLights");
	loc_numberOfLights = glGetUniformLocation(lightingPassShader->GetProgram(), "numberOfLights");
}

void BPLighting::apply()
{
	lightingPass();
}

void BPLighting::lightingPass()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setCurrentShader(lightingPassShader);

	glUniform1i(loc_gPosition, CommonGraphicsData::GPOSITION);
	glUniform1i(loc_gNormal, CommonGraphicsData::GNORMAL);
	glUniform1i(loc_gAlbedo, CommonGraphicsData::GALBEDO);

	//glUniform1f(glGetUniformLocation(lightingPassShader->GetProgram(), "ambientLighting"), ambientLighting);

	glUniform1i(loc_numberOfLights, lightDatas.size());

	//glUniform1i(loc_numShadowCastingLights, shadowData->NUM_LIGHTS);

	//glUniform1iv(loc_shadows, shadowData->NUM_LIGHTS, shadowData->shadowIndexes);
	if (numAmbTex > 0)
		glUniform1iv(loc_ambientTextures, numAmbTex, ambientTextures->texUnits);

	//glUniformMatrix4fv(loc_texMatrices, shadowData->NUM_LIGHTS, false, (float*)shadowData->textureMatrices);

	viewMatrix = camera->buildViewMatrix();
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

	//for (int s = 0; s < shadowData->NUM_LIGHTS; ++s)
	//{
	//	glActiveTexture(GL_TEXTURE4 + s + (numAmbTex - 1));
	//	glBindTexture(GL_TEXTURE_2D, shadowData->shadows[s]);
	//}

	renderScreenQuad();
}