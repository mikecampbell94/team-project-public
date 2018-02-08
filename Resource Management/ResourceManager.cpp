#include "ResourceManager.h"
#include <iostream>
template<class T>
ResourceManager<T>::ResourceManager(unsigned int upperbound)
{
	maxSize = upperbound;
	currentSize = 0;
}

template<class T>
ResourceManager<T>::~ResourceManager()
{
	for (auto iter = resourceBuffer.begin();iter != resourceBuffer.end();iter++) 
	{
		delete iter->second;
	}
}

template<class T>
bool ResourceManager<T>::addResource(std::string identifier, T * resource)
{
	resourceBuffer.insert(std::pair<std::string,T*>(identifier,resource));
	if ((currentSize + sizeof(resource)) <= maxSize) {
		currentSize += sizeof(resource);
		return true;
	}
	else
		return false;
}

template<class T>
T * ResourceManager<T>::getResource(std::string identifier)
{
	try
	{
		return resourceBuffer.at(identifier);
	}
	catch (const std::out_of_range)
	{
		return nullptr;
	}
}

template<class T>
void ResourceManager<T>::deleteResource(std::string identifier, T * resource)
{
	delete resourceBuffer.at(identifier);
	resourceBuffer.erase(identifier);
	currentSize -= sizeof(resource);
}