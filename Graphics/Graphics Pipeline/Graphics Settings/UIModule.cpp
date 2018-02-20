#include "UIModule.h"
#include "../../GraphicsCommon.h"





UIModule::UIModule(const std::string identifier, const Matrix4 projMatrix, const Vector2 resolution) : GraphicsModule(identifier, projMatrix, resolution)
{
	font = new Font(SOIL_load_OGL_texture(SHADERDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	tex = SOIL_load_OGL_texture(SHADERDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	UIShader = new Shader(SHADERDIR"/UIVertex.glsl", SHADERDIR"/UIFrag.glsl");
	UITextShader = new Shader(SHADERDIR"UITextVertex.glsl", SHADERDIR"UITextFrag.glsl");
	Button* uiTest = new Button();	
	uiTest->UiMesh = new Mesh("../Data/Meshes/cube.obj", 1);
	uiTest->position = Vector2(0, -200);
	uiTest->scale = Vector2(30, 10);
	uiTest->text = Text("TestUI",{0,0,0},10);
	UIObjects.push_back(uiTest);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

UIModule::~UIModule()
{
}

void UIModule::initialise()
{
}

void UIModule::apply()
{
	setCurrentShader(UITextShader);
	glDisable(GL_DEPTH_TEST);

	viewMatrix.toIdentity();
	textureMatrix.toIdentity();

	updateShaderMatrices();

	Matrix4 exampleModel;
	exampleModel.setScalingVector(Vector3(16, 16, 1));
	exampleModel.setPositionVector(Vector3(-660, -100, 0));

	for (Button * uiObject : UIObjects)
	{
		uiObject->UiMesh->Draw(*currentShader,Matrix4::translation(uiObject->position) * Matrix4::scale(uiObject->scale));
		if (uiObject->text.text != "")
		{
			TextMesh* textmsh = new TextMesh("TESTING", *font);
		

			textmsh->Draw(*currentShader,exampleModel);
			
			delete textmsh;
		}
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
}

void UIModule::locateUniforms()
{
}
