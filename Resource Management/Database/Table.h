#pragma once

#include "../ResourceManager.h"
#include "../XMLParser.h"

#include <string>
#include <functional>

struct TableConfiguration
{
	std::string name;
	size_t maxSize;

	TableConfiguration(std::string name, size_t maxSize)
	{
		this->name = name;
		this->maxSize = maxSize;
	}
};

template <class ResourceType>
class Table
{
public:
	Table(const std::string name, const size_t maxSize, std::function<ResourceType*(Node*)> builder)
	{
		this->name = name;
		this->maxSize = maxSize;
		this->builder = builder;

		storage = new ResourceManager<ResourceType>(name, maxSize);
	}

	~Table()
	{
		
	}

	void addNewResource(Node* resource)
	{
		storage.addResource(builder(resource));
	}

	void deleteResource(std::string identifier)
	{
		storage.deleteResource(identifier);
	}

	ResourceManager<ResourceType>* getAllResources()
	{
		return &storage;
	}

private:
	std::string name;
	size_t maxSize;

	ResourceManager<ResourceType> storage;
	std::function<ResourceType*(Node*)> builder;
};

