#pragma once
#include <vector>
#include <string>
#include <map>

class InputFliter
{
public:
	InputFliter();
	~InputFliter();

	std::string LoadConfigFile();

	std::vector<int> GetListenedKeys(std::string &configInfo, std::string &separator);

private:
	static inline int GetKeyID(std::string key) { return keyValue.at(key); }

	static const std::map<std::string, int> keyValue;
};