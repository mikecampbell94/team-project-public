#pragma once
#include "Postman.h"

//service locator
class DeliverySystem
{
public:
	static Postman* getPostman() { return postman_; }

	static void provide(Postman* postman)
	{
		postman_ = postman;
	}

private:
	static Postman* postman_;
};