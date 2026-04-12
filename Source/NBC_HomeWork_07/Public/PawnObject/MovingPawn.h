#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "MovingPawn.generated.h"

class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UCapsuleComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAirModeChanged,bool, bIsAirMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeedChanged,float, SpeedAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpeedValueChanged, float, CurrentSpeed, float, MaxSpeed);

UCLASS()
class NBC_HOMEWORK_07_API AMovingPawn : public APawn
{
	GENERATED_BODY()
public:
	AMovingPawn();
	
	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnAirModeChanged OnAirModeChanged;
	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnSpeedChanged OnSpeedChanged;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnSpeedValueChanged OnSpeedValueChanged;
	
	FORCEINLINE float GetMaxSpeed() const {return FMoveSpeed;}
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components|RootComp")
	TObjectPtr<UBoxComponent> BoxComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TSubclassOf<UUserWidget> SpeedWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> SpeedWidget;
 	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Speed")
	float FMoveSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Speed")
	float FRotateSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Speed")
	float FRollSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Speed")
	float FSlowSpeed;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="State")
	bool bIsGround;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Sound")
	TObjectPtr<USoundBase> FlyingSound;
	
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	UFUNCTION()
	void Move(const FInputActionValue& MoveValue);
	UFUNCTION()
	void Look(const FInputActionValue& LookValue);
	UFUNCTION()
	void Rotate(const FInputActionValue& RollAmout);
	
	void GravityAcceleration(float DeltaTime);
	void FallingInterp();
	
	UFUNCTION()
	void PlayFlyingSound(bool bOffGround);
	
	UFUNCTION()
	void SetDefaultCamRotate(bool bNotGround);
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComp;
private:
	const float GravityAccelerationValue = -980.f;
	float VerticalVelocity;
	float Speed;
	
	bool bIsMove;
	
	FRotator OriginCamRotation;
};
