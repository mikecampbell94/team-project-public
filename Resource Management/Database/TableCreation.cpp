#include "TableCreation.h"

#include "Table.h"
#include "../Resource.h"

const size_t MAX_MEMORY_PER_TABLE = 5000;

TableCreation::TableCreation(DataBase* database)
{
	this->database = database;

	tableAdditions.push_back(std::bind(&TableCreation::addGameObject, this));
	tableAdditions.push_back(std::bind(&TableCreation::addSceneNode, this));
	tableAdditions.push_back(std::bind(&TableCreation::addPhysicsObject, this));
}

TableCreation::~TableCreation()
{
}

void TableCreation::addTablesToDatabase() const
{
	for each (auto additionFunction in tableAdditions)
	{
		additionFunction();
	}
}

void TableCreation::addGameObject() const
{
	database->AddTable("GameObject", new Table<Resource>(false, false, [](Node* node)
	{
		//Build object from node
		//Return it
		return nullptr;
	}, TableConfiguration("GProfiler", MAX_MEMORY_PER_TABLE)));
}

void TableCreation::addSceneNode() const
{
}

void TableCreation::addPhysicsObject() const
{
}
