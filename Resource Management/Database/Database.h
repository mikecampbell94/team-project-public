#pragma once

#include "../Resource.h"
#include "Table.h"

#include <string>
#include <map>

class Database
{
public:
	Database();
	~Database();

	void addTable(const std::string tableName, Table<Resource>* table);
	Table<Resource>* getTable(const std::string tableName);

	void addResourceToTable(const std::string tableName, Resource* resource);
	void addResourceToTable(const std::string tableName, Node* node);

private:
	std::map<std::string, Table<Resource>*> tables;
};

