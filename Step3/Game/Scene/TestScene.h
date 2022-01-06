#pragma once
#include "Manager/Scene.h"
#include "Physics/CollisionProcess.h"

using namespace Rendering;

class TestScene final : public Scene
{
public:
	void Start()   override;
	bool Update()  override;
	void End()     override;

private:
	Rendering::Camera				Camera;

	Rendering::Image::Component		Background;

	Rendering::Animation::Component Player;

	Rendering::Image::Component		Map_Block1[4];
	
	Rendering::Image::Component		Map_Block2[13][4];
	
	Rendering::Image::Component		Map_Block3[25][4];


public:

	FORCEINLINE void SetBlock(Image::Component & component)
	{
		component.Content = "Block";
		component.Length = { 100, 100 };
	}

	FORCEINLINE void SetBlock(Image::Component component[4])
	{
		for (int i = 0; i < 4; i++)
		{
			component[i].Content = "Block";
			component[i].Length = { 100, 100 };
		}
	}

	void PasteBlock(Image::Component component[4]);
};
