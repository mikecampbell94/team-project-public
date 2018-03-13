#pragma once
#include "../GraphicsModule.h"

class Camera;

class Wireframe : public GraphicsModule
{
public:
	Wireframe(const std::string identifier, const NCLVector2 resolution, Camera* camera);
	~Wireframe();

	void linkShaders() override;
	void initialise() override;
	void apply() override;

	void regenerateShaders() override;

	void addLine(NCLVector3 from, NCLVector3 to, NCLVector3 colour);

private:
	void locateUniforms() override;

	Shader* debugShader;

	std::vector<NCLVector3> points;
	std::vector<NCLVector3> colours;

	GLuint array;
	GLuint buffer;

	//hello haha test
	Camera* camera;
};

