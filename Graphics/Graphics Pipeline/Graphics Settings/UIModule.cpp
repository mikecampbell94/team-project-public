#include "UIModule.h"

#include "../../GraphicsCommon.h"
#include "../Resource Management/Database/Database.h"

UIModule::UIModule(const std::string identifier, const Matrix4 projMatrix, const Vector2 resolution,
	Database* database) : GraphicsModule(identifier, projMatrix, resolution)
{
	UIShader = new Shader(SHADERDIR"/UIVertex.glsl", SHADERDIR"/UIFrag.glsl");
	this->database = database;
}

UIModule::~UIModule()
{
}

void UIModule::initialise()
{
	UIObjects = UserInterfaceBuilder::buildButtons("../Data/UserInterface/MainMenu.xml", database, resolution);
	//UIObjects = UserInterfaceBuilder::buildButtons("", database);
}

void UIModule::apply()
{
	setCurrentShader(UIShader);
	glDisable(GL_DEPTH_BUFFER);
	viewMatrix.toIdentity();
	updateShaderMatrices();

	for (Button& button : UIObjects)
	{
		button.UIMesh->Draw(*currentShader,Matrix4::translation(button.position) * Matrix4::scale(button.scale));
	}
	glEnable(GL_DEPTH_BUFFER);
}

void UIModule::linkShaders()
{
	UIShader->LinkProgram();
}

void UIModule::regenerateShaders()
{
	UIShader->Regenerate();
}

void UIModule::locateUniforms()
{
}
