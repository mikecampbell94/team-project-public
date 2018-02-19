#include "UIModule.h"
#include "../../GraphicsCommon.h"





UIModule::UIModule(const std::string identifier, const Matrix4 projMatrix, const Vector2 resolution) : GraphicsModule(identifier, projMatrix, resolution)
{
	font = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	UIShader = new Shader(SHADERDIR"/UIVertex.glsl", SHADERDIR"/UIFrag.glsl");
	UITextShader = new Shader(SHADERDIR"/UITextVertex.glsl", SHADERDIR"/UITextFrag.glsl");
	Button* uiTest = new Button();	
	uiTest->UiMesh = new Mesh("../Data/Meshes/cube.obj", 1);
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
	Vector3 pos = Vector3(0, 0, 0);
	setCurrentShader(UITextShader);
	glDisable(GL_DEPTH_TEST);

	viewMatrix.toIdentity();
	//projMatrix = Matrix4::orthographic(-1.0f, 1.0f, 1280.0f, 0.0f, 720.0f, 0.0f);

	//modelMatrix = Matrix4::translation(Vector3(pos.x, 720.0f - pos.y, pos.z)) * Matrix4::scale(Vector3(16, 16, 1));


	updateShaderMatrices();

	Matrix4 exampleModel;
	exampleModel.setScalingVector(Vector3(100, 100, 100));

	for (Button * uiObject : UIObjects)
	{
		//uiObject->UiMesh->Draw(*currentShader,Matrix4::translation(uiObject->position) * Matrix4::scale(uiObject->scale));
		if (uiObject->text.text != "")
		{
			TextMesh* textmsh = new TextMesh("blah", *font);
			LegacyMesh* test = LegacyMesh::GenerateQuad();
			
			
			//test->Draw(*currentShader, exampleModel);

			textmsh->Draw(*currentShader,Matrix4::translation(Vector3(0, 0, 0)) * Matrix4::scale(Vector3(16, 16, 1)));
			
			delete textmsh;
			//delete test;
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
