#include "GlobalSetiings/NBC_Gamemode.h"

#include "PawnObject/MovingPawn.h"

ANBC_Gamemode::ANBC_Gamemode()
{
	DefaultPawnClass = AMovingPawn::StaticClass();
}
