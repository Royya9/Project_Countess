// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Countess_BMagic_Menu_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_BMagic_Menu_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentMana(float Mana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetManaPercentage(float ManaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetESparkAbilityName(const FText& ESparkAbilityText);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetESparkAbilityCost(float Cost);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFireballAbilityName(const FText& FireballAbilityText);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFireballAbilityCost(float Cost);
};
