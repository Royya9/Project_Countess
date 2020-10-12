// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Countess_GameplayAbility_Shield.generated.h"

class UNiagaraComponent;

/**
 * Our Shield Ability. Grants bonus armor during active period
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GameplayAbility_Shield : public UCountess_GameplayAbility_Base
{
	GENERATED_BODY()
	
public:

	UCountess_GameplayAbility_Shield();

	/*Overrides*/
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const override;

 	UFUNCTION()
 	void OnShieldAbilityDurationCompleted();

	UFUNCTION()
	void HandleDurationAndCooldownEffectsOnTimeSlow(const float TimeDilationAmount, const float TimeRemaining, const float ActualDurationTime);

private:

	/*Effect to be applied when Shield Ability is Active*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ShieldOnEffectClass;

	FActiveGameplayEffectHandle ShieldAbilityOnEffectHandle;

//	mutable FActiveGameplayEffectHandle CooldownHandle;
	
	FTimerHandle ShieldAbilityTimerHandle;

	UNiagaraComponent* ShieldNiagaraComponent;
};
