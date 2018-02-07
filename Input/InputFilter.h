#pragma once
#include <vector>
#include <string>
#include <map>

class InputFilter
{
public:
	InputFilter();
	~InputFilter();

	std::string loadConfigFile();

	std::vector<int> getListenedKeys(std::string &configInfo, std::string &separator);

private:
	static inline int getKeyID(std::string key) { return keyValue.at(key); }

	static const std::map<std::string, int> keyValue;
};