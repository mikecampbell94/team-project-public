#pragma once

#include "../GraphicsModule.h"

#include "../../Rendering/View/Camera.h"
#include "../../ASSIMP/ModelMesh.h"
#include "../../ASSIMP/Model.h"

#include <vector>
#include "../Effects/Skybox.h"
#include "../Effects/ParticleSystem.h"

class Window;

class GBuffer : public GraphicsModule
{
public:
	GBuffer(Window* window, Camera* camera, std::vector<ModelMesh*>* modelsInFrame,
		vector<ModelMesh*>* transparentModelsInFrame, std::vector<Model*>** models);
	virtual ~GBuffer();

	void linkShaders() override;
	void regenerateShaders() override;

	void initialise() override;
	void apply() override;

	GBufferData* getGBuffer() const
	{
		return SGBuffer;
	}

	void setReflectionTextureID(unsigned int newTextureID)
	{
		textureID = newTextureID;
	}

	GLuint rboDepth;
	GLuint gBuffer;

	Skybox* skybox;

private:
	void locateUniforms() override;
	void initGBuffer();
	void initAttachments();

	void renderGeometry(vector<ModelMesh*>* meshes);

	GLuint gPosition;
	GLuint gNormal;
	GLuint gAlbedo;
	GLuint attachments[3] =
	{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	Shader* geometryPass;

	Camera* camera;
	Window* window;

	std::vector<ModelMesh*>* modelsInFrame;
	std::vector<ModelMesh*>* transparentModelsInFrame;
	std::vector<Model*>** models;
	GBufferData* SGBuffer;

	unsigned int textureID;

	GLint loc_skybox;
	GLint loc_cameraPos;
	GLint loc_hasTexture;
	GLint loc_isReflective;
	GLint loc_reflectionStrength;
	GLint loc_baseColour;
};

