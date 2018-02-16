#include "UIModule.h"
#include "../../GraphicsCommon.h"





UIModule::UIModule(const std::string identifier, const Matrix4 projMatrix, const Vector2 resolution) : GraphicsModule(identifier, projMatrix, resolution)
{
	font = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	UIShader = new Shader(SHADERDIR"/UIVertex.glsl", SHADERDIR"/UIFrag.glsl");
	Button* uiTest = new Button();
UITextShader = new Shader(SHADERDIR"/TexturedVertex.glsl", SHADERDIR"/TexturedFrag.glsl");
	UIObject* uiTest = new UIObject();	uiTest->UiMesh = new Mesh("../Data/Meshes/cube.obj", 1);
	uiTest->position = Vector2(0, -200);
	uiTest->scale = Vector2(30, 10);
	uiTest->text = Text("TestUI",{0,0,0},10);
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
		//uiObject->UiMesh->Draw(*currentShader,Matrix4::translation(uiObject->position) * Matrix4::scale(uiObject->scale));
		if (uiObject->text.text != "")
		{
			TextMesh textmsh(uiObject->text.text, *font);
			
			textmsh.Draw();
		}
	}

	for (UIObject * uiObject : UIObjects)
	{

	}



	glEnable(GL_DEPTH_BUFFER);
}

void UIModule::linkShaders()
{
	UIShader->LinkProgram();
	UITextShader->LinkProgram();
}

void UIModule::regenerateShaders()
{
}

void UIModule::locateUniforms()
{
}
