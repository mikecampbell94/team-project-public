#pragma once

#include "../GraphicsModule.h"

#include <vector>

struct Particle
{
	NCLMatrix4 modelMatrix;
	NCLVector3 originalPosition;
	NCLVector3 frameTranslation;
	NCLVector4 colour;
	float particleSize;
	float alphaDecay;
	float decayRate;

	Particle(NCLVector3 startPosition, NCLVector3 translation, NCLVector4 colour,
		float size, float rate)
	{
		originalPosition = startPosition;
		modelMatrix = NCLMatrix4::translation(originalPosition);
		frameTranslation = translation;
		alphaDecay = 0.0f;
		particleSize = size;
		decayRate = rate;
		this->colour = colour;
	}
};

class ParticleSystem : public GraphicsModule
{
public:
	explicit ParticleSystem(const std::string identifier, const NCLMatrix4 projmatrix,
		const NCLVector2 resolution, NCLMatrix4* viewMatrix);
	~ParticleSystem();

	void linkShaders() override;
	void regenerateShaders() override;

	void initialise() override;
	void apply() override;

	GLuint* motionBlurFBO;

	std::vector<Particle>* particles;
private:
	void initialiseMesh();
	void locateUniforms() override {}

	NCLMatrix4* viewMatrix;

	Shader* particleShader;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int TVBO;
	GLuint textureID;
	GLuint textureID1;

	float frameCount = 0;
};

