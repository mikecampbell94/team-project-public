#pragma once

#include <functional>
#include <string>
#include <map>
#include <vector>

class Database;

typedef std::function<void(std::vector<std::string>)> LevelEditorAction;

class LevelEditor
{
public:
	static void initialiseLevelEditor(Database* database);
	static void executeDevConsoleLine(std::string devConsoleLine);

private:
	static Database* database;
	static std::map<std::string, LevelEditorAction> actions;
};

