#include "stdafx.h"
#include "Zombie.h"
#include <random>
using namespace std;

void Zombie::Set()
{
	random_device rd;
	mt19937_64 gen(rd());
	uniform_int_distribution<int> Rand(1, 4);
	int val = Rand(gen);

	if (val == 1) ZombieAnim.Location = { 500, 0 };
	else if (val == 2) ZombieAnim.Location = { -500, 0 };
	else if (val == 3) ZombieAnim.Location = { 0, 500 };
	else if (val == 4) ZombieAnim.Location = { 0, -500 };

	ZombieAnim.Content = "Zombie";
	ZombieAnim.Length = { 70, 70 };
	ZombieAnim.Duration = 1;
	ZombieAnim.Repeatable = true;
	ZombieAnim.Line = 8;

	Coll.SetBlock();
}

void Zombie::Move()
{
	AttackPlayer = false;
	
	ZombieUpdate();

	Coll.SetZombie(ZombieAnim);

	if (Coll.ZombieCollision(Coll.Player))
		AttackPlayer = true;

	if (!dead)
	{
		if (stunperiod == 0)
		{
			randnum = rand() % 200 + 200;
			if (stun) stun = false;
			else stun = true;
		}

		stunperiod += 1;
		if (stunperiod == randnum) stunperiod = 0;
		

#pragma region Zombie Move

		Vector<2> PZ;
		PZ = Coll.Player.Location - Z_Location;
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
		if((!stun) && (Length(direction) != 0))
			ZombieAnim.Location += Normalize(direction) * 100 * Time::Get::Delta();
		
		ZombieAnim.Zombie_Draw(PZ);
	}
}

void Zombie::GetDamage(Vector<2> zombiedirect, float damage)
{
	ZombieAnim.Location += Normalize(zombiedirect) * 10;
	Hp -= damage;
	if (Hp == 0) dead = true;
}


