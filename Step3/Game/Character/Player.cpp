#include "stdafx.h"
#include "Player.h"

void Player::SetCharacter()
{
	PlayerAnim.Content = "Player";
	PlayerAnim.Location = { 0, 200 };
	PlayerAnim.Length = { 70, 70 };
	PlayerAnim.Duration = 1;
	PlayerAnim.Repeatable = true;
	PlayerAnim.Line = 8;

	Wall.SetBlock();
}

void Player::Move()
{
	PlayerUpdate();
	Wall.SetObject(PlayerAnim);

	Vector<2> direction;
	
#pragma region Player Move 
	if (Input::Get::Key::Press(VK_LEFT))
	{
		Wall.Object.Location[0] -= 3;
		if (!Wall.WallCollision())
			direction[0] -= 1;
		else if(Input::Get::Key::Press(VK_UP))
		{
			Wall.Object.Location[0] += 3;
			Wall.Object.Location[1] += 3;
			if (!Wall.WallCollision())
				direction[1] += 1;
		}
		else if (Input::Get::Key::Press(VK_DOWN))
		{
			Wall.Object.Location[0] += 3;
			Wall.Object.Location[1] -= 3;
			if (!Wall.WallCollision())
				direction[1] -= 1;
		}
	}
	
	if (Input::Get::Key::Press(VK_RIGHT))
	{
		Wall.Object.Location[0] += 3;
		if (!Wall.WallCollision())
			direction[0] += 1;
		else if (Input::Get::Key::Press(VK_UP))
		{
			Wall.Object.Location[0] -= 3;
			Wall.Object.Location[1] += 3;
			if (!Wall.WallCollision())
				direction[1] += 1;
		}
		else if (Input::Get::Key::Press(VK_DOWN))
		{
			Wall.Object.Location[0] -= 3;
			Wall.Object.Location[1] -= 3;
			if (!Wall.WallCollision())
				direction[1] -= 1;
		}
	}
	
	if (Input::Get::Key::Press(VK_UP))
	{
		Wall.Object.Location[1] += 3;
		if (!Wall.WallCollision())
			direction[1] += 1;
	}
	
	if (Input::Get::Key::Press(VK_DOWN))
	{
		Wall.Object.Location[1] -= 3;
		if (!Wall.WallCollision())
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
}

void Player::Attack()
{
	if (Input::Get::Key::Down(VK_SPACE))
	{
		attack = true;
	}
	
	if (attack)
	{
		Magic* Fire = new Magic;
		Fire->Set();
		Fire->F_Image.Location[0] = P_Location[0];
		Fire->F_Image.Location[1] = P_Location[1] + 10;
		Fire->F_Direction = PlayerAnim.CurrentLine;
		Fire->Move();
	}


}
