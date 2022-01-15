#include "stdafx.h"
#include "StartScene.h"

void StartScene::Start()
{
	Background.Content = "StartImage";
	Background.Length = { 1500, 800 };

	Text1.Font.Name = "Press Any Key";
	Text1.Location = { 0, 0 };
	Text1.Length = { 100, 100 };

	Camera.Location = { 0, 0 };
}

bool StartScene::Update()
{
	Background.Draw();
	
	Text1.Draw();

	Camera.Set();

	if (Input::Get::Key::Down(VK_ESCAPE)) { return true; }
	else { return false; }
}

void StartScene::End()
{

}
