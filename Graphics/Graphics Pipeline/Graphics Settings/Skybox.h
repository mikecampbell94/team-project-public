#pragma once

#include "../GraphicsModule.h"
#include "../../Utilities/Maths/Matrix4.h"

#include <vector>
#include <string>

class Skybox : public GraphicsModule
{
public:
	Skybox(const std::string identifier, const NCLVector2 resolution, NCLMatrix4* viewMatrix);
	~Skybox();

	void linkShaders() override;
	void regenerateShaders() override;

	void initialise() override;
	void apply() override;

	void setSkyboxTexture(unsigned int newTextureID)
	{
		textureID = newTextureID;
	}

	GLuint* GBufferFBO;

private:
	void initialiseMesh();
	void locateUniforms() override {}

	NCLMatrix4* viewMatrix;
	Shader* skyboxShader;
	unsigned int textureID;
	unsigned int VAO, VBO;
};