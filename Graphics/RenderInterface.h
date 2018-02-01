#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "../Utilities/Maths/Vector4.h"

class RenderInterface : public Subsystem
{
public:
	RenderInterface();
	~RenderInterface();

	void loadingScreen() {};
	void renderScene(const float& deltaTime) {};
	void removeAsset() {};
	void addAsset() {};
	void changeColor(Vector4 c) {};

};

