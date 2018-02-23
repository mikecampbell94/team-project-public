#pragma once

#include "../GraphicsModule.h"

class Database;

class PaintTrail : public GraphicsModule
{
public:
	PaintTrail(const std::string identifier, const Matrix4 projmatrix,
		const Vector2 resolution, Database* database);
	~PaintTrail();

	void linkShaders() override;
	void initialise() override;
	void apply() override;
	void regenerateShaders() override;

	GLuint paintTrailTexture;
	Matrix4 textureMatrices;

private:
	void locateUniforms() override {}

	Database* database;
	Shader* paintTrailShader;

	GLuint buffer;
	GLuint rboDepth;
	GLuint attachment[1] = { GL_COLOR_ATTACHMENT6 };

	bool floorRendered = false;
};

