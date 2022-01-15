#include "stdafx.h"
#include "Magic.h"

void Magic::Set()
{
	F_Image.Content = "Fireball";
	F_Image.Angle = 0;
	F_Image.Length = { 10, 10 };

	Coll.SetBlock();
}

void Magic::Move()
{
	Coll.SetMagic(F_Image);

	for (int i = 0; i < Z_Location.size(); i++)
	{
		if (Collide(Coll.Magic, Z_Location[i]))
		{
			AttackZombie = true;
			Select_Zombie = i;
			break;
		}
	}

	if ((!Coll.WallCollision(Coll.Magic)) && (!AttackZombie))
	{
		F_Image.Location += Normalize(SetDirect(F_Direction)) * 1500 * Time::Get::Delta();
		F_Image.Draw();
	}
	else if(Coll.WallCollision(Coll.Magic))
	{
		ING = false;
		F_Image.Location = { 100, 100 };
	}
	else if (AttackZombie)
	{
		ING = false;
		ZombieDirect = SetDirect(F_Direction);
		F_Image.Location = { 100, 100 };
	}

	while (!Z_Location.empty())
	{
		Z_Location.pop_back();
	}
}

Vector<2> Magic::SetDirect(unsigned int f_direction)
{
	Vector<2> direction;

	switch (F_Direction)
	{
	case 1:
		direction[1] -= 1;
		break;
	case 2:
		direction[0] -= 1;
		break;
	case 3:
		direction[0] += 1;
		break;
	case 4:
		direction[1] += 1;
		break;
	case 5:
		direction[0] -= 1;
		direction[1] -= 1;
		break;
	case 6:
		direction[0] += 1;
		direction[1] -= 1;
		break;
	case 7:
		direction[0] -= 1;
		direction[1] += 1;
		break;
	case 8:
		direction[0] += 1;
		direction[1] += 1;
		break;
	}

	return direction;
}
