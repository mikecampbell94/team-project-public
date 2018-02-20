#include "UIModule.h"

#include "../../GraphicsCommon.h"
#include "../Resource Management/Database/Database.h"
#include "UserInterfaceDisplay.h"

UIModule::UIModule(const std::string identifier, const Matrix4 projMatrix, const Vector2 resolution,
	Database* database) : GraphicsModule(identifier, projMatrix, resolution)
{
	font = new Font(SOIL_load_OGL_texture(SHADERDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	tex = SOIL_load_OGL_texture(SHADERDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	UIShader = new Shader(SHADERDIR"/UIVertex.glsl", SHADERDIR"/UIFrag.glsl");
	UITextShader = new Shader(SHADERDIR"UITextVertex.glsl", SHADERDIR"UITextFrag.glsl");
	Button* uiTest = new Button();
	uiTest->UiMesh = new Mesh("../Data/Meshes/cube.obj", 1);
	uiTest->position = Vector2(0, 0);
	uiTest->scale = Vector2(10, 10);
	UIObjects.push_back(uiTest);
	this->database = database;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

UIModule::~UIModule()
{
}

void UIModule::initialise()
{
	UIObjects = *UserInterfaceDisplay::getInterface()->getAllButtonsInMenu();//UserInterfaceBuilder::buildButtons("../Data/UserInterface/MainMenu.xml", database);
}

void UIModule::apply()
{
	setCurrentShader(UITextShader);
	glDisable(GL_DEPTH_TEST);

	viewMatrix.toIdentity();
	textureMatrix.toIdentity();

	updateShaderMatrices();

	for (Button * uiObject : UIObjects)
	{
		TextMesh* textmsh = new TextMesh("TESTING", *font);
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "colour"), 1, (float*)&button->colour);
		button->UIMesh->Draw(*currentShader, Matrix4::translation(button->position) * Matrix4::scale(button->scale));

		uiObject->UiMesh->Draw(*currentShader,Matrix4::translation(uiObject->position) * Matrix4::scale(uiObject->scale));
	}





	glEnable(GL_DEPTH_TEST);
}

void UIModule::linkShaders()
{
	UIShader->LinkProgram();
	UITextShader->LinkProgram();
}

void UIModule::regenerateShaders()
{
	UIShader->Regenerate();
}

void UIModule::locateUniforms()
{
}
