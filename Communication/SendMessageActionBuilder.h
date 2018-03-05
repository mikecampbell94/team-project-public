#pragma once

#include <functional>
#include <unordered_map>

struct Node;

typedef std::function<void()> Executable;
typedef std::function<Executable(Node* node)> Builder;

class SendMessageActionBuilder
{
public:
	static void initialiseBuilders();
	static Executable buildSendMessageAction(Node* node);

private:
	static std::unordered_map<std::string, Builder> builders;
};

