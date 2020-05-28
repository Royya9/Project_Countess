// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Countess_SkillAcquired_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_SkillAcquired_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWidgetTitle(const FText& WidgetTitle);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWidgetDescription(const FText& WidgetDescription);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWidgetAbilityIcon(FSlateBrush WidgetAbilityIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWidgetScreenShotImage(FSlateBrush ScreenShotImage);
};
