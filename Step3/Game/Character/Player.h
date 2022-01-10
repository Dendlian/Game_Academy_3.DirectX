#pragma once
#include "Manager/Character.h"
#include "Physics/CollisionProcess.h"
#include "Magic.h"

class Player : public Character
{
public:
	void SetCharacter() override;
	void Move()			override;

	void Attack();

public:
	Vector<2>		P_Length = { 0,0 };
	float			P_Angle = { 0 };
	Vector<2>		P_Location = { 0,0 };
	unsigned int	P_Direction = 1;

public:
	
	Rendering::Animation::Component PlayerAnim;

	CollisionProcess	Wall;

	Magic	Fire[10];
	int		fire_turn = 0;
	bool	fire_ing[10];

	Rendering::Camera	Camera;


	FORCEINLINE void PlayerUpdate()
	{
		P_Location = PlayerAnim.Location;
		P_Direction = PlayerAnim.CurrentLine;
	}
};

