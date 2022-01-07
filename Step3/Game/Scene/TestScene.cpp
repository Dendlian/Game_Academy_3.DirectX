#include "stdafx.h"
#include "TestScene.h"

void TestScene::Start()
{
	Background.Content = "BackGround";
	Background.Length  = { 1200, 1200 };

	Background2.Content = "BackGround2";
	Background2.Length = { 960 * 6, 730 * 6 };

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

	Player.SetCharacter();
}


bool TestScene::Update()
{
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

	Player.Move();
	Player.Attack();

	if (Input::Get::Key::Down(VK_ESCAPE)) { return true; }
	else                                  { return false; }
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


