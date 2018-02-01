#pragma once

#define SHADERDIR	"../Data/Shaders/"
#define MESHDIR		"../Data/Meshes/"
#define TEXTUREDIR  "../Data/Textures/"
#define SOUNDSDIR	"../Data/Sounds/"

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
};