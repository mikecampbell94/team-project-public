#pragma once

#include <functional>
#include <unordered_map>

struct Node;
typedef std::function<void()> Executable;

class PaintingGameMechanics
{
public:
	PaintingGameMechanics();
	~PaintingGameMechanics();

	Executable buildAction(Node* node);

private:
	std::unordered_map<std::string, Executable> mechanics;
};

