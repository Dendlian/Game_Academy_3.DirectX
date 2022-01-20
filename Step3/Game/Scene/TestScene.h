#pragma once

#include "Manager/Scene.h"

#include "Character/Player.h"
#include "Character/Zombie.h"

#include "Item/Portion.h"
#include "Physics/CollisionProcess.h"



class TestScene final : public Scene
{
public:
	void Start()   override;
	bool Update()  override;
	void End()     override;

private:

	Rendering::Image::Component		Background;
	Rendering::Image::Component		Background2;

	Rendering::Image::Component		Map_Block1[4];
	Rendering::Image::Component		Map_Block2[13][4];
	Rendering::Image::Component		Map_Block3[25][4];

	Rendering::Text::Component		Round;
	int currentRound = 1;
	bool next_round = false;
	
	Rendering::Text::Component		Score;
	int currentScore = 0;

	Sound::Sound BGM;

	Player	Player;

	vector <class Zombie*>Zombie;
	int created_Zombies = 0;
	int current_Zombies = 0;
	int create_stack = 0;

	vector <class Portion*>Portion;
	int Portions = 0;

	Rendering::Animation::Component Door[4];
	bool dooropen = true;

	int rand_num = 0;

public:

	FORCEINLINE void SetBlock(Rendering::Image::Component & component)
	{
		component.Content = "Block";
		component.Length = { 50, 50 };
	}

	FORCEINLINE void SetBlock(Rendering::Image::Component component[4])
	{
		for (int i = 0; i < 4; i++)
		{
			component[i].Content = "Block";
			component[i].Length = { 50, 50 };
		}
	}
	void PasteBlock(Rendering::Image::Component component[4]);

};
