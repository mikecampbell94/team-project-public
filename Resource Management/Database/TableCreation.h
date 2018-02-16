#pragma once

#include <functional>
#include <vector>
#include "Database.h"

class TableCreation
{
public:
	explicit TableCreation(Database* database);
	~TableCreation();

	void addTablesToDatabase() const;

private:
	void addGameObject() const;
	void addSceneNode() const;
	void addPhysicsObject() const;
	void addMesh() const;
	void addSounds() const;

	std::vector<std::function<void()>> tableAdditions;
	Database* database;
};

