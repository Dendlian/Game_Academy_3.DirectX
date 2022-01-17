#include "stdafx.h"
#include "StartScene.h"

void StartScene::Start()
{
	Background.Content = "StartImage";
	Background.Length = { 1500, 800 };
	Camera.Location = { 0, 0 };
}

bool StartScene::Update()
{
	Background.Draw();

	Camera.Set();

	if (Input::Get::Key::Down(VK_ESCAPE)) { return true; }
	else { return false; }
}

void StartScene::End()
{

}
