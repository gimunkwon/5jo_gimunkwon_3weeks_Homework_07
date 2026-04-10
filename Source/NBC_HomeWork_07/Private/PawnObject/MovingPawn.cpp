#include "NBC_HomeWork_07/Public/PawnObject/MovingPawn.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PawnObject/Controller/BasePlayerController.h"


AMovingPawn::AMovingPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComp;
	CapsuleComp->SetSimulatePhysics(false);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(CapsuleComp);
	StaticMeshComp->SetSimulatePhysics(false);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 600.f;
	SpringArmComp->bUsePawnControlRotation = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
	
}


void AMovingPawn::BeginPlay()
{
	Super::BeginPlay();
	// Controller = Cast<ABasePlayerController>(GetController());
}


void AMovingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMovingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetController()))
		{
			EnhancedInputComp->BindAction(PC->IA_Move,ETriggerEvent::Triggered, this, &AMovingPawn::Move);
			EnhancedInputComp->BindAction(PC->IA_Look,ETriggerEvent::Triggered, this, &AMovingPawn::Look);
		}
	}
	
}

void AMovingPawn::Move(const FInputActionValue& MoveValue)
{
	if (!Controller)
	{
		return;
	}
	
	UE_LOG(LogTemp,Warning,TEXT("Move"));
	
	const FVector2D MoveAmount = MoveValue.Get<FVector2D>();
	
	UE_LOG(LogTemp,Log,TEXT("MoveAmount %f"),MoveAmount.X);
	
	if (!FMath::IsNearlyZero(MoveAmount.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveAmount.X);
	}
	
	if (!FMath::IsNearlyZero(MoveAmount.Y))
	{
		AddMovementInput(GetActorForwardVector(), MoveAmount.Y);
	}
}

void AMovingPawn::Look(const FInputActionValue& LookValue)
{
	if (!Controller) return;
	
	FVector MoveAmount = LookValue.Get<FVector>();
}

