#include "ObjectBuilder.h"



ObjectBuilder::ObjectBuilder()
{

	addBuilder("RenderObject", [](Node* node, Database* database) {
		//break node down
		string iD = node->name;
		//dont forget extract enabled/disabled
		string mesh = node->children[1]->value;
		



		SceneNode* sceneNode;
		//Resource<SceneNode>* resource = new Resource<SceneNode>(*sceneNode);
		//database.addResource(resource);
	});

	addBuilder("GameObject", [](Node* node, Database* database) {
		GameObject* gameObject;
		//Resource<GameObject>* resource = new Resource<GameObject>(*gameObject);
		//database.addResource(resource);
	});

	addBuilder("PhysicsObject", [](Node* node, Database* database) {
		GameObject* gameObject;
		//Resource<GameObject>* resource = new Resource<GameObject>(*gameObject);
		//database.addResource(resource);
	});

	addBuilder("Mesh", [](Node* node, Database* database) {
		Mesh* mesh;
		//Resource<Mesh>* resource = new Resource<Mesh>(*mesh);
		//database.addResource(resource);
	});

}


ObjectBuilder::~ObjectBuilder()
{
}

void ObjectBuilder::addBuilder(std::string className, objectbuild buildFunction)
{
	builders.insert({ className,buildFunction});
}
