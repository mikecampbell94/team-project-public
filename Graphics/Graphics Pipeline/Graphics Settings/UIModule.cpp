#include "UIModule.h"
#include "../../GraphicsCommon.h"





UIModule::UIModule(const std::string identifier, const Matrix4 projMatrix, const Vector2 resolution) : GraphicsModule(identifier, projMatrix, resolution)
{

	UIShader = new Shader(SHADERDIR"/UIVertex.glsl", SHADERDIR"/UIFrag.glsl");
	Button* uiTest = new Button();
	uiTest->UiMesh = new Mesh("../Data/Meshes/cube.obj", 1);
	uiTest->position = Vector2(0, 0);
	uiTest->scale = Vector2(10, 10);
	UIObjects.push_back(uiTest);
}

UIModule::~UIModule()
{
}

void UIModule::initialise()
{
}

void UIModule::apply()
{
	setCurrentShader(UIShader);
	glDisable(GL_DEPTH_BUFFER);
	viewMatrix.toIdentity();
	updateShaderMatrices();

	for (Button * uiObject : UIObjects)
	{
		uiObject->UiMesh->Draw(*currentShader,Matrix4::translation(uiObject->position) * Matrix4::scale(uiObject->scale));
	}
	glEnable(GL_DEPTH_BUFFER);
}

void UIModule::linkShaders()
{
	UIShader->LinkProgram();
}

void UIModule::regenerateShaders()
{
}

void UIModule::locateUniforms()
{
}
