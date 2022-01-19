#include "stdafx.h"
#include "StartScene.h"

void StartScene::Start()
{
	Background.Content = "StartImage";
	Background.Length = { 1500, 800 };
	Camera.Location = { 0, 0 };

	Text1.Content = "Press Space To Start";
	Text1.Length = { 250, 100 };
	Text1.Location = { 1280 / 2 , 600 };
	Text1.Font.Name = "Hello";
	Text1.Font.Size = 25;
	Text1.Color = { 255, 255, 255 };
}

bool StartScene::Update()
{
	Camera.Set();
	
	Background.Draw();

	turn += 1;
	if (((turn / 2000) % 2) == 0)
		Text1.Draw();

	if (turn == 4000) turn = 0;

	if (Input::Get::Key::Down(VK_SPACE)) { return true; }
	else { return false; }
}

void StartScene::End()
{

}
