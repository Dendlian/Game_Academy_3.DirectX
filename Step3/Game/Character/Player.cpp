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

	Black[0].Content = "Black";
	Black[0].Location = { 0, 0 };
	Black[0].Length = { 32, 7 };
	Black[0].Angle = 0;

	Black[1].Content = "Black";
	Black[1].Location = { 0, 0 };
	Black[1].Length = { 32, 4 };
	Black[1].Angle = 0;

	// Set SM_bar
	SM_Bar.Content = "White";
	SM_Bar.Location = { 0, 0 };
	SM_Bar.Length = { 30, 3 };
	SM_Bar.Angle = 0;

	Green.Content = "Green";
	Green.Location = { 0,0 };
	Green.Length = { 30, 3 };
	Green.Angle = 0;
		

	// Set Magic 
	for (int i = 0; i < 10; i++)
		Fireball[i].Set();

	// Set SuperMagic
	for (int i = 0; i < 3; i++)
		Frameball[i].Set();

	// Set Wall
	Coll.SetBlock();
}

void Player::Move()
{

	PlayerUpdate();
	Coll.SetPlayer(PlayerAnim);
	
	TransMagic();

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
	SetSM_Bar();

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
	AttackZombie	= false;
	AttackBoss		= false;
	S_AttackZombie	= false;
	S_AttackBoss	= false;

	if (Input::Get::Key::Press(VK_SPACE))
	{
		if(FrameStack < 400)
			FrameStack += 1;
	}


	if (Input::Get::Key::Up(VK_SPACE) && (FrameStack < 100))
	{
		frameball = false;
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

	if (Input::Get::Key::Up(VK_SPACE))
	{
		Green.Length = { 0,0 };

		if (FrameStack == 400 && (using_Magic == 1)) 
		{
			for (int i = 0; i < 3; i++)
			{
				if (!Frameball[i].ING)
				{
					Frameball[i].F_Anim.Location[0] = P_Location[0];
					Frameball[i].F_Anim.Location[1] = P_Location[1] + 10;
					Frameball[i].F_Direction = PlayerAnim.CurrentLine;
					Frameball[i].ING = true;
					break;
				}
			}
		}
		FrameStack = 0;
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
			if (Fireball[i].AttackBoss)
			{
				AttackBoss = true;
				Select_Boss = Fireball[i].Select_Boss;
				Fireball[i].AttackBoss = false;
				break;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (Frameball[i].ING)
		{
			Frameball[i].Move();
			if (Frameball[i].AttackZombie)
			{
				S_AttackZombie = true;
				Select_Zombie = Frameball[i].Select_Zombie;
				ZombieDirect = Frameball[i].ZombieDirect;
				Frameball[i].AttackZombie = false;
				break;
			}
			if (Frameball[i].AttackBoss)
			{
				S_AttackBoss = true;
				Select_Boss = Frameball[i].Select_Boss;
				Frameball[i].AttackBoss = false;
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
	for (int i = 0; i < 3; i++)
	{
		if (Frameball[i].ING)
			Frameball[i].Z_Location.push_back(zombie);
	}
}

void Player::GetBoss(RectAngle boss)
{
	for (int i = 0; i < 10; i++)
	{
		if (Fireball[i].ING)
			Fireball[i].B_Location.push_back(boss);
	}
	for (int i = 0; i < 3; i++)
	{
		if (Frameball[i].ING)
			Frameball[i].B_Location.push_back(boss);
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

	Black[0].Location[0] = P_Location[0];
	Black[0].Location[1] = P_Location[1] + 35;
	Black[0].Draw();

	Hp_Bar.Length = { 30 * (Hp / 1000),5 };

	Hp_Bar.Location[0] = P_Location[0] + (((Hp/2 - 500) * 30) / 1000);
	Hp_Bar.Location[1] = P_Location[1] + 35;
	Hp_Bar.Draw();
}

void Player::SetSM_Bar()
{
	Black[1].Location[0] = P_Location[0];
	Black[1].Location[1] = P_Location[1] + 30;

	Green.Location[0] = P_Location[0];
	Green.Location[1] = P_Location[1] + 30;
	
	Black[1].Draw();
	
	Green.Draw();

	if ((FrameStack > 100) && (FrameStack < 400) && (frameball))
		SM_Bar.Length = { (FrameStack - 100) / 10 , 3 };
	else
	{
		SM_Bar.Length = { 0, 0 };
		frameball = true;
	}

	if (FrameStack == 400)
	{
		Green.Length = { 30, 3 };
	}

	SM_Bar.Location[0] = P_Location[0] - 15 + ((FrameStack - 100) / 20);
	SM_Bar.Location[1] = P_Location[1] + 30;
	SM_Bar.Draw();
}

void Player::TransMagic()
{
	if (Input::Get::Key::Down(VK_SHIFT))
	{
		if (using_Magic < 2)
			using_Magic += 1;
		else using_Magic = 1;
	}

	switch (using_Magic)
	{
	case 1:
		for (int i = 0; i < 10; i++)
			if(!Fireball[i].ING)
				Fireball[i].F_Image.Content = "Fireball";
		break;
	case 2:
		for (int i = 0; i < 10; i++)
			if (!Fireball[i].ING)
				Fireball[i].F_Image.Content = "Iceball";
		break;
	}
}


