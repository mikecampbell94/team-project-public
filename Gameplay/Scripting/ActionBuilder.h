#pragma once

#include <functional>

class Message;
struct Node;

typedef std::function<void(Message)> GameplayAction;
typedef std::function<bool(Message)> Condition;
typedef std::function<void()> Executable;

class ActionBuilder
{
public:
	static GameplayAction buildAction(Node* node);

private:
	static Executable buildSendMessageExecutable(Node* node);
};

