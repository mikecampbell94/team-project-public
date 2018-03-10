#pragma once

#include <functional>
#include <unordered_map>

class Database;
struct Node;

enum PowerUps
{
	SCALE_POWERUP,
	SPEED_POWERUP

};

typedef std::function<void()> Executable;
typedef std::function<Executable(Node*)> Builder;
typedef std::function<Executable(std::vector<std::string>)> DevConsoleNodeBuilder;

class PaintGameActionBuilder
{
public:
	static void initialiseBuilders(Database* database);
	static Executable buildExecutable(Node* node);

private:
	static std::unordered_map<std::string, Builder> builders;
	static std::string powerUpBuilders[2];
	static Database* database;
};

