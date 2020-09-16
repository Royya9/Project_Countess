// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
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
	void SelectedAbility(E_BMagic BMagicAbility);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAbilityName(E_BMagic BMagicAbility, const FText& AbilityName);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAbilityCost(E_BMagic BMagicAbility, float Cost);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetAbilityImage(E_BMagic BMagicAbility, FSlateBrush AbilityIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSlottedWMagicAbilityImage(E_WMagic WMagicAbility, FSlateBrush AbilityIcon);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSlottedWMagicAbilityCost(E_WMagic WMagicAbility, float Cost);
};
