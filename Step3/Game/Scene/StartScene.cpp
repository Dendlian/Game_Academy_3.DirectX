#include "stdafx.h"
#include "StartScene.h"

void StartScene::Start()
{
	Background.Content = "StartImage";
	Background.Length = { 1500, 800 };
	Camera.Location = { 0, 0 };

	Text1.Content = "Hello";
	Text1.Length = { 500, 200 };
	Text1.Location = { 1280 / 2 , 720 / 2 };
	Text1.Font.Name = "Cafe24";
	Text1.Font.Size = 200;
}

bool StartScene::Update()
{
	Camera.Set();
	
	Background.Draw();

	Text1.Draw();

	if (Input::Get::Key::Down(VK_ESCAPE)) { return true; }
	else { return false; }
}

void StartScene::End()
{

}
