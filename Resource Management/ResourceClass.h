#pragma once
#include <string>

using namespace std;
template <class T>

class Resource

{
public:
	Resource(T p)
		:ID(NULL)
		,size(0)
	{}
	~Resource() {}
	
	virtual string GetID() { return ID; }
	virtual int GetSize() { return size; }
	virtual void SetID(String id) { ID = id; }
	virtual void SetSize(T p) { size = sizeof(p); }
	
protected:
	string ID;
	int size;

};

