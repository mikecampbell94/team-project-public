#pragma once

class Player;

class ThirdPersonCameraController
{
public:
	ThirdPersonCameraController(Player* player);
	~ThirdPersonCameraController();

	void moveCamera();

private:
	Player* player;
};

