#pragma once
#include "Manager/Object.h"
#include "Physics/CollisionProcess.h"
#include "Magic.h"


class Player : public Object
{
public:
	void Set()			override;
	void Move()			override;
	void Attack();

	CollisionProcess Coll;

public:
	Vector<2>		P_Length = { 0,0 };
	Vector<2>		P_Location = { 0,0 };
	float			P_Angle = { 0 };
	unsigned int	P_Direction = 1;

	float Hp = 1000;
	int invincibility = 0;

	Rendering::Animation::Component PlayerAnim;
	Rendering::Image::Component Hp_Bar;
	Rendering::Image::Component Black;

	Vector<2> ZombieDirect = { 0, 0 };

public:

	Magic	Fireball[10];
	bool AttackZombie = false;

	int invalidity = 0;

	Rendering::Camera	Camera;

	FORCEINLINE void PlayerUpdate()
	{
		P_Location = PlayerAnim.Location;
		P_Direction = PlayerAnim.CurrentLine;
	}

	FORCEINLINE void GetZombie(RectAngle zombie)
	{
		Coll.Zombie = zombie;
	}

	void GetDamage();

	void SetHP_Bar();
};

