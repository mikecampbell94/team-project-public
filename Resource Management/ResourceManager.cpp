#include "ResourceManager.h"

template<class T>
ResourceManager<T>::ResourceManager()
{

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
void ResourceManager<T>::addResource(std::string identifier, T * resource)
{
	resourceBuffer.insert(std::pair<std::string,T*>(identifier,resource));
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
	delete resourceBuffer.at(identifier);
	resourceBuffer.erase(identifier);
}