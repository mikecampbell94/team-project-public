#include "GameText.h"

#include "../../GraphicsCommon.h"
#include "../Graphics/Utility/Camera.h"

GameText::GameText(const std::string identifier, const Matrix4 orthographicMatrix, Matrix4 perspectiveMatrix,
	const Vector2 resolution, Camera* camera)
	: GraphicsModule(identifier, perspectiveMatrix, resolution)
{
	font = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
	tex = SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	UIShader = new Shader(SHADERDIR"/UIVertex.glsl", SHADERDIR"/UIFrag.glsl");
	UITextShader = new Shader(SHADERDIR"UITextVertex.glsl", SHADERDIR"UITextFrag.glsl");

	this->camera = camera;
	this->orthographicMatrix = orthographicMatrix;
	this->perspectiveMatrix = perspectiveMatrix;
}


GameText::~GameText()
{
}

void GameText::initialise()
{
}

void GameText::apply()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//viewMatrix.toIdentity();
	textureMatrix.toIdentity();


	setCurrentShader(UITextShader);
	updateShaderMatrices();

	for (int i = 0; i < bufferedText.size(); ++i)
	{
		if (bufferedOrthographicUsage[i] == true)
		{
			viewMatrix.toIdentity();
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&orthographicMatrix);
		}
		else
		{
			viewMatrix = camera->buildViewMatrix();
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&perspectiveMatrix);
		}

		TextMesh textMesh(bufferedText[i], *font);
		textMesh.Draw(*currentShader, Matrix4::translation(bufferedPositions[i]) * Matrix4::scale(Vector3(bufferedScales[i].x, bufferedScales[i].y, 1)));
	}

	bufferedText.clear();
	bufferedPositions.clear();
	bufferedScales.clear();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void GameText::linkShaders()
{
	UIShader->LinkProgram();
	UITextShader->LinkProgram();
}

void GameText::regenerateShaders()
{
	UIShader->Regenerate();
}

void GameText::bufferText(std::string text, Vector3 position, Vector3 scale, bool orthographic)
{
	bufferedText.push_back(text);
	bufferedPositions.push_back(position);
	bufferedScales.push_back(scale);
	bufferedOrthographicUsage.push_back(orthographic);
}

void GameText::locateUniforms()
{
}
