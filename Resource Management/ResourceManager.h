#pragma once
#include <map>
#include <algorithm>


template<class T>
class ResourceManager
{
public:
	ResourceManager(size_t upperbound);
	~ResourceManager();

	bool addResource(std::string identifier,T* resource);

	T* getResource(std::string identifier);

	void deleteResource(std::string identifier);

private:
	unordered_map<std::string,T*> resourceBuffer;
	size_t maxSize;
	size_t currentSize;
};