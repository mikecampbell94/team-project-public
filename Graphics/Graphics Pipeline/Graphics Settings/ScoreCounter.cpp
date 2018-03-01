#include "ScoreCounter.h"

#include "../../GraphicsCommon.h"
#include "../../GraphicsUtility.h"

ScoreCounter::ScoreCounter(const std::string identifier, const Matrix4 projmatrix, const Vector2 resolution)
	: GraphicsModule(identifier, projmatrix, resolution)
{
	computeShader = new ComputeShader(SHADERDIR"/Compute/compute.glsl", true);
}

ScoreCounter::~ScoreCounter()
{
	delete computeShader;
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
	scores.red = 0;
	playerScoresSSBO = GraphicsUtility::InitSSBO(1, 4, playerScoresSSBO, sizeof(PlayerScores), &scores, GL_STATIC_COPY);


	//glGenBuffers(1, &redCounter);
	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, redCounter);
	//glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
	//glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, redCounter);

	//glGenBuffers(1, &yellowCounter);
	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, yellowCounter);
	//glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 1);
	//glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, yellowCounter);
}

void ScoreCounter::apply()
{
	GLuint zero = 0;

	scores.red = 0;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, playerScoresSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(PlayerScores),
		&scores, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, playerScoresSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, redCounter);
	//glInvalidateBufferData(redCounter);
	//glClearBufferData(GL_ATOMIC_COUNTER_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &zero);
	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, yellowCounter);
	//glInvalidateBufferData(yellowCounter);
	//glClearBufferData(GL_ATOMIC_COUNTER_BUFFER, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, &zero);
	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 1);

	computeShader->UseProgram();

	glUniform1i(glGetUniformLocation(computeShader->GetProgram(), "paintTrailTexture"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, *paintTrailTexture);
	//currentShader->ApplyTexture(6, *paintTrailTexture);

	computeShader->Compute(Vector3(1280, 720, 1));
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, playerScoresSSBO);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	memcpy(&scores, p, sizeof(PlayerScores));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	std::cout << scores.red << std::endl;
}

void ScoreCounter::locateUniforms()
{
}
