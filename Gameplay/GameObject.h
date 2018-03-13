#pragma once
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Resource Management/Resources/Resource.h"
#include "../Utilities/Maths/Vector3.h"
#include <functional>

class PhysicsNode;

struct PaintGameStats
{
	int maxPaint;
	int currentPaint = 0;
	float defaultInvMass = 1.f;
	int meteors;
	NCLVector4 colourToPaint;
	NCLVector3 defaultScale;
	float timeToWait;
	std::function<void()> executeAfter = std::function<void()>();
	float timer;
	bool canJump = false;
};

class GameObject : public Resource
{
public:
	GameObject();
	~GameObject();

	void setSceneNode(SceneNode* sceneNode);
	void setPhysicsNode(PhysicsNode* physicsNode);

	SceneNode* getSceneNode();
	PhysicsNode* getPhysicsNode();

	void update(float dt);
	void setPosition(NCLVector3 position);
	void setRotation(NCLVector4 rotation);
	void setScale(NCLVector3 scale);
	void setEnabled(bool isEnabled);

	const NCLVector3& getScale() const
	{
		return scale;
	}

	const NCLVector3& getPosition() const
	{
		return position;
	}

	bool getEnabled()
	{
		return isEnabled;
	}

	bool isEnabled = true;
	PaintGameStats stats;

private: 
	SceneNode *sceneNode;
	PhysicsNode *physicsNode = nullptr;

	

	NCLVector3 position;
	NCLVector3 scale;

};

