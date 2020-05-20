// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Countess_PlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCountessAbilityAcquiredDelegate, FSlateBrush, AbilityIcon, float, Cooldown); //Add float Cooldown, Ability Type (White Magic/Black Magic) etc..

/*Forward Declarations*/
class UCountess_AbilitySystemComponent;
class UCountess_AttributeSet_Base;
class UCountess_GameplayAbility_Base;
class UGameplayAbility;

/**
 *	
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:

	UCountess_AbilitySystemComponent* AbilitySystemComponent;

	UCountess_AttributeSet_Base* AttributeSet;

	/*Array containing all our acquired abilities*/
	TArray<TSubclassOf<UCountess_GameplayAbility_Base>> AcquiredAbilities;

	/*Array containing all the abilities that are given on game start like health/mana/stamina regen etc..*/
	TArray<TSubclassOf<UCountess_GameplayAbility_Base>> StartupAbilities;

protected:

	/*Spawns at level 1. Only gaining experience increases the level*/
	UPROPERTY(EditAnywhere, Category = "PlayerLevel")
	uint32 PlayerLevel;


	bool bAbilitiesInitialized;

public:

	ACountess_PlayerState();

	/*Overrides*/
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	/*Setters*/

	/*Give this Ability to our PlayerState*/
	bool AcquireAbilitiy(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToAcquire);

	bool Countess_TryActivateAbilityByClass(TSubclassOf<UGameplayAbility>& AbilityToActivate);

	void Countess_CancelAbility(TSubclassOf<UGameplayAbility>& AbilityToCancel);

	void SetStartupAbilities(TSubclassOf<UCountess_GameplayAbility_Base>& StartupAbility);

	void GiveStartupAbilities();

	void RemoveStartupAbilities();

	void RefreshStartupAbilities();

	UFUNCTION(BlueprintCallable, Category = "Countess | Test")
	void SetPlayerLevel(int32 NewLevel);

	/*Getters*/

	/*Override from Ability System Interface*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/*Getter for Default Object from Ability Class. Has usage in classes that do not include UCountess_GameplayAbility_Base class*/
	UGameplayAbility* GetAbilityObjectFromClass(TSubclassOf<UGameplayAbility>& Ability);

	FORCEINLINE UCountess_AttributeSet_Base* GetAttributeSet() const { return AttributeSet; }

	FORCEINLINE TArray<TSubclassOf<UCountess_GameplayAbility_Base>> GetAcquiredAbilities() const { return AcquiredAbilities; }

public:
	/* Queries*/

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	bool IsAlive() const;

	//UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	uint32 GetPlayerLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Test")
	FORCEINLINE int32 GetPlayerLevel_FOR_TESTING() const { return PlayerLevel; }

	/*Checks our list of acquired abilities whether we have the ability to Jump and if yes, populates the JumpAbility class with corresponding Countess_Ability_Jump*/
	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	bool CanJump(TSubclassOf<UGameplayAbility>& JumpAbility) const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetHealthRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetArmor() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetCurrentMana() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetManaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetCurrentStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	float GetStaminaRegenRate() const;


	/*Delegates to inform attribute changes to whoever is listening to*/
	FCountessAttributeChangedDelegate Countess_Health_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_Stamina_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_Mana_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_MaxHealth_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_MaxStamina_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_MaxMana_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_HealthRegenRate_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_ManaRegenRate_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_StaminaRegenRate_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_Armor_Changed_Delegate;

	/*Delegate to inform Acquired Ability Details to whoever is listening to*/
	FCountessAbilityAcquiredDelegate Countess_Ability_Acquired_Delegate;

	/*Delegates that listen to attribute changes from AbilitySystemComponent*/
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;
	FDelegateHandle ManaRegenRateChangedDelegateHandle;
	FDelegateHandle StaminaRegenRateChangedDelegateHandle;
	FDelegateHandle ArmorChangedDelegateHandle;
	
	/*Corresponding functions where necessary logic takes place*/
	//UFUNCTION(BlueprintCallable)
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void OnManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnHealthRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void OnManaRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void OnStaminaRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void OnArmorChanged(const FOnAttributeChangeData& Data);
};
