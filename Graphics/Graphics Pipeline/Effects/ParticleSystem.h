#pragma once

#include "../GraphicsModule.h"

#include <vector>

struct Particle
{
	Matrix4 modelMatrix;
	Vector3 originalPosition;
	Vector3 frameTranslation;
	Vector4 colour;
	float particleSize;
	float alphaDecay;
	float decayRate;

	Particle(Vector3 startPosition, Vector3 translation, Vector4 colour,
		float size, float rate)
	{
		originalPosition = startPosition;
		modelMatrix = Matrix4::translation(originalPosition);
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
	explicit ParticleSystem(const std::string identifier, const Matrix4 projmatrix,
		const Vector2 resolution, Matrix4* viewMatrix);
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

	Matrix4* viewMatrix;

	Shader* particleShader;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int TVBO;
	GLuint textureID;
	GLuint textureID1;

	float frameCount = 0;
};

