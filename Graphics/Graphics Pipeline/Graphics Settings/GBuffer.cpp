#include "GBuffer.h"

#include "../Input/Devices/Window.h"
#include "../../GraphicsUtility.h"
#include "../../GraphicsCommon.h"

GBuffer::GBuffer(const std::string identifier, const Matrix4 projmatrix,
	const Vector2 resolution, Window* window, Camera* camera, std::vector<SceneNode*>* nodesInFrame)
	: GraphicsModule(identifier, projmatrix, resolution)
{
	this->nodesInFrame = nodesInFrame;
	this->projMatrix = projmatrix;
	this->camera = camera;
	this->window = window;

	geometryPass = new Shader(SHADERDIR"/SSAO/ssao_geometryvert.glsl",
		SHADERDIR"/SSAO/ssao_geometryfrag.glsl");

	SGBuffer = new GBufferData();
	SGBuffer->gAlbedo = &gAlbedo;
	SGBuffer->gNormal = &gNormal;
	SGBuffer->gPosition = &gPosition;
}

GBuffer::~GBuffer()
{
	delete geometryPass;
	glDeleteTextures(1, &gPosition);
	glDeleteTextures(1, &gNormal);
	glDeleteTextures(1, &gAlbedo);
}

void GBuffer::linkShaders()
{
	geometryPass->LinkProgram();
}

void GBuffer::regenerateShaders()
{
	geometryPass->Regenerate();
}

void GBuffer::initialise()
{
	initGBuffer();
	initAttachments();
	locateUniforms();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::apply()
{
	//Render any geometry to GBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderGeometry(nodesInFrame);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_BLEND);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::locateUniforms()
{
	setCurrentShader(geometryPass);

	loc_skybox = glGetUniformLocation(geometryPass->GetProgram(), "skybox");
	loc_cameraPos = glGetUniformLocation(geometryPass->GetProgram(), "cameraPos");
	loc_hasTexture = glGetUniformLocation(geometryPass->GetProgram(), "hasTexture");
	loc_isReflective = glGetUniformLocation(geometryPass->GetProgram(), "isReflective");
	loc_reflectionStrength = glGetUniformLocation(geometryPass->GetProgram(), "reflectionStrength");
	loc_baseColour = glGetUniformLocation(geometryPass->GetProgram(), "baseColour");
}

void GBuffer::initGBuffer()
{
	GraphicsUtility::ClearGLErrorStack();

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//Position colour buffer
	glGenTextures(1, &gPosition);
	GraphicsUtility::CreateScreenTexture(resolution, gPosition, 
		GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST, CommonGraphicsData::GPOSITION, true);
	GraphicsUtility::CheckGLError("GPosition");

	//Normal coluor buffer
	glGenTextures(1, &gNormal);
	GraphicsUtility::CreateScreenTexture(resolution, gNormal, 
		GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST, CommonGraphicsData::GNORMAL, false);
	GraphicsUtility::CheckGLError("GNormal");

	//Colour + specular colour buffer
	glGenTextures(1, &gAlbedo);
	GraphicsUtility::CreateScreenTexture(resolution, gAlbedo, 
		GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, CommonGraphicsData::GALBEDO, false);
	GraphicsUtility::CheckGLError("GAlbedo");

	GraphicsUtility::VerifyBuffer("GBuffer", false);
}

void GBuffer::initAttachments()
{
	//Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	glDrawBuffers(3, attachments);

	//Create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)resolution.x, (GLsizei)resolution.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	GraphicsUtility::VerifyBuffer("RBO Depth GBuffer", false);
}

void GBuffer::renderGeometry(std::vector<SceneNode*>* nodesInFrame)
{
	setCurrentShader(geometryPass);
	viewMatrix = camera->buildViewMatrix();
	updateShaderMatrices();

	glUniform1i(glGetUniformLocation(geometryPass->GetProgram(), "paintTrailTexture"), 6);
	glUniformMatrix4fv(glGetUniformLocation(geometryPass->GetProgram(), "paintTrailTextureMatrix"), 1, false, (float*)paintTextureMatrix);
	currentShader->ApplyTexture(6, *paintTrailTexture);

	glUniform3fv(loc_cameraPos, 1, (float*)&camera->getPosition());

	for (unsigned int i = 0; i < nodesInFrame->size(); ++i)
	{
		glUniform1i(glGetUniformLocation(geometryPass->GetProgram(), "isPaintSurface"), nodesInFrame->at(i)->isPaintSurface);
		glUniform4fv(loc_baseColour, 1, (float*)&nodesInFrame->at(i)->getColour());
		nodesInFrame->at(i)->Draw(*currentShader);
	}
}

