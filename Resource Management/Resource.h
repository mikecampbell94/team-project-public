#pragma once
#include <string>

class Resource

{
public:
	Resource()
	{
		name = "";
		size = size_t(0);
	}

	Resource(std::string name, size_t size)
	{
		this->name = name;
		this->size = size;
	}

	~Resource() {}

	std::string getName() 
	{ 
		return name; 
	}

	void setName(std::string newName) 
	{ 
		name = newName;
	}

	size_t getSize() 
	{ 
		return size; 
	}

	void setSize(size_t newSize) 
	{ 
		size = newSize; 
	}

protected:
	std::string name;
	size_t size;
};