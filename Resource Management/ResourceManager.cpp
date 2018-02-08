#include "ResourceManager.h"

template<class T>
ResourceManager<T>::ResourceManager(size_t upperbound)
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
	if ((currentSize + sizeof(resource)) <= maxSize) {
		resourceBuffer.insert(std::pair<std::string,T*>(identifier,resource));
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
void ResourceManager<T>::deleteResource(std::string identifier)
{	
	try
	{
		T* resource = resourceBuffer.at(identifier);
		currentSize -= sizeof(resource);
		delete resource;
		resourceBuffer.erase(identifier);
	}
	catch (const std::out_of_range)
	{

	}
}

