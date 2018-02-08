#pragma once
#include <map>

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
	map<std::string,T*> resourceBuffer;
	size_t maxSize;
	size_t currentSize;
};