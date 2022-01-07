#pragma once

#include "Engine/Collision.h"
using namespace Collision;

class CollisionProcess
{
public:
	RectAngle Controler;

	RectAngle BlockRect[4][4];

public:

	void SetPlayer(Rendering::Animation::Component Player);

	void SetBlock();

	bool WallCollision();
};

