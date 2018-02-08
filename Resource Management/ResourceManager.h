#pragma once
#include <map>
#include <algorithm>


template<class T>
class ResourceManager
{
public:
	ResourceManager(unsigned int upperbound);
	~ResourceManager();

	bool addResource(std::string identifier,T* resource);

	T* getResource(std::string identifier);

	void deleteResource(std::string identifier, T * resource);

protected:
	unordered_map<std::string,T*> resourceBuffer;
	size_t maxSize;
	size_t currentSize;
	std::string StringUtility(std::string identifier);
};