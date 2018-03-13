#include "Wireframe.h"
#include "../../Utility/Camera.h"
#include "../../GraphicsUtility.h"

const int STEP_COUNT = 18;
const float DIVISOR = 360.0f / STEP_COUNT;

Wireframe::Wireframe(const std::string identifier, const NCLVector2 resolution, Camera* camera) : GraphicsModule(identifier, resolution)
{
	debugShader = new Shader(SHADERDIR"/debugVertex.glsl", SHADERDIR"/debugFragment.glsl", "", true);
	this->camera = camera;
}

Wireframe::~Wireframe()
{
	delete debugShader;
}

void Wireframe::linkShaders()
{
	debugShader->LinkProgram();
}

void Wireframe::initialise()
{
	
}

void Wireframe::apply()
{
	GraphicsUtility::ClearGLErrorStack();
	
	setCurrentShader(debugShader);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	GraphicsUtility::CheckGLError("");
	NCLMatrix4 viewMatrix = camera->buildViewMatrix();
	NCLMatrix4 viewProjMatrix = CommonGraphicsData::SHARED_PROJECTION_MATRIX * viewMatrix;
	buildLinesFromCircles();

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewProjMatrix"), 1, false, (float*)&viewProjMatrix);
	
	renderLines();

	GraphicsUtility::CheckGLError("");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Wireframe::regenerateShaders()
{
	debugShader->Regenerate();
}

void Wireframe::addLine(NCLVector3 from, NCLVector3 to, NCLVector3 colour)
{
	linePoints.push_back(from);
	linePoints.push_back(to);
	lineColours.push_back(colour);
	lineColours.push_back(colour);
}

void Wireframe::addCircle(NCLVector3 position, float radius, NCLVector3 colour)
{
	circlePositions.push_back(position);
	circleColours.push_back(colour);
	radii.push_back(radius);
}

void Wireframe::locateUniforms()
{
}

void Wireframe::buildLinesFromCircles()
{
	for (int j = 0; j < circlePositions.size(); ++j)
	{
		splitCircle(j);
	}

	circleColours.clear();
	circlePositions.clear();
	radii.clear();
}

void Wireframe::splitCircle(int circleIndex)
{
	for (int i = 0; i < STEP_COUNT; ++i)
	{

		float startx = radii[circleIndex] * (float)cos(DegToRad(i * DIVISOR)) + circlePositions[circleIndex].x;
		float endx = radii[circleIndex] * (float)cos(DegToRad((i + 1) * DIVISOR)) + circlePositions[circleIndex].x;


		float starty = radii[circleIndex] * (float)sin(DegToRad(i * DIVISOR)) + circlePositions[circleIndex].y;
		float endy = radii[circleIndex] * (float)sin(DegToRad((i + 1) * DIVISOR)) + circlePositions[circleIndex].y;

		addLine(NCLVector3(startx, starty, circlePositions[circleIndex].z),
			NCLVector3(endx, endy, circlePositions[circleIndex].z), circleColours[circleIndex]);
	}
}

void Wireframe::renderLines()
{
	if (!linePoints.empty())
	{
		glGenVertexArrays(1, &array);
		glBindVertexArray(array);
		glGenBuffers(2, buffers);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, linePoints.size() * sizeof(NCLVector3), &linePoints[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ARRAY_BUFFER, lineColours.size() * sizeof(NCLVector3), &lineColours[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(array);
		glDrawArrays(GL_LINES, 0, linePoints.size());
		glDeleteBuffers(2, buffers);

		glBindVertexArray(0);

		linePoints.clear();
		lineColours.clear();
	}
}

void Wireframe::updateWireframes()
{

}