#pragma once

#pragma comment(lib, "assimp-vc140-mt.lib")

#include "SOIL.h"
#include "SubMesh.h"
#include "../../Resource Management/Resources/Resource.h"

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include <iostream>
#include <string>
#include <vector>

#include <unordered_map>
#include <Simple OpenGL Image Library\src\stb_image_aug.h>

class Mesh : public Resource
{
public:
	Mesh(char *path, int numModels)
	{
		this->numModels = numModels;
		LoadModel(path);

		setSize(sizeof(*this));
	}

	Mesh(const string path, int numModels)
	{
		this->numModels = numModels;
		LoadModel(path);
		
		setSize(sizeof(*this));
	}

	Mesh()
	{

	}

	~Mesh()
	{
		for (SubMesh* mesh : meshes)
		{
			delete mesh;
		}
	};

	void LoadModel(std::string path);

	void SetTransformForAllSubMeshes(NCLMatrix4 transform);

	void ProcessNode(aiNode *node, const aiScene *scene);
	SubMesh* ProcessMesh(aiMesh *mesh, const aiScene *scene);


	static Mesh* GenerateHeightMap(int width, int height);

	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);

	static unsigned int TextureFromFile(const char *path, const string &directory);

	void SetReflectionAttributesForAllSubMeshes(int isReflective, float strength);
	void SetbackupColourAttributeForAllSubMeshes(NCLVector4 colour);

	virtual void Draw(Shader& shader,NCLMatrix4 worldTransform);

	float getRadius()
	{
		return this->meshes[0]->GetBoundingRadius();
	}
	

	void loadTexture(std::string filepath);

	//Model Data 
	std::vector<SubMesh*> meshes;
	Mesh* mesh;
	unordered_map<string, SubMesh*> meshesByName;
	std::string directory;
	std::vector<Texture> loadedTextures;

	Assimp::Importer import;
	const aiScene* scene;

	int hasTexture = 0;
	int numModels;

	float radius;

	bool perlin = false;

};

