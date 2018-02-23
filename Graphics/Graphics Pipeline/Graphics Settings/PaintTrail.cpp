#include "PaintTrail.h"

#include "../../GraphicsCommon.h"
#include "../../GraphicsUtility.h"
#include "../../../Gameplay/GameObject.h"
#include "../Resource Management/Database/Database.h"
#include "../../Rendering/OGLRenderer.h"

#define SHADOWSIZE 4096

PaintTrail::PaintTrail(const std::string identifier, const Matrix4 projmatrix,
	const Vector2 resolution, Database* database)
	: GraphicsModule(identifier, projmatrix, resolution)
{
	paintTrailShader = new Shader(SHADERDIR"PaintTrail/paintTrailVert.glsl", SHADERDIR"PaintTrail/paintTrailFrag.glsl", "", true);
	this->database = database;
}

PaintTrail::~PaintTrail()
{
	delete paintTrailShader;
}

void PaintTrail::linkShaders()
{
	paintTrailShader->LinkProgram();
}

void PaintTrail::regenerateShaders()
{
	paintTrailShader->Regenerate();
}

void PaintTrail::initialise()
{
	GraphicsUtility::ClearGLErrorStack();

	glGenFramebuffers(1, &buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	glGenTextures(1, &paintTrailTexture);
	GraphicsUtility::CreateScreenTexture(resolution, paintTrailTexture,
		GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, 6, false);
	GraphicsUtility::CheckGLError("PaintTrailTexture");
	GraphicsUtility::VerifyBuffer("paintTrailBuffer", false);

	//Create and attach depth buffer (renderbuffer)
	glDrawBuffers(1, attachment);
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)resolution.x, (GLsizei)resolution.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	GraphicsUtility::VerifyBuffer("RBO Depth Paint Trail", false);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PaintTrail::apply()
{
	//glCullFace(GL_FRONT);
	setCurrentShader(paintTrailShader);

	//glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
	if (!floorRendered)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	//glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);

	GameObject* floor = static_cast<GameObject*>(database->getTable("GameObjects")->getResource("floor"));
	GameObject* player = static_cast<GameObject*>(database->getTable("GameObjects")->getResource("playerBall"));

	viewMatrix = Matrix4::buildViewMatrix(Vector3(1, 800, 1), Vector3(0, 0, 0));
	textureMatrices = biasMatrix * (projMatrix * viewMatrix);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);

	//glDisable(GL_DEPTH_TEST);
	if (floor != nullptr && player != nullptr)
	{
		//glClear(GL_DEPTH_BUFFER_BIT);
		if (!floorRendered)
		{
			glUniform4fv(glGetUniformLocation(paintTrailShader->GetProgram(), "baseColour"), 1, (float*)&floor->getSceneNode()->getColour());
			floor->getSceneNode()->Draw(*paintTrailShader);
			floorRendered = true;
		}

		glUniform4fv(glGetUniformLocation(paintTrailShader->GetProgram(), "baseColour"), 1, (float*)&player->getSceneNode()->getColour());
		player->getSceneNode()->Draw(*paintTrailShader);
	}

	//glEnable(GL_DEPTH_TEST);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//glViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glCullFace(GL_BACK);
}