#pragma once
#include <unordered_map>
#include <algorithm>

template<class T>
class ResourceManager
{
public:
	ResourceManager()
	{
		resourceManagerID = "";
		maxSize = 0;
		currentSize = 0;
	}

	ResourceManager(std::string id, size_t upperbound)
	{
		resourceManagerID = id;
		maxSize = upperbound;
		currentSize = 0;
	}

	~ResourceManager()
	{
		deleteAllResources();
	}

	void addResource(T* resource)
	{
		if ((currentSize + resource->getSize()) <= maxSize)
		{
			resourceBuffer.insert({ resource->getName(), resource });
			currentSize += resource->getSize();
		}
	}

	T* getResource(std::string identifier)
	{
		if (resourceBuffer.find(identifier) == resourceBuffer.end())
		{
			return nullptr;
		}
		else
		{
			return resourceBuffer.at(identifier);
		}
	}

	void deleteResource(std::string identifier)
	{
		if ((T* resource = resourceBuffer.find(identifier)) != resourceBuffer.end())
		{
			currentSize -= resource->GetSize();	//getSize() is the memberfunction of the Resource.h
			delete resource;
			resourceBuffer.erase(identifier);
		}
	}

	void deleteAllResources()
	{
		for (auto iter = resourceBuffer.begin(); iter != resourceBuffer.end(); iter++)
		{
			delete iter->second;
		}

		resourceBuffer.clear();
	}

private:
	std::string resourceManagerID;
	size_t maxSize;
	size_t currentSize;
	std::unordered_map<std::string, T*> resourceBuffer;
};