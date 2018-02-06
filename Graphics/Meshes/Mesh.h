#pragma once

#pragma comment(lib, "assimp-vc140-mt.lib")

#include "SubMesh.h"

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include <iostream>
#include <string>
#include <vector>

//#include <Simple OpenGL Image Library\src\stb_image_aug.h>
#include <unordered_map>

class Mesh
{
public:
	Mesh(char *path, int numModels)
	{
		this->numModels = numModels;
		LoadModel(path);
	}

	Mesh(const string path, int numModels)
	{
		this->numModels = numModels;
		LoadModel(path);
	}

	~Mesh()
	{
		for each (SubMesh* mesh in meshes)
		{
			delete mesh;
		}
	};

	void LoadModel(std::string path);

	void SetTransformForAllSubMeshes(Matrix4 transform);

	void ProcessNode(aiNode *node, const aiScene *scene);
	SubMesh* ProcessMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);

	static unsigned int TextureFromFile(const char *path, const string &directory);

	void SetReflectionAttributesForAllSubMeshes(int isReflective, float strength);
	void SetbackupColourAttributeForAllSubMeshes(Vector4 colour);

	//Model Data 
	std::vector<SubMesh*> meshes;
	unordered_map<string, SubMesh*> meshesByName;
	std::string directory;
	std::vector<Texture> loadedTextures;

	Assimp::Importer import;
	const aiScene* scene;

	int numModels;
};

