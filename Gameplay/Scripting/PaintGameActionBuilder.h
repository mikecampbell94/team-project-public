#pragma once

#include <functional>
#include <unordered_map>

class Database;
struct Node;

typedef std::function<void()> Executable;
typedef std::function<Executable(Node*)> Builder;
typedef std::function<Executable(std::vector<std::string>)> DevConsoleNodeBuilder;

class PaintGameActionBuilder
{
public:
	static void initialiseBuilders();
	static Executable buildExecutable(Node* node);

private:
	static std::unordered_map<std::string, Builder> builders;
	Database* database;
};

