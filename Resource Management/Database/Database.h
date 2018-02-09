#pragma once

#include "../Resource.h"
#include "Table.h"

#include <string>

class Database
{
public:
	Database();
	~Database();

	void AddTable(const std::string tableName, Table<Resource>* table)
	{
		tables[tableName] = table;
	}

	unordered_map <string, Table<Resource>*> tables;
};

