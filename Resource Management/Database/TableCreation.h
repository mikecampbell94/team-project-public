#pragma once

#include <functional>
#include <vector>

class DataBase;

class TableCreation
{
public:
	explicit TableCreation(DataBase* database);
	~TableCreation();

	void addTablesToDatabase() const;

private:
	void addGameObject() const;
	void addSceneNode() const;
	void addPhysicsObject() const;

	std::vector<std::function<void()>> tableAdditions;
	DataBase* database;
};

