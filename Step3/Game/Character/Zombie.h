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
	Vector<2>		Z_Location = { 0,-400 };
	float			Z_Angle = { 0 };
	unsigned int	Z_Direction = 1;

	unsigned int Hp = 100;

	CollisionProcess Coll;

	Rendering::Animation::Component ZombieAnim;

public:

	Vector<2>	P_Location = { 0,0 };


	FORCEINLINE void ZombieUpdate()
	{
		Z_Location = ZombieAnim.Location;
		Z_Direction = ZombieAnim.CurrentLine;
	}

	FORCEINLINE void GetPlayer(Player player)
	{
		P_Location = player.P_Location;
	}
	
	void GetDamage(Vector<2> zombiedirect);

};

