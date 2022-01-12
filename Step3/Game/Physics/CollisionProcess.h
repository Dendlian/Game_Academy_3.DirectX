#pragma once
#include "Engine/Collision.h"
using namespace Collision;

class CollisionProcess
{
public:
	RectAngle Player;
	RectAngle Zombie;
	RectAngle Magic;

	RectAngle BlockRect[4][4];

public:

	void SetPlayer(Rendering::Animation::Component Character);

	void SetZombie(Rendering::Animation::Component Character);

	void SetMagic(Rendering::Image::Component Thing);

	void SetBlock();

	bool WallCollision(RectAngle Object);

	bool ZombieCollition(RectAngle Object);

	
};

