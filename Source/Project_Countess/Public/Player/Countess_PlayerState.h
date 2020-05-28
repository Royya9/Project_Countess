// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Interfaces/Countess_Interface_AbilityDetail.h"
#include "Countess_PlayerState.generated.h"

/*Forward Declarations*/
class UCountess_AbilitySystemComponent;
class UCountess_AttributeSet_Base;
class UCountess_GameplayAbility_Base;
class UGameplayAbility;

/**
 *	Our PlayerState. Main class that implements ASI and has AttributeSet. 
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_PlayerState : public APlayerState, public IAbilitySystemInterface, public ICountess_Interface_AbilityDetail
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
	int32 PlayerLevel;

	bool bAbilitiesInitialized;

public:

	ACountess_PlayerState();

	/*Overrides*/
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	/*Setters*/

	/*Give this Ability to our PlayerState*/
	
	bool AcquireAbilitiy(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToAcquire);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool Countess_Interface_AcquireAbilitiy(TSubclassOf<UCountess_GameplayAbility_Base>& AbilityToAcquire);
	virtual bool Countess_Interface_AcquireAbilitiy_Implementation(TSubclassOf<UCountess_GameplayAbility_Base>& AbilityToAcquire) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool Countess_Interface_TryActivateAbilityByClass(TSubclassOf<UGameplayAbility>& AbilityToActivate);
	virtual bool Countess_Interface_TryActivateAbilityByClass_Implementation(TSubclassOf<UGameplayAbility>& AbilityToActivate) override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	void Countess_Interface_CancelAbility(TSubclassOf<UGameplayAbility>& AbilityToCancel);
	virtual void Countess_Interface_CancelAbility_Implementation(TSubclassOf<UGameplayAbility>& AbilityToCancel) override;

	void SetStartupAbilities(TSubclassOf<UCountess_GameplayAbility_Base>& StartupAbility);

	void GiveStartupAbilities();

	void RemoveStartupAbilities();

	void RefreshStartupAbilities();

// 	UFUNCTION(BlueprintCallable, Category = "Countess | Test")
// 	void SetPlayerLevel(int32 NewLevel);

	UFUNCTION()
	void PlayerLevelIncreased(int32 NewLevel);
	/*Getters*/

	/*Override from Ability System Interface*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/*Getter for Default Object from Ability Class. Has usage in classes that do not include UCountess_GameplayAbility_Base class*/
	UGameplayAbility* GetAbilityObjectFromClass(TSubclassOf<UGameplayAbility>& Ability);

	FORCEINLINE UCountess_AttributeSet_Base* GetAttributeSet() const { return AttributeSet; }

	virtual TArray<TSubclassOf<UCountess_GameplayAbility_Base>> GetAcquiredAbilities() const override { return AcquiredAbilities; }

public:

	bool CanDoubleJump() const;
	/* Queries. All of these are implementations of Countess_Interface_AbilityDetail*/

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual bool IsAlive() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Test")
	FORCEINLINE int32 GetPlayerLevel_FOR_TESTING() const { return PlayerLevel; }

	/*Checks our list of acquired abilities whether we have the ability to Jump and if yes, populates the JumpAbility class with corresponding Countess_Ability_Jump*/
	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual bool CanJump(TSubclassOf<UGameplayAbility>& OUTJumpAbility) const override;

	/*Checks our list of acquired abilities whether we have the ability to BackDash and if yes, populates the BackDashAbility class with corresponding Countess_Ability_BDash*/
	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual bool CanBackDash(TSubclassOf<UGameplayAbility>& OUTBackDashAbility) const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual int32 GetPlayerLevel() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetCurrentHealth() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetMaxHealth() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetHealthRegenRate() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetArmor() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetCurrentMana() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetMaxMana() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetManaRegenRate() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetCurrentStamina() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetMaxStamina() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetStaminaRegenRate() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetCurrentExp() const override;

	UFUNCTION(BlueprintCallable, Category = "Countess | Getters")
	virtual float GetMaxExp() const override;

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
	FCountessAttributeChangedDelegate Countess_Exp_Changed_Delegate;
	FCountessAttributeChangedDelegate Countess_MaxExp_Changed_Delegate;
	
	/*Delegate to inform Acquired Ability Details to whoever is listening to*/
	FCountessAbilityAcquiredDelegate Countess_Ability_Acquired_Delegate;

	/*Overrides Countess_Interface_AbilityDetail*/

	virtual FCountessAttributeChangedDelegate& GetCountessHealthChangedDelegate() override { return Countess_Health_Changed_Delegate; }
	virtual FCountessAttributeChangedDelegate& GetCountessManaChangedDelegate() override { return Countess_Mana_Changed_Delegate; }
	virtual FCountessAttributeChangedDelegate& GetCountessStaminaChangedDelegate()override { return Countess_Stamina_Changed_Delegate; }

	virtual FCountessAttributeChangedDelegate& GetCountessMaxHealthChangedDelegate() override { return Countess_MaxHealth_Changed_Delegate; }
	virtual FCountessAttributeChangedDelegate& GetCountessMaxManaChangedDelegate() override { return Countess_MaxMana_Changed_Delegate; }
	virtual FCountessAttributeChangedDelegate& GetCountessMaxStaminaChangedDelegate() override { return Countess_MaxStamina_Changed_Delegate; }

	virtual FCountessAttributeChangedDelegate& GetCountessHealthRegenRateChangedDelegate() override { return Countess_HealthRegenRate_Changed_Delegate; }
	virtual FCountessAttributeChangedDelegate& GetCountessManaRegenRateChangedDelegate() override { return Countess_ManaRegenRate_Changed_Delegate; }
	virtual FCountessAttributeChangedDelegate& GetCountessStaminaRegenRateChangedDelegate() override { return Countess_StaminaRegenRate_Changed_Delegate; }

	virtual FCountessAttributeChangedDelegate& GetCountessArmorChangedDelegate() override { return Countess_Armor_Changed_Delegate; }
	virtual FCountessAttributeChangedDelegate& GetCountessExpChangedDelegate() override { return Countess_Exp_Changed_Delegate; }
	virtual FCountessAttributeChangedDelegate& GetCountessMaxExpChangedDelegate() override { return Countess_MaxExp_Changed_Delegate; }

	virtual FCountessLevelChangedDelegate& GetCountessLevelChangedDelegate() override;
	virtual FCountessAbilityAcquiredDelegate& GetCountessAbilityAcquiredDelegate() override { return Countess_Ability_Acquired_Delegate; }

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
	virtual void OnExpChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxExpChanged(const FOnAttributeChangeData& Data);
};
