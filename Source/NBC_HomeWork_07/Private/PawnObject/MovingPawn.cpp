#include "NBC_HomeWork_07/Public/PawnObject/MovingPawn.h"

#include "Components/CapsuleComponent.h"


AMovingPawn::AMovingPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComp;
	CapsuleComp->SetSimulatePhysics(false);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(CapsuleComp);
	StaticMeshComp->SetSimulatePhysics(false);
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

