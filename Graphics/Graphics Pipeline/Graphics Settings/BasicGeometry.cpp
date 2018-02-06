#include "BasicGeometry.h"

#include "../../GraphicsCommon.h"
#include "../../Utility/Camera.h"

BasicGeometry::BasicGeometry(const std::string identifier, 
	const Matrix4 projmatrix, const Vector2 resolution, Camera* camera, std::vector<SubMesh*>* modelsInFrame)
	: GraphicsModule(identifier, projmatrix, resolution)
{
	this->modelsInFrame = modelsInFrame;
	basicShader = new Shader(SHADERDIR"/basicVertex.glsl", SHADERDIR"/basicFrag.glsl");

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
	for (int i = 0; i < modelsInFrame->size(); ++i)
	{
		modelsInFrame->at(i)->Draw(*currentShader);
	}

	viewMatrix = camera->buildViewMatrix();
}

void BasicGeometry::locateUniforms()
{
}
