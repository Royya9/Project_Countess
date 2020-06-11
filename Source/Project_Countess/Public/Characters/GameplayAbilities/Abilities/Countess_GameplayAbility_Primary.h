// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Countess_GameplayAbility_Primary.generated.h"

class UAnimMontage;
class UGameplayEffect;

/**
 *  Our primary Attack Ability (Use swords!)
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GameplayAbility_Primary : public UCountess_GameplayAbility_Base
{
	GENERATED_BODY()

public:

	UCountess_GameplayAbility_Primary();

	/*Overrides*/
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	UFUNCTION()
    void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UPROPERTY()
	UAnimMontage* PrimaryAttackAnimMontage;

	TSubclassOf<UGameplayEffect> DamageEffectClass;
};