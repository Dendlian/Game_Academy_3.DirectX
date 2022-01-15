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

	Player	Player;

	Zombie	Zombie[2];
	
	
	Portion Portion;

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
