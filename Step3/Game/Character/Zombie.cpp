#include "stdafx.h"
#include "Zombie.h"

void Zombie::Set()
{
	ZombieAnim.Content = "Zombie";
	ZombieAnim.Location = { 0, -400 };
	ZombieAnim.Length = { 70, 70 };
	ZombieAnim.Duration = 1;
	ZombieAnim.Repeatable = true;
	ZombieAnim.Line = 8;

	Coll.SetBlock();
}

void Zombie::Move()
{
	ZombieUpdate();
	Coll.SetZombie(ZombieAnim);

#pragma region Zombie Move
	Vector<2> PZ;
	PZ = P_Location - Z_Location;

	Vector<2> direction = { 0, 0 };

	if (PZ[0] < 0)
	{
		Coll.Zombie.Location[0] -= 3;
		if (!Coll.WallCollision(Coll.Zombie))
			direction[0] -= 1;
		else if (PZ[1] > 0)
		{
			Coll.Zombie.Location[0] += 3;
			Coll.Zombie.Location[1] += 3;
			if (!Coll.WallCollision(Coll.Zombie))
				direction[1] += 1;
		}
		else if (PZ[1] < 0)
		{
			Coll.Zombie.Location[0] += 3;
			Coll.Zombie.Location[1] -= 3;
			if (!Coll.WallCollision(Coll.Zombie))
				direction[1] -= 1;
		}
	}

		
	if (PZ[0] > 0)
	{
		Coll.Zombie.Location[0] += 3;
		if (!Coll.WallCollision(Coll.Zombie))
			direction[0] += 1;
		else if (PZ[1] > 0)
		{
			Coll.Zombie.Location[0] -= 3;
			Coll.Zombie.Location[1] += 3;
			if (!Coll.WallCollision(Coll.Zombie))
				direction[1] += 1;
		}
		else if (PZ[1] < 0)
		{
			Coll.Zombie.Location[0] -= 3;
			Coll.Zombie.Location[1] -= 3;
			if (!Coll.WallCollision(Coll.Zombie))
				direction[1] -= 1;
		}
	}

	if (PZ[1] < 0)
	{
		Coll.Zombie.Location[1] -= 3;
		if (!Coll.WallCollision(Coll.Zombie))
			direction[1] -= 1;
	}

	if (PZ[1] > 0)
	{
		Coll.Zombie.Location[1] += 3;
		if (!Coll.WallCollision(Coll.Zombie))
			direction[1] += 1;
	}
#pragma endregion

	if (Length(direction) != 0)	
		ZombieAnim.Location += Normalize(direction) * 100 * Time::Get::Delta();
	
	ZombieAnim.Zombie_Draw(PZ);
}

void Zombie::GetDamage(Vector<2> zombiedirect)
{
	ZombieAnim.Location[1] -= 10;
}


