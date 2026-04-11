#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MovingPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UCapsuleComponent;

UCLASS()
class NBC_HOMEWORK_07_API AMovingPawn : public APawn
{
	GENERATED_BODY()
public:
	AMovingPawn();
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components|RootComp")
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Speed")
	float FMoveSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Speed")
	float FRotateSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Speed")
	float FRollSpeed;
	
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
	
};
