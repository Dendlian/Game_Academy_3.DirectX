#include "stdafx.h"
#include "Player.h"

void Player::Set()
{
	PlayerAnim.Content = "Player";
	PlayerAnim.Location = { 0, 0 };
	PlayerAnim.Length = { 70, 70 };
	PlayerAnim.Duration = 1;
	PlayerAnim.Repeatable = true;
	PlayerAnim.Line = 8;

	// Set Magic 
	for (int i = 0; i < 10; i++)
	{
		Fireball[i].Set();
	}

	// Set Wall
	Coll.SetBlock();
}

void Player::Move()
{
	PlayerUpdate();
	Coll.SetPlayer(PlayerAnim);
	
#pragma region Player Move 

	Vector<2> direction;

	if (Input::Get::Key::Press(VK_LEFT))
	{
		Coll.Player.Location[0] -= 3;
		if (!Coll.WallCollision(Coll.Player))
			direction[0] -= 1;
		else if(Input::Get::Key::Press(VK_UP))
		{
			Coll.Player.Location[0] += 3;
			Coll.Player.Location[1] += 3;
			if (!Coll.WallCollision(Coll.Player))
				direction[1] += 1;
		}
		else if (Input::Get::Key::Press(VK_DOWN))
		{
			Coll.Player.Location[0] += 3;
			Coll.Player.Location[1] -= 3;
			if (!Coll.WallCollision(Coll.Player))
				direction[1] -= 1;
		}
	}
	
	if (Input::Get::Key::Press(VK_RIGHT))
	{
		Coll.Player.Location[0] += 3;
		if (!Coll.WallCollision(Coll.Player))
			direction[0] += 1;
		else if (Input::Get::Key::Press(VK_UP))
		{
			Coll.Player.Location[0] -= 3;
			Coll.Player.Location[1] += 3;
			if (!Coll.WallCollision(Coll.Player))
				direction[1] += 1;
		}
		else if (Input::Get::Key::Press(VK_DOWN))
		{
			Coll.Player.Location[0] -= 3;
			Coll.Player.Location[1] -= 3;
			if (!Coll.WallCollision(Coll.Player))
				direction[1] -= 1;
		}
	}
	
	if (Input::Get::Key::Press(VK_UP))
	{
		Coll.Player.Location[1] += 3;
		if (!Coll.WallCollision(Coll.Player))
			direction[1] += 1;
	}
	
	if (Input::Get::Key::Press(VK_DOWN))
	{
		Coll.Player.Location[1] -= 3;
		if (!Coll.WallCollision(Coll.Player))
			direction[1] -= 1;
	}
#pragma endregion

	if (Length(direction) != 0)
	{
		
		Camera.Location		+= Normalize(direction) * 600 * Time::Get::Delta();
		PlayerAnim.Location += Normalize(direction) * 600 * Time::Get::Delta();
	}

	Camera.Set();
	PlayerAnim.Player_Draw();
	
	// Update Zombie
	Coll.Zombie = Zombie;
	for (int i = 0; i < 10; i++)
	{
		Fireball[i].Coll.Zombie = Zombie;
	}
}

void Player::Attack()
{
	// ZombieAttack = false;

	if (Input::Get::Key::Down(VK_SPACE))
	{
		for (int i = 0; i < 10; i++)
		{
			if (!Fireball[i].ING)
			{
				fire_turn = i;
				Fireball[fire_turn].F_Image.Location[0] = P_Location[0];
				Fireball[fire_turn].F_Image.Location[1] = P_Location[1] + 10;
				Fireball[fire_turn].F_Direction = PlayerAnim.CurrentLine;
				Fireball[i].ING = true;
				break;
			}
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if(Fireball[i].ING) Fireball[i].Move();
	}
}

