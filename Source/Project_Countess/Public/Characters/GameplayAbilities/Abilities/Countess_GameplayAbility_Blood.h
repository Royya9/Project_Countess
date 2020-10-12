// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Countess_GameplayAbility_Blood.generated.h"

class UNiagaraComponent;

/**
 *  Our BloodLust Ability. Sacrifices a percentage of current health and health regen rate to gain bonus damage
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GameplayAbility_Blood : public UCountess_GameplayAbility_Base
{
	GENERATED_BODY()

public:

	UCountess_GameplayAbility_Blood();

	/*Overrides*/
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const override;

	//virtual float GetCDTimeRemaining(const UAbilitySystemComponent* ASC) const override;
	
	UFUNCTION()
	void OnBloodLustAbilityDurationCompleted();

	UFUNCTION()
	void HandleDurationAndCooldownEffectsOnTimeSlow(const float TimeDilationAmount, const float TimeRemaining, const float ActualDurationTime);

private:

	/*Effect to be applied when BloodLust Ability is Active*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> BloodLustOnEffectClass;

	FActiveGameplayEffectHandle BloodLustAbilityOnEffectHandle;

//	mutable FActiveGameplayEffectHandle CooldownHandle;

	FTimerHandle BloodLustAbilityTimerHandle;

	UNiagaraComponent* BloodLustNiagaraComponent;
	
};
