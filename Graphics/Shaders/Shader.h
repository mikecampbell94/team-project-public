#pragma once

#define SHADER_VERTEX	0
#define SHADER_FRAGMENT 1
#define SHADER_GEOMETRY 2

#define POSITION_BUFFER 0
#define TEX_COORD_BUFFER 2

#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include "../Utilities/Maths/Vector3.h"

using namespace std;

class Shader
{
public:
	Shader(string vertex, string fragment,
		string geometry = "", bool isVerbose = false);
	~Shader(void);

	GLuint	getProgram() { return program; }
	bool	linkProgram();

	/*
	Recompile the shaders without having to restart.
	(Will still need linking)
	*/
	void regenerate();

	void applyTexture(int unit, unsigned int texture)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(program, name.c_str()), value);
	}

	void setVec3(const std::string &name, const NCLVector3 &value) const
	{
		glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, (float*)&value);
	}

	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
	}

protected:
	void	setDefaultAttributes();
	bool	loadShaderFile(string from, string & into);
	string includeShader(string includeLine);
	GLuint	generateShader(string from, GLenum type);

	GLuint	objects[3];
	GLuint	program;

	bool	loadFailed;
	bool	verbose; //I added this stuff (Kiya)

	string vFile;
	string fFile;
	string gFile;
};
