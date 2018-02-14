#pragma once

#include <functional>
#include <vector>
#include "Database.h"
#include "../GameObjectBuilder.h"

class TableCreation
{
public:
	explicit TableCreation(Database* database);
	~TableCreation();

	void addTablesToDatabase() const;

private:
	void addGameObject() const;
	void addMesh() const;


	std::vector<std::function<void()>> tableAdditions;
	Database* database;
};

