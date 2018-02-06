#pragma once
#include <vector>
#include <string>
#include <map>

class InputFliter
{
public:
	InputFliter();
	~InputFliter();

	std::string loadConfigFile();

	std::vector<int> getListenedKeys(std::string &configInfo, std::string &separator);

private:
	static inline int getKeyID(std::string key) { return keyValue.at(key); }

	static const std::map<std::string, int> keyValue;
};