#include "stdafx.h"
#include "Magic.h"

void Magic::Set()
{
	F_Image.Content = "Fireball";
	F_Image.Angle = 0;
	F_Image.Length = { 10, 10 };
	
	Wall.SetBlock();
}

void Magic::Move()
{
	Wall.SetObject(F_Image);

	Vector<2> direction;

	switch (F_Direction)
	{
	case 1:
		direction[1] -= 1;
		break;
	case 2:
		direction[0] -= 1;
		break;
	case 3:
		direction[0] += 1;
		break;
	case 4:
		direction[1] += 1;
		break;
	case 5:
		direction[0] -= 1;
		direction[1] -= 1;
		break;
	case 6:
		direction[0] += 1;
		direction[1] -= 1;
		break;
	case 7:
		direction[0] -= 1;
		direction[1] += 1;
		break;
	case 8:
		direction[0] += 1;
		direction[1] += 1;
		break;
	}
	
	if (!Wall.WallCollision())
	{
		F_Image.Location += Normalize(direction) * 2000 * Time::Get::Delta();
		F_Image.Draw();
	}
	else
	{
		
	}
}
