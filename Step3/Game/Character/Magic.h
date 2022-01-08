#pragma once
#include "Engine/Vector.h"
#include "Physics/CollisionProcess.h"

class Magic
{
public:
	Rendering::Image::Component F_Image;
	CollisionProcess	Wall;
	unsigned int	F_Direction = 1;

public:
	float Damage = 10;
	void Set();
	void Move();
};

