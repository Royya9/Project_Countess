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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // #TODO Show cost for all levels instead of only one level 
	void SetCost(float Cost);

	//CostName = Mana or Stamina or Health etc
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCostName(FName CostName);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCooldown(float Cooldown);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamage(float Damage);
};
