#pragma once
#include "Manager/Object.h"
#include "Engine/Vector.h"
#include "Physics/CollisionProcess.h"

#include <vector>
using namespace std;

class Magic : public Object
{
public:

	Rendering::Image::Component F_Image;
	unsigned int	F_Direction = 1;
	
	bool ING = false;
	bool AttackZombie = false;
	
	float Damage = 10;
	CollisionProcess Coll;

	Vector<2> ZombieDirect = { 0, 0 };

public:
	void Set()	override;
	void Move() override;

	Vector<2> SetDirect(unsigned int f_direction);
};