#include "PawnObject/Controller/BasePlayerController.h"
#include "EnhancedInputSubsystems.h"


ABasePlayerController::ABasePlayerController()
{
	IMC_Default = nullptr;
	IA_Look = nullptr;
	IA_Move = nullptr;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = 
		Cast<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()))
	{
		SubSystem->AddMappingContext(IMC_Default, 0);
	}
	
}
