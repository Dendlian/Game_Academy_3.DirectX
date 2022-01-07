#pragma once

#include "Engine/Collision.h"
using namespace Collision;

class CollisionProcess
{
public:
	RectAngle Object;

	RectAngle BlockRect[4][4];

public:

	void SetObject(Rendering::Animation::Component Character);

	void SetObject(Rendering::Image::Component Thing);

	void SetBlock();

	bool WallCollision();
};

