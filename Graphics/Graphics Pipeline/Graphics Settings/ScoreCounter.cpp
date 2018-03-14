#include "ScoreCounter.h"

#include "../../GraphicsCommon.h"
#include "../../GraphicsUtility.h"
#include "../../../Communication/DeliverySystem.h"
#include "../../Resource Management/Database/Database.h"
#include "../../Gameplay/GameObject.h"

#include <math.h>
#include "../../../Gameplay/Scripting/PaintGameActionBuilder.h"

ScoreCounter::ScoreCounter(const std::string identifier, const NCLVector2 resolution, Database* database)
	: GraphicsModule(identifier, resolution)
{
	computeShader = new ComputeShader(SHADERDIR"/Compute/compute.glsl", true);
	textShader = new Shader(SHADERDIR"UITextVertex.glsl", SHADERDIR"/ScoreText/textFrag.glsl", "", true);
	this->database = database;

	font = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);
}

ScoreCounter::~ScoreCounter()
{
	delete computeShader;
	delete textShader;
}

void ScoreCounter::bufferScoreHolder(std::string scoreHoldername)
{
	const NCLVector4 scoreHolderColour = static_cast<GameObject*>(
		database->getTable("GameObjects")->getResource(scoreHoldername))->getSceneNode()->getColour();

	scoreHolders.push_back(scoreHoldername);
	coloursToCount.push_back(scoreHolderColour);
	scores.push_back(0);
}

void ScoreCounter::linkShaders()
{
	computeShader->LinkProgram();
	textShader->LinkProgram();
}

void ScoreCounter::regenerateShaders()
{
}

void ScoreCounter::initialise()
{
	playerScoresSSBO = GraphicsUtility::InitSSBO(1, 4, playerScoresSSBO, sizeof(int), &scores, GL_STATIC_COPY);
	coloursSSBO = GraphicsUtility::InitSSBO(1, 5, coloursSSBO, sizeof(NCLVector4), &coloursToCount, GL_STATIC_COPY);
}

void ScoreCounter::apply()
{
	elapsedTime += timer.getTimeSinceLastRetrieval();

	if (elapsedTime >= 30.0f)
	{
		calculateScores();
		elapsedTime = 0;
	}
	
	displayScores();
}

void ScoreCounter::calculateScores()
{
	GLuint zero = 0;

	for (int i = 0; i < scoreHolders.size(); ++i)
	{
		scores[i] = 0;
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, playerScoresSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * scores.size(),
		&scores[0], GL_STATIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, coloursSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(NCLVector4) * coloursToCount.size(),
		&coloursToCount[0], GL_STATIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	computeShader->UseProgram();

	glUniform1i(glGetUniformLocation(computeShader->GetProgram(), "arraySize"), coloursToCount.size());
	glUniform1i(glGetUniformLocation(computeShader->GetProgram(), "paintTrailTexture"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, *paintTrailTexture);

	computeShader->Compute(NCLVector3(64, 36, 1));
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, playerScoresSSBO);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	memcpy(&scores[0], p, sizeof(int) * scores.size());
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ScoreCounter::displayScores()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	textureMatrix.toIdentity();

	setCurrentShader(textShader);
	updateShaderMatrices();

	for (int i = 0; i < scoreHolders.size(); ++i)
	{
		viewMatrix.toIdentity();
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&CommonGraphicsData::SHARED_ORTHOGRAPHIC_MATRIX);

		NCLVector3 colour(coloursToCount[i].x, coloursToCount[i].y, coloursToCount[i].z);
		glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "colour"), 1, (float*)&colour);


		float percentage = scores[i] / 15000.0f;
		int score = int(percentage * 100.0f);

		std::string name = scoreHolders[i] + " : " + std::to_string(score);

		int numDigits = score > 0 ? (int)log10((double)score) + 1 : 1;
		int numSpacesToAdd = 5 - numDigits;
		for (int j = 0; j < numSpacesToAdd; ++j)
		{
			name += " ";
		}

		TextMesh textMesh(name, *font);
		textMesh.Draw(*currentShader, NCLMatrix4::translation(NCLVector3(290, (i * -20.0f) + 320, 0)) * NCLMatrix4::scale(NCLVector3(20, 20, 1)));

		if (PaintGameActionBuilder::online)
		{
			DeliverySystem::getPostman()->insertMessage(TextMessage("NetworkClient", "sendscore " + to_string(i) + " " + to_string(score)));
		}

		DeliverySystem::getPostman()->insertMessage(TextMessage("Gameplay", "sendscore " + to_string(i) + " " + to_string(score)));
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void ScoreCounter::locateUniforms()
{
}
