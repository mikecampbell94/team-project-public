#pragma once

#include "XMLParser.h"

class Database;

class XMLWriter
{
public:
	XMLWriter(Database* database);
	~XMLWriter();

	void saveLevelFile(std::string levelName);

private:
	void createFolder(const char* path);
	void saveMeshFile(std::string meshFileName);
	void saveLevelDetails(std::string levelDetailsFile);

	Database* database;
};

