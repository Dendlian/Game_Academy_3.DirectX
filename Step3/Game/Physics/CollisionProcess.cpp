#include "stdafx.h"
#include "CollisionProcess.h"

using namespace Rendering;

void CollisionProcess::SetPlayer(Animation::Component Character)
{
	Player.Angle	= Character.Angle;
	Player.Length[0] = Character.Length[0] * 2 / 3;
	Player.Length[1] = Character.Length[1] * 3 / 4;

	Player.Location[0] = Character.Location[0];
	Player.Location[1] = Character.Location[1] - 5;
}

void CollisionProcess::SetMagic(Image::Component Thing)
{
	Magic.Angle =	Thing.Angle;
	Magic.Length[0] = Thing.Length[0];
	Magic.Length[1] = Thing.Length[1];

	Magic.Location = Thing.Location;
}

void CollisionProcess::SetZombie(Animation::Component Character)
{
	Zombie.Angle = Character.Angle;
	Zombie.Length[0] = Character.Length[0] * 3 / 5;
	Zombie.Length[1] = Character.Length[1] * 2 / 3;
	Zombie.Location = Character.Location;
}

void CollisionProcess::SetBlock()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			BlockRect[i][j].Angle = 0;
	

	BlockRect[0][0].Length = { 50, 50 };
	BlockRect[0][0].Location = { 100, 100 };

	BlockRect[0][1].Length = { 50, 50 };
	BlockRect[0][1].Location = { -100, 100 };

	BlockRect[0][2].Length = { 50, 50 };
	BlockRect[0][2].Location = { 100, -100 };

	BlockRect[0][3].Length = { 50, 50 };
	BlockRect[0][3].Location = { -100, -100 };



	BlockRect[1][0].Length = { 350, 50 };
	BlockRect[1][0].Location = { 250, 400 };

	BlockRect[1][1].Length = { 350, 50 };
	BlockRect[1][1].Location = { -250, 400 };

	BlockRect[1][2].Length = { 350, 50 };
	BlockRect[1][2].Location = { 250, -400 };

	BlockRect[1][3].Length = { 350, 50 };
	BlockRect[1][3].Location = { -250, -400 };



	BlockRect[2][0].Length = { 50, 350 };
	BlockRect[2][0].Location = { 400, 250 };

	BlockRect[2][1].Length = { 50, 350 };
	BlockRect[2][1].Location = { -400, 250 };

	BlockRect[2][2].Length = { 50, 350 };
	BlockRect[2][2].Location = { 400, -250 };

	BlockRect[2][3].Length = { 50, 350 };
	BlockRect[2][3].Location = { -400, -250 };



	BlockRect[3][0].Length = { 50, 1200};
	BlockRect[3][0].Location = { 600, 0};

	BlockRect[3][1].Length = { 50, 1200 };
	BlockRect[3][1].Location = { -600, 0 };

	BlockRect[3][2].Length = { 1200, 50 };
	BlockRect[3][2].Location = { 0, 600 };

	BlockRect[3][3].Length = { 1200, 50 };
	BlockRect[3][3].Location = { 0, -600 };
}

bool CollisionProcess::WallCollision(RectAngle Object)
{
	bool collision = false;
	for (int i = 0; i < 16; i++)
	{
		if (Collide(Object, BlockRect[i / 4][i % 4]))
			collision = true;
	}
	return collision;
}
