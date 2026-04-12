#include "NBC_HomeWork_07/Public/PawnObject/MovingPawn.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PawnObject/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"


AMovingPawn::AMovingPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComp;
	BoxComp->SetSimulatePhysics(false);
	BoxComp->SetCollisionProfileName(FName("Pawn"));
	
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComp->SetupAttachment(BoxComp);
	SkeletalMeshComp->SetSimulatePhysics(false);
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 600.f;
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bDoCollisionTest = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
	
	FMoveSpeed = 1.f;
	FRotateSpeed = 1.f;
	FRollSpeed = 1.f;
	FSlowSpeed = 0.5f;
	VerticalVelocity = 0.f;
	
	bIsMove = false;
	
	OnAirModeChanged.AddDynamic(this, &AMovingPawn::PlayFlyingSound);
	OnAirModeChanged.AddDynamic(this, &AMovingPawn::SetDefaultCamRotate);
}

void AMovingPawn::BeginPlay()
{
	Super::BeginPlay();
	if (SpeedWidgetClass)
	{
		SpeedWidget = CreateWidget<UUserWidget>(GetWorld(),SpeedWidgetClass);
		if (SpeedWidget)
		{
			SpeedWidget->AddToViewport();
		}
	}
	OriginCamRotation = SpringArmComp->GetRelativeRotation();
}

void AMovingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsGround)
	{
		VerticalVelocity += GravityAccelerationValue * DeltaTime;
	}
	GravityAcceleration(DeltaTime);
	
	float TargetSpeed = bIsMove ? 1.f : 0.f;
	
	if (OnSpeedChanged.IsBound())
	{
		OnSpeedChanged.Broadcast(Speed / FMoveSpeed * TargetSpeed);
	}
	if (OnSpeedValueChanged.IsBound())
	{
		OnSpeedValueChanged.Broadcast(Speed * TargetSpeed, FMoveSpeed);
	}
	bIsMove = false;
	
	
	FString bOnGround = bIsGround ? TEXT("Ground") : TEXT("Sky");
	
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, bOnGround);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("중력가속도 %f"),VerticalVelocity));
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
	
	FVector SpeedVector = MoveValue.Get<FVector>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	if (!SpeedVector.IsNearlyZero())
	{
		bIsMove = true;
		SpeedVector.Normalize();
		
		FVector ForwardVector = GetActorForwardVector();
		FVector RightVector = GetActorRightVector();
		
		if (bIsGround)
		{
			ForwardVector.Z = 0.f;
			RightVector.Z = 0.f;
			ForwardVector.Normalize();
			RightVector.Normalize();
		}
		
		FVector HorizonVector = (ForwardVector * SpeedVector.X + RightVector * SpeedVector.Y) * FMoveSpeed * DeltaTime;
		FVector PureVelocity = (ForwardVector * SpeedVector.X + RightVector * SpeedVector.Y) * FMoveSpeed;
		
		if (!bIsGround)
		{
			HorizonVector *= FSlowSpeed;
			Speed = PureVelocity.Length() * FSlowSpeed;
		}
		else
		{
			Speed = PureVelocity.Length();
		}
		
		AddActorWorldOffset(HorizonVector, true);
		
		if (SpeedVector.Z > 0.f)
		{
			VerticalVelocity = FMoveSpeed;
			bIsGround = false;
		}
		else if (SpeedVector.Z < 0.f && !bIsGround)
		{
			VerticalVelocity -= FMoveSpeed * DeltaTime;
		}
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
		
		if (bIsGround)
		{
			if (SpringArmComp)
			{
				FRotator SetArmRotation = SpringArmComp->GetRelativeRotation();
				SetArmRotation.Pitch += PitchAmount;
				SetArmRotation.Pitch = FMath::Clamp(SetArmRotation.Pitch, -80.f, 80.f);
				
				SpringArmComp->SetRelativeRotation(SetArmRotation);
			}
			
			AddActorLocalRotation(FRotator(0.f, YawAmount, 0.f));
			
			FRotator CurrentRotation = GetActorRotation();
			if (!FMath::IsNearlyZero(CurrentRotation.Pitch) || !FMath::IsNearlyZero(CurrentRotation.Roll))
			{
				SetActorRotation(FRotator(0.f, CurrentRotation.Yaw, 0.f));
			}
		}
		else
		{
			AddActorLocalRotation(FRotator(PitchAmount,YawAmount,0.f));
		}
		
	}
}

void AMovingPawn::Rotate(const FInputActionValue& RollAmout)
{
	if (!Controller) return;
	
	if (bIsGround) return;
	
	float RollValue = RollAmout.Get<float>();
	
	if (!FMath::IsNearlyZero(RollValue))
	{
		RollValue *= FRollSpeed * GetWorld()->GetDeltaSeconds();
		
		FRotator FinalRotateValue(0.f,0.f,RollValue); 
		
		AddActorLocalRotation(FinalRotateValue);
	}
	
}

void AMovingPawn::GravityAcceleration(float DeltaTime)
{
	FVector Offset(0.f, 0.f, VerticalVelocity * DeltaTime);
	
	FHitResult MoveHit;
	AddActorWorldOffset(Offset, true, &MoveHit);
    
	FVector Start = GetActorLocation();
	
	float HalfHeight = BoxComp->GetScaledBoxExtent().Z;
	float TraceDistance = 70.0f;
	
	FVector SenseEnd = Start - FVector(0.f, 0.f, HalfHeight + TraceDistance);
    
	FCollisionQueryParams IgnoreParams;
	IgnoreParams.AddIgnoredActor(this);
	
	FHitResult GroundHit;
	bool bOnGround = GetWorld()->LineTraceSingleByChannel(
		GroundHit, 
		Start, 
		SenseEnd, 
		ECC_Visibility, 
		IgnoreParams
	);
	
	FColor DebugColor = bOnGround ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, SenseEnd, DebugColor, false, -1.f, 0, 2.f);
	
	if (bOnGround || MoveHit.IsValidBlockingHit())
	{
		if (VerticalVelocity < 0.f)
		{
			VerticalVelocity = 0.f;
			bIsGround = true;
			OnAirModeChanged.Broadcast(bIsGround);
			FallingInterp();
		}
	}
	else
	{
		bIsGround = false;
		OnAirModeChanged.Broadcast(bIsGround);
	}
}

void AMovingPawn::FallingInterp()
{
	if (bIsGround)
	{
		FRotator CurrentRotation = GetActorRotation();
		if (!FMath::IsNearlyZero(CurrentRotation.Pitch) || !FMath::IsNearlyZero(CurrentRotation.Roll))
		{
			SetActorRotation(FRotator(0.f,CurrentRotation.Yaw,0.f));
		}
	}
}

void AMovingPawn::PlayFlyingSound(bool bOffGround)
{
	if (!bOffGround)
	{
		if (AudioComp && AudioComp->IsPlaying())
			return;
		
		if (FlyingSound)
		{
			AudioComp = UGameplayStatics::SpawnSound2D(this, FlyingSound);
		}
	}
	else
	{
		if (AudioComp && AudioComp->IsPlaying())
		{
			AudioComp->Stop();
		}
	}
}

void AMovingPawn::SetDefaultCamRotate(bool bNotGround)
{
	if (!bNotGround)
	{
		SpringArmComp->SetRelativeRotation(OriginCamRotation);
	}
}




