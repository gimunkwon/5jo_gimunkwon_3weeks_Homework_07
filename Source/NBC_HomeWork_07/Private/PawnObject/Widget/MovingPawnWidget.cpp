#include "PawnObject/Widget/MovingPawnWidget.h"

#include "Components/Image.h"
#include "Components/RadialSlider.h"
#include "Components/TextBlock.h"
#include "PawnObject/MovingPawn.h"

void UMovingPawnWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (AMovingPawn* OwnerPawn = Cast<AMovingPawn>(GetOwningPlayerPawn()))
	{
		OwnerPawn->OnAirModeChanged.AddDynamic(this, &UMovingPawnWidget::UpdateAirModeUI);
		OwnerPawn->OnSpeedChanged.AddDynamic(this, &UMovingPawnWidget::OnSpeedChanged);
		OwnerPawn->OnSpeedValueChanged.AddDynamic(this, &UMovingPawnWidget::OnSpeedValueChanged);
	}
}

void UMovingPawnWidget::UpdateAirModeUI(bool bIsAirMode)
{
	if (Image_AirMode)
	{
		float AlphaValue = bIsAirMode ? 0.1f : 1.f;
		Image_AirMode->SetRenderOpacity(AlphaValue);
	}
}

void UMovingPawnWidget::OnSpeedChanged(float ChangeSpeedAmount)
{
	if (Slider_Tacometer)
	{
		Slider_Tacometer->SetValue(ChangeSpeedAmount);
	}
}

void UMovingPawnWidget::OnSpeedValueChanged(float ChangedSpeedValue, float MaxSpeedValue)
{
	if (Text_CurrentSpeed)
	{
		FString SpeedValue = FString::Printf(TEXT("%.1f/%.1f"),ChangedSpeedValue,MaxSpeedValue);
		Text_CurrentSpeed->SetText(FText::FromString(SpeedValue));
	}
}


