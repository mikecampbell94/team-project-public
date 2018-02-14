#include "SceneManager.h"

#include "../Utility/Camera.h"

SceneManager::SceneManager(Camera* camera, std::vector<SceneNode*>* sceneNodes)
{
	this->sceneNodes = sceneNodes;
	this->camera = camera;

	sceneNodes = new vector<SceneNode*>();
	sceneNodesInFrustum = new vector<SceneNode*>();
	transparentSceneNodesInFrustum = new vector<SceneNode*>();

	for each (SceneNode* node in *sceneNodes)
	{
		sceneNodes->push_back(node);
	}
}

SceneManager::~SceneManager()
{
	delete sceneNodes;
	delete sceneNodesInFrustum;
	delete transparentSceneNodesInFrustum;
}

void SceneManager::clearMeshLists()
{
	sceneNodesInFrustum->clear();
	transparentSceneNodesInFrustum->clear();
}

void SceneManager::buildMeshLists()
{
	for each (SceneNode* node in *sceneNodes)
	{
		node->Update(0.0f);
		allocateSubNodesToNodeLists(node);
	}
}

std::vector<SceneNode*>* SceneManager::getSceneNodesInFrustum()
{
	return sceneNodesInFrustum;
}

std::vector<SceneNode*>* SceneManager::getTransparentSceneNodesInFrustum()
{
	return transparentSceneNodesInFrustum;
}

std::vector<SceneNode*>** SceneManager::getAllNodes()
{
	return &sceneNodes;
}

void SceneManager::allocateSubNodesToNodeLists(SceneNode* node)
{
	if (camera->sceneNodeIsInCameraView(node))
	{
		if (node->getColour().w < 1.0f)
		{
			transparentSceneNodesInFrustum->push_back(node);
		}
		else
		{
			sceneNodesInFrustum->push_back(node);
		}
	}
}
