#pragma once
#include "Manager/Object.h"
#include "Physics/CollisionProcess.h"
#include "Magic.h"
#include "SperMagic.h"
#include <vector>

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
	unsigned int using_Magic = 1;

	Rendering::Animation::Component PlayerAnim;
	Rendering::Image::Component Hp_Bar;
	Rendering::Image::Component SM_Bar;
	Rendering::Image::Component Black[2];
	Rendering::Image::Component Green;

	bool AttackZombie = false;
	bool S_AttackZombie = false;
	int Select_Zombie = 0;

	bool AttackBoss = false;
	bool S_AttackBoss = false;
	int Select_Boss = 0;
	
	Vector<2> ZombieDirect = { 0, 0 };

	int FrameStack = 0;
	bool frameball = false;


public:

	Magic		Fireball[10];
	SuperMagic  Frameball[3];

	int invalidity = 0;

	Rendering::Camera	Camera;

	FORCEINLINE void PlayerUpdate()
	{
		P_Location = PlayerAnim.Location;
		P_Direction = PlayerAnim.CurrentLine;
	}

	void GetZombie(RectAngle zombie);
	void GetBoss(RectAngle boss);

	void GetDamage(float damage);
	
	void SetHP_Bar();
	void SetSM_Bar();
	void TransMagic();

};

