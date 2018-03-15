#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include "../Utilities/Maths/Vector3.h"

using namespace std;

class ComputeShader
{
public:
	ComputeShader(string compute, bool isVerbose = false);
	~ComputeShader();

	GLuint	getProgram() { return program; }
	void	useProgram();
	bool	linkProgram();

	void compute(NCLVector3 workGroups);

	void regenerate();
protected:
	bool	loadShaderFile(string from, string & into);
	string includeShader(string includeLine);
	GLuint	generateShader(string from);

	GLuint	object[1];
	GLuint	program;

	bool	loadFailed;
	bool	verbose; 

	string compute;
};

