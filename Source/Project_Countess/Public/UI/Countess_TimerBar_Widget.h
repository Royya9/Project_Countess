// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Countess_TimerBar_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_TimerBar_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetTimerBarWidgetText(const FText& TimerBarText);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetTimerBarDuration(float Duration);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetTimerBarPercentage(const float Percentage);

	UPROPERTY(BlueprintReadOnly)
	bool bIsAbilityTimeSlow;
	
};
