#include "GlobalSetiings/NBC_Gamemode.h"

#include "PawnObject/MovingPawn.h"
#include "PawnObject/Controller/BasePlayerController.h"

ANBC_Gamemode::ANBC_Gamemode()
{
	DefaultPawnClass = AMovingPawn::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
}
