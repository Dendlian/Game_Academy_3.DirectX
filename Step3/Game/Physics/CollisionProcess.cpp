#include "stdafx.h"
#include "CollisionProcess.h"

using namespace Rendering;

void CollisionProcess::SetObject(Animation::Component character)
{
	Object.Angle	= character.Angle;
	Object.Length[0] = character.Length[0] * 2 / 3 + 3;
	Object.Length[1] = character.Length[1] * 3 / 4 + 3;

	Object.Location = character.Location;
}

void CollisionProcess::SetObject(Rendering::Image::Component Thing)
{
	Object.Angle =	Thing.Angle;
	Object.Length[0] = Thing.Length[0];
	Object.Length[1] = Thing.Length[1];

	Object.Location = Thing.Location;
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


bool CollisionProcess::WallCollision()
{
	bool collision = false;
	for (int i = 0; i < 16; i++)
	{
		if (Collide(Object, BlockRect[i / 4][i % 4]))
			collision = true;
	}
	return collision;
}
