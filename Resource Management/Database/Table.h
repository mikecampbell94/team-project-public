#pragma once

#include <string>

struct TableConfiguration
{
	std::string name;
	size_t maxSize;

	TableConfiguration()
	{
		this->name = name;
		this->maxSize = maxSize;
	}
};

template <class ResourceType>
class Table
{
public:
	Table(const std::string name, const size_t maxSize)
	{

	}

	~Table()
	{
		
	}

	void addResource(ResourceType* resource)
	{
		
	}

	void deleteResource(ResourceType* resource)
	{
		
	}

//	ResourceManager<ResourceType>* getAllEntries
//	{
//		return &storage;
//	}
//
//private:
//	ResourceManager<ResourceType>* storage;
};

