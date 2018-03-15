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
		loadModel(path);

		file = path;

		setSize(sizeof(*this));
	}

	Mesh(const string path, int numModels)
	{
		this->numModels = numModels;
		loadModel(path);

		file = path;
		
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

	void loadModel(std::string path);
	void setupMesh()
	{
		for (SubMesh* submesh : meshes)
		{
			submesh->setupMesh();
		}

		if (textureFile != "")
		{
			loadTexture(textureFile);
		}
	}
	void setTransformForAllSubMeshes(NCLMatrix4 transform);

	void processNode(aiNode *node, const aiScene *scene);
	SubMesh* processMesh(aiMesh *mesh, const aiScene *scene);


	static Mesh* generateHeightMap(int width, int height);

	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);

	static unsigned int textureFromFile(const char *path, const string &directory);

	void setReflectionAttributesForAllSubMeshes(int isReflective, float strength);
	void setbackupColourAttributeForAllSubMeshes(NCLVector4 colour);

	virtual void draw(Shader& shader,NCLMatrix4 worldTransform);

	float getRadius()
	{
		return this->meshes[0]->getBoundingRadius();
	}
	

	void loadTexture(std::string filepath);
	void setTextureFile(std::string textureFile)
	{
		this->textureFile = textureFile;
	}

	//Model Data 
	std::vector<SubMesh*> meshes;
	Mesh* mesh;
	unordered_map<string, SubMesh*> meshesByName;
	std::string directory;
	std::string file;
	std::string textureFile;
	std::vector<Texture> loadedTextures;

	Assimp::Importer import;
	const aiScene* scene;

	int hasTexture = 0;
	int numModels;

	float radius;

	bool perlin = false;
};

