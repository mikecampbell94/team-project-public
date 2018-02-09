#include "InputFilter.h"
#include "InputUtility.h"

InputFilter::InputFilter()
{

}

InputFilter::~InputFilter()
{

}

std::string InputFilter::loadConfigFile()
{
	return std::string();
}

std::vector<int> InputFilter::getListenedKeys(std::string &configInfo, std::string &separator)
{
	std::vector<int> listenedKeys;

	if (configInfo == "")
	{
		return listenedKeys;
	}

	std::string appendStr = configInfo + separator;

	size_t pos = appendStr.find(separator);
	size_t size = appendStr.size();

	while (pos != std::string::npos)
	{
		std::string t = appendStr.substr(0, pos);
		listenedKeys.push_back(InputUtility::getKeyID(t));
		appendStr = appendStr.substr(pos + 1, size);
		pos = appendStr.find(separator);
	}

	return listenedKeys;
}
