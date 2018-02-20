#pragma once

#include "../GraphicsModule.h"
#include "../../Utility/Camera.h"
#include "../../Utility/Light.h"

#include <vector>

class BPLighting : public GraphicsModule
{
public:
	BPLighting(const std::string identifier, const Matrix4 projmatrix,
		const Vector2 resolution, Camera* cam, GBufferData* gBuffer, std::vector<Light*>** lights,
		SSAOTextures* ssaoTextures);

	virtual ~BPLighting()
	{
		delete lightingPassShader;
	}

	void linkShaders()  override;
	void regenerateShaders()  override;

	void updateShadowData(ShadowData* shadowData) 
	{
		this->shadowData = shadowData;
	}

	void initialise() override;
	void apply() override;

	GLuint FBO;
	float ambientLighting;
	bool* SSAOApplied;

private:
	void locateUniforms() override;
	void lightingPass();

	//Uniform locations
	GLint lightDataBuffer;

	GLint loc_gPosition;
	GLint loc_gNormal;
	GLint loc_gAlbedo;
	GLint loc_shadows;
	GLint loc_ambientTextures;
	GLint loc_texMatrices;
	GLint loc_numberOfLights;
	GLint loc_camMatrix;
	GLint loc_cameraPos;
	GLint loc_numShadowCastingLights;

	Shader*		lightingPassShader;
	Camera*		camera;
	std::vector<LightData> lightDatas;
	std::vector<Light*>** lights;
	ShadowData* shadowData;
	GBufferData*	gBuffer;
	SSAOTextures* ambientTextures;
};

