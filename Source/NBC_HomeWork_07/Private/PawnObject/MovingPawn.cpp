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
	
	FMoveSpeed = 1.f;
	FRotateSpeed = 1.f;
	FRollSpeed = 1.f;
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
			EnhancedInputComp->BindAction(PC->IA_Rotate,ETriggerEvent::Triggered, this, &AMovingPawn::Rotate);
		}
	}
	
}

void AMovingPawn::Move(const FInputActionValue& MoveValue)
{
	if (!Controller) return;
	
	FVector2D SpeedAmount = MoveValue.Get<FVector2D>();
	
	if (!SpeedAmount.IsNearlyZero())
	{
		SpeedAmount.Normalize();
		
		FVector ForwardVector = GetActorForwardVector() * SpeedAmount.X;
		FVector RightVector = GetActorRightVector() * SpeedAmount.Y;
		
		FVector FinalVector = ForwardVector + RightVector;
		
		FVector DeltaLocation = FinalVector * FMoveSpeed * GetWorld()->GetDeltaSeconds();
		
		AddActorWorldOffset(DeltaLocation);
	}
}

void AMovingPawn::Look(const FInputActionValue& LookValue)
{
	if (!Controller) return;
	
	const FVector2D LookAxisValue = LookValue.Get<FVector2D>();
	
	if (!LookAxisValue.IsNearlyZero())
	{
		float YawAmount = LookAxisValue.X * FRotateSpeed;
		float PitchAmount = LookAxisValue.Y * FRotateSpeed;
		
		FRotator CombineRotation(PitchAmount,YawAmount,0.f);
		
		AddActorLocalRotation(CombineRotation);
	}
}

void AMovingPawn::Rotate(const FInputActionValue& RollAmout)
{
	if (!Controller) return;
	
	float RollValue = RollAmout.Get<float>();
	
	if (!FMath::IsNearlyZero(RollValue))
	{
		RollValue *= FRollSpeed * GetWorld()->GetDeltaSeconds();
		
		FRotator FinalRotateValue(0.f,0.f,RollValue); 
		
		AddActorLocalRotation(FinalRotateValue);
	}
	
}

