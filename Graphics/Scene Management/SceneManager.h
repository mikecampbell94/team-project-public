#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "SOIL.lib")
#include "SceneNode.h"
#include "../Meshes/SubMesh.h"

#include <vector>

class SceneManager
{
public:
	explicit SceneManager(std::vector<SceneNode*>* sceneNodes);
	~SceneManager();

	void ClearMeshLists();
	void BuildMeshLists();

	std::vector<SubMesh*>* getSubMeshesInFrustum();
	std::vector<SubMesh*>* getTransparentSubMeshesInFrustum();
	std::vector<Mesh*>** getAllMeshes();

private:
	std::vector<Mesh*>* meshes;
	std::vector<SceneNode*>* sceneNodes;
	std::vector<SubMesh*>* subMeshesInFrustum;
	std::vector<SubMesh*>* transparentSubMeshesInFrustum;
};

