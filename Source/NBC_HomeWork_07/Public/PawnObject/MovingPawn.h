#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MovingPawn.generated.h"

UCLASS()
class NBC_HOMEWORK_07_API AMovingPawn : public APawn
{
	GENERATED_BODY()
public:
	AMovingPawn();
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components|Mesh")
	USceneComponent* SceneRootComp;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components|Mesh")
	USkeletalMeshComponent* SkeletalMeshComp;
	
	virtual void BeginPlay() override;
public:
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
