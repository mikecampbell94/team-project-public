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
	UITextShader = new Shader(SHADERDIR"UITextVertex.glsl", SHADERDIR"UITextFrag.glsl", "", true);

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
			bufferedScales[i].x = -bufferedScales[i].x;
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&perspectiveMatrix);
		}

		glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "colour"), 1, (float*)&bufferedColours[i]);

		TextMesh textMesh(bufferedText[i], *font);
		textMesh.Draw(*currentShader, Matrix4::translation(bufferedPositions[i]) * Matrix4::scale(Vector3(bufferedScales[i].x, bufferedScales[i].y, 1)));
	}

	bufferedText.clear();
	bufferedPositions.clear();
	bufferedScales.clear();
	bufferedOrthographicUsage.clear();

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

void GameText::bufferText(std::string text, Vector3 position, Vector3 scale, Vector3 colour, bool orthographic)
{
	bufferedText.push_back(text);
	bufferedPositions.push_back(position);
	bufferedScales.push_back(scale);
	bufferedColours.push_back(colour);
	bufferedOrthographicUsage.push_back(orthographic);
}

void GameText::locateUniforms()
{
}
