#include "BasicGeometry.h"

#include "../../GraphicsCommon.h"
#include "../../Utility/Camera.h"

BasicGeometry::BasicGeometry(const std::string identifier, 
	const Matrix4 projmatrix, const Vector2 resolution, Camera* camera, std::vector<SceneNode*>* nodesInFrame)
	: GraphicsModule(identifier, projmatrix, resolution)
{
	this->nodesInFrame = nodesInFrame;
	basicShader = new Shader(SHADERDIR"/basicVertex.glsl", SHADERDIR"/basicFrag.glsl");
	//basicShader = new Shader(SHADERDIR"/TexturedVertex.glsl", SHADERDIR"/TexturedFrag.glsl");
	this->camera = camera;

}

BasicGeometry::~BasicGeometry()
{
	delete basicShader;
}

void BasicGeometry::linkShaders()
{
	basicShader->LinkProgram();
}

void BasicGeometry::regenerateShaders()
{
	basicShader->Regenerate();
}

void BasicGeometry::initialise()
{
}

void BasicGeometry::apply()
{
	setCurrentShader(basicShader);

	updateShaderMatrices();
	for (unsigned int i = 0; i < nodesInFrame->size(); ++i)
	{
		nodesInFrame->at(i)->Draw(*currentShader);
	}



	viewMatrix = camera->buildViewMatrix();
}

void BasicGeometry::locateUniforms()
{
}
