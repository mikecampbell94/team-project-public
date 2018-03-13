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
	void addCircle(NCLVector3 position, float radius, NCLVector3 colour);

private:
	void updateWireframes();
	void locateUniforms() override;

	void buildLinesFromCircles();
	void splitCircle(int circleIndex);
	void renderLines();

	Shader* debugShader;

	std::vector<NCLVector3> linePoints;
	std::vector<NCLVector3> lineColours;

	std::vector<NCLVector3> circlePositions;
	std::vector<float> radii;
	std::vector<NCLVector3> circleColours;

	GLuint array;
	GLuint buffers[2];

	Camera* camera;
};

