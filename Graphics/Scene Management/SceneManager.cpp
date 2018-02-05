#include "SceneManager.h"

SceneManager::SceneManager(std::vector<SceneNode*>* sceneNodes)
{
	this->sceneNodes = sceneNodes;

	meshes = new vector<Mesh*>();
	subMeshesInFrustum = new vector<SubMesh*>();
	transparentSubMeshesInFrustum = new vector<SubMesh*>();

	for each (SceneNode* node in *sceneNodes)
	{
		meshes->push_back(node->GetMesh());
	}
}

SceneManager::~SceneManager()
{
	delete meshes;
	delete subMeshesInFrustum;
	delete transparentSubMeshesInFrustum;
}

void SceneManager::ClearMeshLists()
{
	subMeshesInFrustum->clear();
	transparentSubMeshesInFrustum->clear();
}

void SceneManager::BuildMeshLists()
{
	//TODO
	for each (SceneNode* node in *sceneNodes)
	{
		node->Update(0.0f);

		for each (SubMesh* subMesh in node->GetMesh()->meshes)
		{
			if (subMesh->baseColour.w < 1.0f)
			{
				transparentSubMeshesInFrustum->push_back(subMesh);
			}
			else
			{
				subMeshesInFrustum->push_back(subMesh);
			}
		}
	}
}

std::vector<SubMesh*>* SceneManager::getSubMeshesInFrustum()
{
	return subMeshesInFrustum;
}

std::vector<SubMesh*>* SceneManager::getTransparentSubMeshesInFrustum()
{
	return transparentSubMeshesInFrustum;
}

std::vector<Mesh*>** SceneManager::getAllMeshes()
{
	return &meshes;
}
