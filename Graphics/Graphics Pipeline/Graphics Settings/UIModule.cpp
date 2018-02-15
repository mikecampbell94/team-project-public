#include "UIModule.h"
#include "../../GraphicsCommon.h"





UIModule::UIModule(const std::string identifier, const Matrix4 projMatrix, const Vector2 resolution) : GraphicsModule(identifier, projMatrix, resolution)
{

	currentShader = new Shader(SHADERDIR"/basicVertex.glsl", SHADERDIR"/basicFrag.glsl");
	
	UIObject* uiTest = new UIObject();
	uiTest->UiMesh = new Mesh("../Data/Meshes/cube.obj", 1);
	uiTest->position = Vector2(100, 100);


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
	glDisable(GL_DEPTH_BUFFER);
	updateShaderMatrices();

	for (UIObject * uiObject : UIObjects)
	{
		//uiObject->UiMesh->Draw(*currentShader,Matrix4::translation(uiObject->position) * Matrix4::translation(uiObject->scale));
	}

}

void UIModule::linkShaders()
{
}

void UIModule::regenerateShaders()
{
}

void UIModule::locateUniforms()
{
}
