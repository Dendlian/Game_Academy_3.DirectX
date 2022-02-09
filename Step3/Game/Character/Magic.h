#pragma once
#include "Manager/Object.h"
#include "Engine/Vector.h"
#include "Physics/CollisionProcess.h"

#include <vector>
using namespace std;

class Magic : public Object
{
public:

	unsigned int using_Magic = 1;

	Rendering::Image::Component F_Image;
	unsigned int	F_Direction = 1;
	
	bool ING = false;

	float Damage = 10; 
	CollisionProcess Coll;

public:
	void Set()	override;
	void Move() override;

	Vector<2> SetDirect(unsigned int f_direction);

	bool AttackZombie = false;
	int Select_Zombie = 0;
	Vector<2> ZombieDirect = { 0, 0 };
	vector<RectAngle> Z_Location;

	bool AttackBoss = false;
	int Select_Boss = 0;
	vector<RectAngle> B_Location;
};