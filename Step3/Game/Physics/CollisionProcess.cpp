#include "stdafx.h"
#include "CollisionProcess.h"

using namespace Rendering;

void CollisionProcess::SetPlayer(Animation::Component player)
{
	Controler.Angle	= player.Angle;
	Controler.Length	= player.Length;
	Controler.Location = player.Location;
}

void CollisionProcess::SetBlock()
{
	BlockRect[0].Angle = 0;
	BlockRect[0].Location = { 100, 100 };
	BlockRect[0].Length = { 50, 50 };

	BlockRect[1].Angle = 0;
	BlockRect[1].Location = { -100, 100 };
	BlockRect[1].Length = { 50, 50 };

	BlockRect[2].Angle = 0;
	BlockRect[2].Location = { 100, -100 };
	BlockRect[2].Length = { 50, 50 };

	BlockRect[3].Angle = 0;
	BlockRect[3].Location = { -100, -100 };
	BlockRect[3].Length = { 50, 50 };
}

bool CollisionProcess::WallCollition()
{
	bool collision = false;
	for (int i = 0; i < 14; i++)
	{
		if (Collide(Controler, BlockRect[i]))
			collision = true;
	}
	return collision;
}

/*
void CollisionProcess::SetBlock()
{
	for (int i = 0; i < 4; i++)
	{
		BlockRect[0][i].Length = { 100, 100 };
		BlockRect[1][i].Length = { 350, 50 };
		BlockRect[2][i].Length = { 50, 350 };
	}
	
	BlockRect[0][0].Location = { 100, 100 };
	BlockRect[0][1].Location = { -100, 100 };
	BlockRect[0][2].Location = { 100, -100 };
	BlockRect[0][3].Location = { -100, -100 };

	BlockRect[1][0].Location = { 250, 400 };
	BlockRect[1][0].Location = { -250, 400 };
	BlockRect[1][0].Location = { 250, -400 };
	BlockRect[1][0].Location = { -250, -400 };

	BlockRect[2][0].Location = { 400, 250 };
	BlockRect[2][0].Location = { -400, 250 };
	BlockRect[2][0].Location = { 400, -250 };
	BlockRect[2][0].Location = { -400, -250 };
	

	BlockRect[3][0].Length = { 50, 1200};
	BlockRect[3][0].Location = { 600, 0};

	BlockRect[3][1].Length = { 50, 1200 };
	BlockRect[3][1].Location = { -600, 0 };

	BlockRect[3][2].Length = { 1200, 50 };
	BlockRect[3][2].Location = { 0, 600 };

	BlockRect[3][3].Length = { 1200, 50 };
	BlockRect[3][3].Location = { 0, -600 };
}


bool CollisionProcess::WallCollition()
{
	bool collision = false;
	for (int i = 0; i < 16; i++)
	{
		if (Collide(Controler, BlockRect[i / 4][i % 4]))
			collision = true;
	}
	return collision;
}

*/