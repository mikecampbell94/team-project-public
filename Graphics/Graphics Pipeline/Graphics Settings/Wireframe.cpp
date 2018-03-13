#include "Wireframe.h"
#include "../../Utility/Camera.h"
#include "../../GraphicsUtility.h"

Wireframe::Wireframe(const std::string identifier, const NCLVector2 resolution, Camera* camera) : GraphicsModule(identifier, resolution)
{
	debugShader = new Shader(SHADERDIR"/debugVertex.glsl", SHADERDIR"/debugFragment.glsl");
	//glGenVertexArrays(1, &array);
	//glGenBuffers(2, buffers);
	this->camera = camera;
	addLine(NCLVector3(0, 0, 0), NCLVector3(0, 1000, 0), NCLVector3(1, 0, 0));
	addLine(NCLVector3(0, 0, 0), NCLVector3(100, 1000, 0), NCLVector3(1, 0, 0));
}

Wireframe::~Wireframe()
{
	delete debugShader;
	glDeleteBuffers(1, &buffer);
}

void Wireframe::linkShaders()
{
	debugShader->LinkProgram();
}

void Wireframe::initialise()
{
	glGenVertexArrays(1, &array);
	glBindVertexArray(array);
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(NCLVector3), &points[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	//glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(NCLVector3), &colours[0], GL_DYNAMIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Wireframe::apply()
{
	GraphicsUtility::ClearGLErrorStack();

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	setCurrentShader(debugShader);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//glLineWidth(10);
	GraphicsUtility::CheckGLError("");
	NCLMatrix4 viewMatrix = camera->buildViewMatrix();
	NCLMatrix4 viewProjMatrix = CommonGraphicsData::SHARED_PROJECTION_MATRIX * viewMatrix;
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewProjMatrix"), 1, false, (float*)&viewProjMatrix);
	if (!points.empty())
	{

		glBindVertexArray(array);
		glDrawArrays(GL_LINES, 0, points.size());

		glBindVertexArray(0);

		//points.clear();
		//colours.clear();
	}

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
	points.push_back(from);
	points.push_back(to);
	colours.push_back(colour);
}

void Wireframe::locateUniforms()
{
}
