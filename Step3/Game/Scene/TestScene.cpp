#include "stdafx.h"
#include "TestScene.h"
#include <stdlib.h>
#include <time.h>

void TestScene::Start()
{
	Background.Content = "BackGround";
	Background.Length  = { 1200, 1200 };

	Background2.Content = "BackGround2";
	Background2.Length = { 960 * 6, 730 * 6 };

	for (int i = 0; i < 4; i++)
	{
		Door[i].Content = "Door";
		Door[i].Length = { 70, 70 };
		Door[i].Duration = 1;
		Door[i].Repeatable = true;
	}
	
	Door[0].Location = { 0, 500 };
	Door[1].Location = { 0, -500 };
	Door[2].Location = { 500, 0};
	Door[3].Location = { -500, 0 };
	
	BGM.Content = "BGM";
	BGM.volume = 0.5f;
	BGM.Play();

#pragma region Text Setting
	Round.Content = "Round : 1";
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
	Score.Color = { 0, 0, 0 };
	Score.Font.Bold = true;
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
	
	for (int i = 0; i < 4; i++)
	{
		Map_Block1[i].Draw();
	
		for (int j = 0; j < 13; j++)
			Map_Block2[j][i].Draw();
		for (int k = 0; k < 25; k++)
			Map_Block3[k][i].Draw();
	}

	if (next_round)
	{
		next_round = false;
		dooropen = true;
		currentRound += 1;
		create_stack = 0;
		created_Zombies = 0;
	}

	if (created_Zombies == (currentRound * 10)) dooropen = false;
	
	if (dooropen)
		for (int i = 0; i < 4; i++) Door[i].Door_Draw();

#pragma endregion

	if (create_stack < 5000)
		create_stack += 1;

	if ((create_stack == 4000) && (created_Zombies < (currentRound * 10)))
	{
		Zombie.push_back(new class Zombie);
		create_stack = 0;
		Zombie[current_Zombies]->Set();
		created_Zombies += 1;
		current_Zombies += 1;
	}

	for (int i = 0; i < current_Zombies; i++)
	{
		Zombie[i]->GetPlayer(Player.Coll.Player);
		Zombie[i]->Move();
	}

	for (int i = 0; i < current_Zombies; i++)
	{
		Player.GetZombie(Zombie[i]->Coll.Zombie);
		if (Zombie[i]->AttackPlayer)
			Player.GetDamage(Zombie[i]->Damage);
	}

	Player.Move();
	Player.Attack();

	if (Player.AttackZombie)
		Zombie[Player.Select_Zombie]->GetDamage(Player.ZombieDirect);
	
#pragma region Create Zombie / Portion

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
		}
	}
	if ((created_Zombies != 0) && (Zombie.empty())) next_round = true;
	
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

	// Round.Draw();
	// Score.Draw();

	if (Input::Get::Key::Down(VK_ESCAPE)) 
	{
		Player.Camera.Location = { 0, 0 };
		return true; 
	}

	if (Player.Hp <= 0) { return true; }
	else { return false; }
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


