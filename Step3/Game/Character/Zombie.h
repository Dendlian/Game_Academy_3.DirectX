#pragma once
#include "Manager/Object.h"
#include "Physics/CollisionProcess.h"
#include "Player.h"
#include "Magic.h"

class Zombie : public Object
{
public:
	void Set()			override;
	void Move()			override;
public:
	Vector<2>		Z_Length = { 0,0 };
	Vector<2>		Z_Location = { 0, 0 };
	float			Z_Angle = { 0 };
	unsigned int	Z_Direction = 1;

	float Hp = 100;
	float Damage = 100;
	bool dead = false;

	int randnum;

	bool stun = false;
	bool frozen = false;
	bool superfrozen = false;

	int stunperiod = 0;
	int frozenperiod = 0;

	bool AttackPlayer = false;

	CollisionProcess Coll;
	Rendering::Animation::Component ZombieAnim;

public:


	FORCEINLINE void ZombieUpdate()
	{
		Z_Location = ZombieAnim.Location;
		Z_Direction = ZombieAnim.CurrentLine;
	}

	FORCEINLINE void GetPlayer(RectAngle player)
	{
		Coll.Player = player;
	}
	
	void GetDamage(Vector<2> zombiedirect, float damage, unsigned using_magic);

};

