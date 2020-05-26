// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "UObject/Interface.h"
#include "Countess_Interface_AbilityDetail.generated.h"

class UGameplayAbility;
class UCountess_GameplayAbility_Base;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCountess_Interface_AbilityDetail : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface that acts as bridge b/w Player State which implements this and any other class (ie PlayerController) which has a pointer to PlayerState Base Class
 */
class PROJECT_COUNTESS_API ICountess_Interface_AbilityDetail
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool Countess_Interface_TryActivateAbilityByClass(TSubclassOf<UGameplayAbility> AbilityToGive);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	void Countess_Interface_CancelAbility(TSubclassOf<UGameplayAbility>& AbilityToCancel);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool Countess_Interface_AcquireAbilitiy(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToAcquire);

	virtual FCountessAttributeChangedDelegate& GetCountessHealthChangedDelegate() = 0;
	virtual FCountessAttributeChangedDelegate& GetCountessManaChangedDelegate() = 0;
	virtual FCountessAttributeChangedDelegate& GetCountessStaminaChangedDelegate() = 0;

	virtual FCountessAttributeChangedDelegate& GetCountessMaxHealthChangedDelegate() = 0;
	virtual FCountessAttributeChangedDelegate& GetCountessMaxManaChangedDelegate() = 0;
	virtual FCountessAttributeChangedDelegate& GetCountessMaxStaminaChangedDelegate() = 0;

	virtual FCountessAttributeChangedDelegate& GetCountessHealthRegenRateChangedDelegate() = 0;
	virtual FCountessAttributeChangedDelegate& GetCountessManaRegenRateChangedDelegate() = 0;
	virtual FCountessAttributeChangedDelegate& GetCountessStaminaRegenRateChangedDelegate() = 0;

	virtual FCountessAttributeChangedDelegate& GetCountessArmorChangedDelegate() = 0;
	virtual FCountessAttributeChangedDelegate& GetCountessExpChangedDelegate() = 0;
	virtual FCountessAttributeChangedDelegate& GetCountessMaxExpChangedDelegate() = 0;

	virtual FCountessLevelChangedDelegate& GetCountessLevelChangedDelegate() = 0;

	virtual FCountessAbilityAcquiredDelegate& GetCountessAbilityAcquiredDelegate() = 0;

	virtual int32 GetPlayerLevel() const = 0;
	 
	virtual float GetCurrentHealth() const = 0;
	 
	virtual float GetMaxHealth() const = 0;
	 
	virtual float GetHealthRegenRate() const = 0;
	 
	virtual float GetArmor() const = 0;
	 
	virtual float GetCurrentMana() const = 0;
	 
	virtual float GetMaxMana() const = 0;
	 
	virtual float GetManaRegenRate() const = 0;
	 
	virtual float GetCurrentStamina() const = 0;
	 
	virtual float GetMaxStamina() const = 0;
	 
	virtual float GetStaminaRegenRate() const = 0;
	 
	virtual float GetCurrentExp() const = 0;
	 
	virtual float GetMaxExp() const = 0;

	virtual bool CanJump(TSubclassOf<UGameplayAbility>& JumpAbility) const = 0;

	virtual bool IsAlive() const = 0;

	virtual TArray<TSubclassOf<UCountess_GameplayAbility_Base>> GetAcquiredAbilities() const = 0;
};
