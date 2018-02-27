#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine()
{
}

FiniteStateMachine::~FiniteStateMachine()
{
}

void FiniteStateMachine::compileParsedXMLIntoFSM(Node* node)
{
	logic.compileParsedXMLIntoScript(node);
	logic.executeActionsOnStart();
}