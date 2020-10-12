// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Components/WidgetComponent.h"
#include "Countess_TimerBar_WidgetComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessTimerBarComponentDestroyedDelegate, int32, ComponentId);

class UCountess_TimerBar_Widget;
class UCountess_Timer_Component;

/**
 *  Timer Widget that appears above Player indicating how much time duration left as Progress bar and the name of ability
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_TimerBar_WidgetComp : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UCountess_TimerBar_WidgetComp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetAbilityTextAndDuration(const FText& AbilityText, const float Duration, UCountess_Timer_Component* Timer_Component = nullptr);
	
	TSubclassOf<UCountess_TimerBar_Widget> TimerBarWidgetClass;

	UCountess_TimerBar_Widget* TimerBarWidget;

	FTimerHandle TimerBarWidgetHandle;

	FCountessTimerBarComponentDestroyedDelegate TimerBarComponentDestroyedDelegate;

	UFUNCTION()
	void DestroyTimerBarWidgetComponent();

	// Set from ShowTimerBar Function in CountessPlayerController to id each timerbar
	int32 ComponentId;

	bool bIsAbilityTimeSlow;
};
