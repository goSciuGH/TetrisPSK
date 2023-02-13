#include "Mino.h"

Mino::Mino()
{
	readyToShow = false;
}

void Mino::rotate(bool cw)
{
	if (cw) // ClockWise
	{
		int swap = posX;
		posX = posY;
		posY = 0 - swap;
	}
	else // Counter ClockWise
	{
		int swap = posX;
		posX = 0 - posY;
		posY = swap;
	}
}