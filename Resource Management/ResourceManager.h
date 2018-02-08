#pragma once
#include <map>

template<class T>
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void addResource(std::string identifier,T* resource);

	T* getResource(std::string identifier);

	void deleteResource(std::string identifier);
protected:
	map<std::string,T*> resourceBuffer;
};