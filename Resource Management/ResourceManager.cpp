#include "ResourceManager.h"

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
	std::string formattedid = StringUtility(identifier);
	
	resourceBuffer.insert(std::pair<std::string,T*>(formattedid,resource));
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
	std::string formattedid = StringUtility(identifier);
	try
	{
		return resourceBuffer.at(formattedid);
	}
	catch (const std::out_of_range)
	{
		return nullptr;
	}
}

template<class T>
void ResourceManager<T>::deleteResource(std::string identifier, T * resource)
{
	std::string formattedid = StringUtility(identifier);
	delete resourceBuffer.at(formattedid);
	resourceBuffer.erase(formattedid);
	currentSize -= sizeof(resource);
}

template<class T>
std::string ResourceManager<T>::StringUtility(std::string identifier)
{
	std::string finalIdentifier = identifier;

	//Remove spaces
	finalIdentifier.erase(remove_if(finalIdentifier.begin(), finalIdentifier.end(),
		isspace), finalIdentifier.end());

	//Make lowercase
	transform(finalIdentifier.begin(), finalIdentifier.end(),
		finalIdentifier.begin(), ::tolower);

	return finalIdentifier;
}
