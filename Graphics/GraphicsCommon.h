#pragma once

#define SHADERDIR	"../Shaders/"
#define MESHDIR		"../Data/Meshes/"
#define TEXTUREDIR  "../Data/Textures/"
#define SOUNDSDIR	"../Data/Sounds/"

#include "Maths/Matrix4.h"

class CommonGraphicsData
{
public:
	enum GBUFFER
	{
		GPOSITION,
		GNORMAL,
		GALBEDO
	};

	enum AMBIENT_TEX
	{
		SSAO_INDEX
	};

	static Matrix4 SHARED_PROJECTION_MATRIX;
	static Matrix4 SHARED_ORTHOGRAPHIC_MATRIX;
};