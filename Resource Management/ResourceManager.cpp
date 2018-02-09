#include "ResourceManager.h"

template<class T>
ResourceManager<T>::ResourceManager(std::string id, size_t upperbound)
{
	ResourceManagerID = id;
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
	resourceBuffer.clear();
}

template<class T>
bool ResourceManager<T>::addResource(std::string identifier, Resource<T>* resource)
{	
	if ((currentSize + resource->GetSize()) <= maxSize)
	{
		resourceBuffer.insert(std::pair<std::string,T*>(identifier,resource));
		currentSize += resource->GetSize();
	}
	else
		return false;
}

template<class T>
Resource<T>* ResourceManager<T>::getResource(std::string identifier)
{			
	if ((Resource<T>* resource = resourceBuffer.find(identifier)) != resourceBuffer.end())
		return nullptr;
	else
		return resource;
}

template<class T>
void ResourceManager<T>::deleteResource(std::string identifier)
{				
	if ((Resource<T>* resource = resourceBuffer.find(identifier)) != resourceBuffer.end())
	{	
		currentSize -= resource->GetSize();	//getSize() is the memberfunction of the Resource.h
		delete resource;
		resourceBuffer.erase(identifier);
	}	
	else
		return nullptr;
}

