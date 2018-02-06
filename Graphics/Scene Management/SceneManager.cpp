#include "SceneManager.h"

#include "../Utility/Camera.h"

SceneManager::SceneManager(Camera* camera, std::vector<SceneNode*>* sceneNodes)
{
	this->sceneNodes = sceneNodes;
	this->camera = camera;

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

void SceneManager::clearMeshLists()
{
	subMeshesInFrustum->clear();
	transparentSubMeshesInFrustum->clear();
}

void SceneManager::buildMeshLists()
{
	for each (SceneNode* node in *sceneNodes)
	{
		node->Update(0.0f);
		allocateSubMeshesToMeshLists(node);
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

void SceneManager::allocateSubMeshesToMeshLists(SceneNode* node)
{
	for each (SubMesh* subMesh in node->GetMesh()->meshes)
	{
		if (camera->subMeshIsInCameraView(subMesh))
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
