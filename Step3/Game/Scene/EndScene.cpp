#include "stdafx.h"
#include "EndScene.h"

void EndScene::Start() 
{
	Background.Content = "GameOver";
	Background.Length = { 1500, 1000};

	Camera.Location = { 0,0 };
}

bool EndScene::Update()
{
	Camera.Set();

	Background.Draw();

	return false;
}

void EndScene::End() { }