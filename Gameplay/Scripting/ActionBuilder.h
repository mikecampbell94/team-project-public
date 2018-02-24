#pragma once

#include <functional>
#include <vector>

class Message;
struct Node;

typedef std::function<void(Message)> GameplayAction;
typedef std::function<void(float&)> TimedGameplayAction;
typedef std::function<bool(Message)> Condition;
typedef std::function<void()> Executable;

class ActionBuilder
{
public:
	static GameplayAction buildAction(Node* node);
	static TimedGameplayAction buildTimedAction(Node* node);

	static void compileActionSection(Node* section, Condition& condition, std::vector<Executable>& executables);
	static Executable compileActionSectionWithoutCondition(Node* section);

private:
	static GameplayAction buildFinalActionWithCondition(Condition& condition, std::vector<Executable>& executables);
	static GameplayAction buildFinalAction(std::vector<Executable>& executables);

	static Condition buildIfStatement(Node* node);
	static Executable buildSendMessageExecutable(Node* node);
};

