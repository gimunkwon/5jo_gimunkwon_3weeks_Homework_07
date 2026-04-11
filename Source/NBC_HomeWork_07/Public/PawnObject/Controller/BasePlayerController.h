#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class NBC_HOMEWORK_07_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABasePlayerController();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Input|InputMappingContext")
	TObjectPtr<UInputMappingContext> IMC_Default;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Input|InputAction")
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Input|InputAction")
	TObjectPtr<UInputAction> IA_Look;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Input|InputAction")
	TObjectPtr<UInputAction> IA_Rotate;
	
protected:
	virtual void BeginPlay() override;
};
