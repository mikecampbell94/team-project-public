#include "SceneNode.h"

SceneNode::SceneNode(string meshFile, Vector4 colour)
{
	this->mesh = new Mesh(meshFile, 1);
	this->colour = colour;
	parent = NULL;

	boundingRadius = 10.0f;
	distanceFromCamera = 0.0f;
}

SceneNode::SceneNode(Mesh * mesh, Vector4 colour)
{
	this->mesh = mesh;
	this->colour = colour;
	parent = NULL;

	boundingRadius = 10.0f;
	distanceFromCamera = 0.0f;
}

SceneNode::~SceneNode(void)
{
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->parent = this;
}

void SceneNode::RemoveChild(SceneNode* s) {
	children.erase(std::remove(children.begin(),
		children.end(), s), children.end());
}

/*
* Advanced rendering can be performed here per SceneNode
* Such as disabling depth testing or reuploading data
*/
void SceneNode::Draw(Shader& shader) {
	if (mesh) {
		for each (SubMesh* submesh in mesh->meshes)
		{
			submesh->Draw(shader, worldTransform);
		}
	}
}

void SceneNode::DrawShadow()
{
	if (mesh) {
		for each (SubMesh* submesh in mesh->meshes)
		{
			submesh->DrawShadow();
		}
	}
}

void SceneNode::Update(float msec) {
	if (parent) {
		worldTransform = parent->worldTransform * transform;
	}
	else {
		worldTransform = transform;
	}

	mesh->SetTransformForAllSubMeshes(worldTransform);

	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}
