#pragma once
#include "Engine/Game.h"

class Game final : public Engine::Game
{
public:
	void Start()  override;
	bool Update() override;
	void End()    override;

public:
	std::vector<class Scene*>Scenes;

	size_t Now = 0;
};