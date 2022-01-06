#include "stdafx.h"
#include "Game.h"
#include "Scene/TestScene.h"
#include "Scene/PlayerScene.h"

void Game::Start()
{
	Scenes.push_back(new TestScene);
	Scenes.push_back(new PlayerScene);

	Scenes.at(Now)->Start();
}

bool Game::Update()
{
	if (Scenes.at(Now)->Update())
	{
		if (++Now < Scenes.size())
		{
			Scenes.at(Now)->Start();

			return false;
		}
		else { return true; }
	}
	else { return false; }
}

void Game::End()
{
	for (Scene const* const scene : Scenes)
		delete scene;
}

Engine::Game* Engine::Initialize()
{
	return new ::Game();
}