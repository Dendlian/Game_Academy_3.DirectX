#pragma once
#include "Physics/CollisionProcess.h"
#include "Character/Player.h"

class Portion
{
public:
	void Set();
	void Move(Player & player);

	RectAngle Portion;
	Rendering::Image::Component P_Image;

	unsigned int timer = 0;
	bool empty = false;

};

