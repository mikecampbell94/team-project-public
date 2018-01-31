#pragma once
#include <unordered_map>



class BaseController
{
public:
	BaseController();
	~BaseController();

	virtual bool const getKeyState(int key) = 0;

	inline void setActionKeyBind(int key) {};

	inline int const getActionKeyBind(int action) {};

protected:
	std::unordered_map<int, int> controllerActionBindings;
};

