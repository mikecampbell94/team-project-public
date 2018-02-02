#pragma once
#include <unordered_map>
#include <List>
#include "InputDevice.h"

typedef unsigned VirtualKey;

class VirtualKeySet
{
public:
	VirtualKeySet();
	~VirtualKeySet();

	//GETS
	inline int getNumOfVirtualKeys() {};

	inline std::list<int> getBindedKeysByVirtualKey(VirtualKey vk) {};

	bool getVirtualKeyState(VirtualKey vk);

	void pollAllVirtualKeys(void(*virtualKeyCallback)(VirtualKey vk, bool state)) {};

	//BINDS
	inline void bindVirtualKeyWithKey(VirtualKey vk, int key) {};

	inline void bindVirtualKeyWithKeys(VirtualKey vk, std::list<int> keys) {};

	//SETS
	VirtualKey createVirtualKey();//Generate an unused index as a new virtual key

	void loadConfiguration();//Set bindings by configuration file

private:
	int numOfVirtualKeys;

	std::unordered_map<VirtualKey, std::list<int>> virtualKeyBindings;
};
