#include "stdafx.h"
#include "TestScene.h"
#include <stdlib.h>
#include <time.h>


void TestScene::Start()
{
#pragma region Map Setting
	Background.Content = "BackGround";
	Background.Length = { 1200, 1200 };

	Background2.Content = "BackGround2";
	Background2.Length = { 960 * 6, 730 * 6 };

	Background3.Content = "BackGround3";
	Background3.Length = { 250, 250 };

	for (int i = 0; i < 4; i++)
	{
		Background4[i].Content = "BackGround4";
		Background4[i].Length = { 100, 100 };
	}
	Background4[0].Location = { 200, 200 };
	Background4[1].Location = { -200, 200 };
	Background4[2].Location = { 200, -200 };
	Background4[3].Location = { -200, -200 };

	for (int i = 0; i < 4; i++)
	{
		Door[i].Content = "Door";
		Door[i].Length = { 70, 70 };
		Door[i].Duration = 1;
		Door[i].Repeatable = true;
	}

	Door[0].Location = { 0, 500 };
	Door[1].Location = { 0, -500 };
	Door[2].Location = { 500, 0 };
	Door[3].Location = { -500, 0 };

	BGM.Content = "BGM";
	BGM.volume = 0;
	BGM.Play();
#pragma endregion

#pragma region Text Setting
	Round.Content = "Round : ";
	Round.Length = { 100, 100 };
	Round.Location = { 70 , 70 };
	Round.Font.Name = "Hello";
	Round.Font.Size = 20;
	Round.Color = { 255, 0, 0 };
	Round.Font.Bold = true;

	Score.Content = "Score  : ";
	Score.Length = { 100, 100 };
	Score.Location = { 70 , 100 };
	Score.Font.Name = "Hello";
	Score.Font.Size = 20;
	Score.Color = { 0, 0, 255 };
	Score.Font.Bold = true;

	stringRound = to_string(currentRound);
	char const* round_char = stringRound.c_str();

	intRound.Content = round_char;
	intRound.Length = { 100, 100 };
	intRound.Location = { 150 , 70 };
	intRound.Font.Name = "Hello";
	intRound.Font.Size = 20;
	intRound.Color = { 255, 0, 0 };
	intRound.Font.Bold = true;

	stringScore = to_string(currentScore);
	char const* score_char = stringScore.c_str();

	intScore.Content = score_char;
	intScore.Length = { 100, 100 };
	intScore.Location = { 150 , 100 };
	intScore.Font.Name = "Hello";
	intScore.Font.Size = 20;
	intScore.Color = { 0, 0, 255 };
	intScore.Font.Bold = true;

	using_Magic.Content = "fireball";
	using_Magic.Length = { 35, 20 };
	using_Magic.Location = { 640 , 320 };
	using_Magic.Font.Name = "Hello";
	using_Magic.Font.Size = 12;
	using_Magic.Color = { 0, 0, 0 };
	using_Magic.Font.Bold = true;

#pragma endregion

#pragma region Block Setting

	// Map_Block1
	Map_Block1[0].Location = { 100, 100 };
	PasteBlock(Map_Block1);
	SetBlock(Map_Block1);

	// Map_Block2
	Map_Block2[0][0].Location = { 400, 400 };
	for (int i = 1; i <= 6; i++)
	{
		Map_Block2[i][0].Location = { 400, 400 - (i * 50) };
		Map_Block2[i + 6][0].Location = { 400 - (i * 50), 400 };
	}
	for (int i = 0; i < 13; i++)
	{
		PasteBlock(Map_Block2[i]);
		SetBlock(Map_Block2[i]);
	}

	// Map_Block3
	Map_Block3[0][0].Location = { 600, 600 };
	for (int i = 1; i <= 12; i++)
	{
		Map_Block3[i][0].Location = { 600, 600 - (i * 50) };
		Map_Block3[i + 12][0].Location = { 600 - (i * 50), 600 };
	}
	for (int i = 0; i < 25; i++)
	{
		PasteBlock(Map_Block3[i]);
		SetBlock(Map_Block3[i]);
	}
#pragma endregion

	Player.Set();
}

bool TestScene::Update()
{
#pragma region Draw Map

	if (rand_num != 100) rand_num += 1;
	else rand_num = 0;

	Background2.Draw();
	Background.Draw();
	// Background3.Draw();
	
	for (int i = 0; i < 4; i++)
	{
		Background4[i].Draw();
		Map_Block1[i].Draw();
	
		for (int j = 0; j < 13; j++)
			Map_Block2[j][i].Draw();
		for (int k = 0; k < 25; k++)
			Map_Block3[k][i].Draw();
	}


	if (created_Zombies == (currentRound * 10)) dooropen = false;
	
	if (dooropen)
		for (int i = 0; i < 4; i++) Door[i].Door_Draw();

#pragma endregion

#pragma region Draw Zombie

	if (create_stack < 2000)
		create_stack += 1;

	if ((create_stack == 1000) && (created_Zombies < (currentRound * 10)))
	{
		Zombie.push_back(new class Zombie);
		create_stack = 0;
		Zombie[current_Zombies]->Set();
		created_Zombies += 1;
		current_Zombies += 1;

		if ((currentRound % 2 == 0) && (created_Boss < (currentRound / 2)))
		{
			Boss.push_back(new class ZombieBoss);
			Boss[current_Boss]->Set();
			current_Boss += 1;
		}
	}

	for (int i = 0; i < current_Zombies; i++)
	{
		Zombie[i]->GetPlayer(Player.Coll.Player);
		Zombie[i]->Move();
		Player.GetZombie(Zombie[i]->Coll.Zombie);
		if (Zombie[i]->AttackPlayer)
			Player.GetDamage(Zombie[i]->Damage);
	}

	for (int i = 0; i < current_Boss; i++)
	{
		Boss[i]->GetPlayer(Player.Coll.Player);
		Boss[i]->Move();
		Player.GetBoss(Boss[i]->Coll.Zombie);
		if (Boss[i]->AttackPlayer)
			Player.GetDamage(Boss[i]->Damage);
	}

	for (int i = 0; i < current_Zombies; i++)
	{
		if (Zombie[i]->dead)
		{
			if ((rand_num % 10) == 0)
			{
				Portion.push_back(new class Portion);
				Portion[Portions]->Set();
				Portion[Portions]->P_Image.Location = Zombie[i]->Z_Location;
				Portion[Portions]->Portion.Location = Zombie[i]->Z_Location;
				Portions += 1;
			}
			Zombie.erase(Zombie.begin() + i);
			current_Zombies -= 1;

			currentScore += 100;
			stringScore = to_string(currentScore);
			char const* score_char = stringScore.c_str();
			intScore.Content = score_char;
		}
	}

	for (int i = 0; i < current_Boss; i++)
	{
		if (Boss[i]->dead)
		{
			Boss.erase(Boss.begin() + i);
			current_Boss -= 1;

			currentScore += 300;
			stringScore = to_string(currentScore);
			char const* score_char = stringScore.c_str();
			intScore.Content = score_char;
		}
	}

#pragma endregion

#pragma region Draw Player
	Player.Move();
	Player.Attack();

	if (Player.AttackZombie)
		Zombie[Player.Select_Zombie]->GetDamage(Player.ZombieDirect);

	if (Player.AttackBoss)
		Boss[Player.Select_Boss]->GetDamage();
#pragma endregion

#pragma region Draw Protion
	
	for (int i = 0; i < Portions; i++)
	{
		Portion[i]->Move(Player);
		if (Portion[i]->empty)
		{
			Portion.erase(Portion.begin() + i);
			Portions -= 1;
		}
	}


#pragma endregion

#pragma region Draw Text
	Round.Draw();
	Score.Draw();
	intRound.Draw();
	intScore.Draw();
	using_Magic.Draw();

#pragma endregion

#pragma region Next Round Setting
	if ((created_Zombies != 0) && (Zombie.empty()))
	{
		dooropen = true;
		currentRound += 1;
		create_stack = 0;
		created_Zombies = 0;

		stringRound = to_string(currentRound);
		char const* round_char = stringRound.c_str();
		intRound.Content = round_char;
	}
#pragma endregion

#pragma region Game Over
	if (Input::Get::Key::Down(VK_ESCAPE)) 
	{
		Player.Camera.Location = { 0, 0 };
		return true; 
	}

	if (Player.Hp <= 0) { return true; }
	else { return false; }
#pragma endregion
}

void TestScene::End() { }

void TestScene::PasteBlock(Rendering::Image::Component component[4])
{
	component[1].Location[0] = -component[0].Location[0];
	component[1].Location[1] =  component[0].Location[1];

	component[2].Location[0] =  component[0].Location[0];
	component[2].Location[1] = -component[0].Location[1];

	component[3].Location[0] = -component[0].Location[0];
	component[3].Location[1] = -component[0].Location[1];
}
