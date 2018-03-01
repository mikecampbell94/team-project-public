#include "ScoreCounter.h"

#include "../../GraphicsCommon.h"
#include "../../GraphicsUtility.h"
#include "../../../Communication/DeliverySystem.h"
#include "../../Resource Management/Database/Database.h"
#include "../../Gameplay/GameObject.h"

ScoreCounter::ScoreCounter(const std::string identifier, const Matrix4 projmatrix, const Vector2 resolution, Database* database)
	: GraphicsModule(identifier, projmatrix, resolution)
{
	computeShader = new ComputeShader(SHADERDIR"/Compute/compute.glsl", true);
	this->database = database;

	//ScoreHolder scoreHolder;
	//scoreHolder.name = "poop";
	//scoreHolder.colour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	//bufferScoreHolder(scoreHolder);

	//ScoreHolder scoreHolder1;
	//scoreHolder1.name = "wee";
	//scoreHolder1.colour = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
	//bufferScoreHolder(scoreHolder1);
}

ScoreCounter::~ScoreCounter()
{
	delete computeShader;
}

void ScoreCounter::bufferScoreHolder(std::string scoreHoldername)
{
	scoreHolders.push_back(scoreHoldername);

	Vector4 scoreHolderColour = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(scoreHoldername))->getSceneNode()->getColour();
	coloursToCount.push_back(scoreHolderColour);
	scores.push_back(0);
}

void ScoreCounter::linkShaders()
{
	computeShader->LinkProgram();
}

void ScoreCounter::regenerateShaders()
{
}

void ScoreCounter::initialise()
{
	playerScoresSSBO = GraphicsUtility::InitSSBO(1, 4, playerScoresSSBO, sizeof(int), &scores, GL_STATIC_COPY);
	coloursSSBO = GraphicsUtility::InitSSBO(1, 5, coloursSSBO, sizeof(Vector4), &coloursToCount, GL_STATIC_COPY);
}

void ScoreCounter::apply()
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
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4) * coloursToCount.size(),
		&coloursToCount[0], GL_STATIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	computeShader->UseProgram();

	glUniform1i(glGetUniformLocation(computeShader->GetProgram(), "arraySize"), coloursToCount.size());
	glUniform1i(glGetUniformLocation(computeShader->GetProgram(), "paintTrailTexture"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, *paintTrailTexture);

	computeShader->Compute(Vector3(1280, 720, 1));
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, playerScoresSSBO);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	memcpy(&scores[0], p, sizeof(int) * scores.size());
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	for (int i = 0; i < scoreHolders.size(); ++i)
	{
		Vector3 colour(coloursToCount[i].x, coloursToCount[i].y, coloursToCount[i].z);
		DeliverySystem::getPostman()->insertMessage(TextMeshMessage("RenderingSystem", scoreHolders[i] + " : " + std::to_string(scores[i]),
			Vector3(0, i * 30.0f, 0), Vector3(30, 30, 1), colour, true));
	}
}

void ScoreCounter::locateUniforms()
{
}
