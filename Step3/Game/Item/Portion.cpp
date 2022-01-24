#include "stdafx.h"
#include "Portion.h"

void Portion::Set()
{
	Portion.Location = { 200, 200 };
	Portion.Length = { 20, 20 };

	P_Image.Content = "Portion";
	P_Image.Location = { 200, 200 };
	P_Image.Length = { 30, 30 };
}

void Portion::Move(Player & player)
{
	if (((timer / 250) % 2) == 0)
		P_Image.Location[1] += 0.005f;
	else
		P_Image.Location[1] -= 0.005f;
	
	if (timer == 500) timer = 0;
	else timer += 1;

	if (player.Coll.PlayerCollision(Portion))
	{
		Portion.Location = { 100, 100 };
		player.Hp += 200;
		empty = true;
	}
	if(!empty) P_Image.Draw();
}
