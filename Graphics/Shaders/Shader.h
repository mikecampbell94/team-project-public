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

using namespace std;

class Shader
{
public:
	Shader(string vertex, string fragment,
		string geometry = "", bool isVerbose = false);
	~Shader(void);

	GLuint	GetProgram() { return program; }
	bool	LinkProgram();

protected:
	void	SetDefaultAttributes();
	bool	LoadShaderFile(string from, string & into);
	GLuint	GenerateShader(string from, GLenum type);

	GLuint	objects[3];
	GLuint	program;

	bool	loadFailed;
	bool	verbose; //I added this stuff (Kiya)
};
