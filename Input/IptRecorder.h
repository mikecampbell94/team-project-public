#pragma once
#include <vector>

//TODO: Create this class
class playerBase;

enum KeyUsage {
	TRIGGERED,HELD,RELEASED
};

struct KeyData {
	int keyIdentifier;
	KeyUsage keyState;
};

class IptRecorder
{
public:
	IptRecorder(std::vector<int> keysToListen) 
	{
		this->keysToListen = keysToListen; 
	};
	
	~IptRecorder() {};


	virtual void fillInputs() = 0;
	virtual void clearInputs() = 0;

	std::vector<KeyData> inputBuffer;

protected:
	std::vector<int> keysToListen;

	playerBase* playerAssignedTo;
};

