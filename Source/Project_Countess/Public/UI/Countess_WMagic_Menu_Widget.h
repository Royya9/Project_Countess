// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Countess_WMagic_Menu_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_WMagic_Menu_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentMana(float Mana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetManaPercentage(float ManaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SelectedAbility(E_WMagic WMagicAbility);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetAbilityName(E_WMagic WMagicAbility, const FText& AbilityName);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetAbilityCost(E_WMagic WMagicAbility, float Cost);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetAbilityImage(E_WMagic WMagicAbility, FSlateBrush AbilityIcon);
};
