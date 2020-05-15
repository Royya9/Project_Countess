// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Countess_PlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessAbilityAcquiredDelegate, FSlateBrush, AbilityIcon); //Add float Cooldown, Ability Type (White Magic/Black Magic) etc..

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

public:

	ACountess_PlayerState();

	/*Overrides*/
	virtual void PostInitializeComponents() override;

	/*Setters*/

	/*Give this Ability to our PlayerState*/
	bool AcquireAbilitiy(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToAcquire);

	bool Countess_TryActivateAbilityByClass(TSubclassOf<UGameplayAbility>& AbilityToActivate);

	void Countess_CancelAbility(TSubclassOf<UGameplayAbility>& AbilityToCancel);

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


	/*Delegates to inform attribute changes to whoever is listening to*/
	FCountessAttributeChangedDelegate Countess_Health_Changed_Delegate;

	/*Delegate to inform Acquired Ability Details to whoever is listening to*/
	FCountessAbilityAcquiredDelegate Countess_Ability_Acquired_Delegate;

	/*Delegates that listen to attribute changes from AbilitySystemComponent*/
	FDelegateHandle HealthChangedDelegateHandle;
	
	/*Corresponding functions where necessary logic takes place*/
	//UFUNCTION(BlueprintCallable)
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
};
