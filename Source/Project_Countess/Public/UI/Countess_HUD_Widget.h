// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Countess_HUD_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_HUD_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/*Events to handle Player Attributes on UI*/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthRegenRate(float HealthRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentMana(float CurrentMana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxMana(float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetManaPercentage(float ManaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetManaRegenRate(float ManaRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentStamina(float CurrentStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxStamina(float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStaminaPercentage(float StaminaPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStaminaRegenRate(float StaminaRegenRate);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetExp(float Exp);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxExp(float MaxExp);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetExpPercentage(float ExpPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPlayerLevel(int32 PlayerLevel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetArmor(float Armor);

	/*Events to handle Player Abilities' UI */

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWMagicAbilityIcon(FSlateBrush AbilityIcon);
};
