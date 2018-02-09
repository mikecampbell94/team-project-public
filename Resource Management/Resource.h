#pragma once
#include <string>

using namespace std;
template <class T>
class Resource

{
public:
	Resource(T p)
	{
		name = "";
		size = 0;
	}
	~Resource() {}

	virtual string GetID() { return name; }
	virtual size_t GetSize() { return size; }
	virtual void SetID(std::string id) { name = id; }
	virtual void SetSize(T p) { size = sizeof(p); }

protected:
	string name;
	size_t size;

};