#include "stdafx.h"
#include "Player.h"

void Player::Set()
{
	// Set Player
	PlayerAnim.Content = "Player";
	PlayerAnim.Location = { 0, 0 };
	PlayerAnim.Length = { 70, 70 };
	PlayerAnim.Duration = 1;
	PlayerAnim.Repeatable = true;
	PlayerAnim.Line = 8;

	// Set Hp_Bar
	Hp_Bar.Content = "HP";
	Hp_Bar.Location = { 0, 0 };
	Hp_Bar.Length = { 30, 5 };
	Hp_Bar.Angle = 0;

	Black.Content = "Black";
	Black.Location = { 0, 0 };
	Black.Length = { 32, 7 };
	Black.Angle = 0;

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

	SetHP_Bar();

	if(invalidity == 0)
	PlayerAnim.Player_Draw();
	else
	{
		if (invalidity < 800)
		{
			invalidity += 1;
			int num = static_cast<int>(invalidity);
			if ((num / 120) % 2 == 1) PlayerAnim.Player_Draw();
			else
			{
				if (Input::Get::Key::Press(VK_DOWN))
					PlayerAnim.CurrentLine = 1;
				if (Input::Get::Key::Press(VK_LEFT))
					PlayerAnim.CurrentLine = 2;
				if (Input::Get::Key::Press(VK_RIGHT))
					PlayerAnim.CurrentLine = 3;
				if (Input::Get::Key::Press(VK_UP))
					PlayerAnim.CurrentLine = 4;
			}
		}
		else invalidity = 0;
	}
}

void Player::Attack()
{
	AttackZombie = false;

	if (Input::Get::Key::Down(VK_SPACE))
	{
		for (int i = 0; i < 10; i++)
		{
			if (!Fireball[i].ING)
			{
				Fireball[i].F_Image.Location[0] = P_Location[0];
				Fireball[i].F_Image.Location[1] = P_Location[1] + 10;
				Fireball[i].F_Direction = PlayerAnim.CurrentLine;
				Fireball[i].ING = true;
				break;
			}
		}
	}
	for (int i = 0; i < 10; i++)
	{ 
		if (Fireball[i].ING)
		{
			Fireball[i].Move();
			if (Fireball[i].AttackZombie)
			{
				AttackZombie = true;
				Select_Zombie = Fireball[i].Select_Zombie;
				ZombieDirect = Fireball[i].ZombieDirect;
				Fireball[i].AttackZombie = false;
				break;
			}
		}
	}
}

void Player::GetZombie(RectAngle zombie)
{
	for (int i = 0; i < 10; i++)
	{
		if (Fireball[i].ING) 
			Fireball[i].Z_Location.push_back(zombie);
	}
}

void Player::GetDamage(float damage)
{
	if (invalidity == 0) 
	{
		Hp -= damage;
		invalidity += 1;
	}
}

void Player::SetHP_Bar()
{
	if (Hp > 1000) Hp = 1000;

	Black.Location[0] = P_Location[0];
	Black.Location[1] = P_Location[1] + 30;
	Black.Draw();

	Hp_Bar.Length = { 30 * (Hp / 1000),5 };

	Hp_Bar.Location[0] = P_Location[0] + (((Hp/2 - 500) * 30) / 1000);
	Hp_Bar.Location[1] = P_Location[1] + 30;
	Hp_Bar.Draw();
}


