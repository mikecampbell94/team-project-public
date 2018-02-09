#pragma once
#include <unordered_map>
#include <algorithm>
#include "Resource.h"

template<class T>
class ResourceManager
{
public:
	ResourceManager(std::string id, size_t upperbound);
	~ResourceManager();

	void addResource(std::string identifier, Resource<T>* resource);
	Resource<T>* getResource(std::string identifier);
	void deleteResource(std::string identifier);

private:
	std::string resourceManagerID;
	size_t maxSize;
	size_t currentSize;
	unordered_map<std::string, Resource<T>*> resourceBuffer;
};