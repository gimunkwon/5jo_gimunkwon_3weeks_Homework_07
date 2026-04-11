#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MovingPawnWidget.generated.h"

class UTextBlock;
class URadialSlider;
class UImage;

UCLASS()
class NBC_HOMEWORK_07_API UMovingPawnWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_AirMode;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URadialSlider> Slider_Tacometer;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_CurrentSpeed;
	
	
	UFUNCTION()
	void UpdateAirModeUI(bool bIsAirMode);
	UFUNCTION()
	void OnSpeedChanged(float ChangeSpeedAmount);
	UFUNCTION()
	void OnSpeedValueChanged(float ChangedSpeedValue, float MaxSpeedValue);
};
