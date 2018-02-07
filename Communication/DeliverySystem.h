#pragma once
#include "Postman.h"

//service locator
class DeliverySystem
{
public:
	static Postman* getPostman() 
	{ 
		return postman; 
	}

	static void provide(Postman* newPostman)
	{
		postman = newPostman;
	}

private:
	static Postman* postman;
};