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

	unsigned int Hp = 1000;

	Rendering::Animation::Component PlayerAnim;

public:
	
	RectAngle Zombie;

	Magic	Fireball[10];
	int		fire_turn = 0;

	// bool ZombieAttack = false;
	// Vector<2> ZombieDirect = {0, 0};

	Rendering::Camera	Camera;

	FORCEINLINE void PlayerUpdate()
	{
		P_Location = PlayerAnim.Location;
		P_Direction = PlayerAnim.CurrentLine;
	}

	FORCEINLINE void GetZombie(RectAngle zombie)
	{
		Zombie = zombie;
	}
};

