#include "NBC_HomeWork_07/Public/PawnObject/MovingPawn.h"



AMovingPawn::AMovingPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AMovingPawn::BeginPlay()
{
	Super::BeginPlay();
}


void AMovingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMovingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

