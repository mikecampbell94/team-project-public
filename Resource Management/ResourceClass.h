#pragma once
#include "../Graphics/Scene Management/SceneNode.h"

typedef unsigned int GLuint;

class GameObject;

class Resourceclass
{
public:
	Resourceclass() {}
	~Resourceclass() {}
	
	virtual void SetID(string id) { ID =  id; }	
	virtual string GetID() { return ID; }
	
protected:
	string ID;
	


};

