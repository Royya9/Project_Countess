// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Countess_GameplayAbility_ArcBlas.generated.h"

class ACountess_Actor_ArcBlast;
class UAnimMontage;
class UGameplayEffect;

/**
 *  Our ArcticBlast Ability. Stuns and damages enemy for some duration
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GameplayAbility_ArcBlas : public UCountess_GameplayAbility_Base
{
	GENERATED_BODY()

public:

	UCountess_GameplayAbility_ArcBlas();

	/*Overrides*/
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const override;

	TSubclassOf<ACountess_Actor_ArcBlast> ArcticBlastClass;

	UAnimMontage* ArcticBlastAnimMontage;

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);


	TSubclassOf<UGameplayEffect> ArcticBlastDamageEffectClass;

	UFUNCTION()
	void HandleDurationAndCooldownEffectsOnTimeSlow(const float TimeDilationAmount, const float TimeRemaining, const float ActualDurationTime);
	
/*
	UFUNCTION()
	void OnArcticBlastAbilityDurationCompleted();

private:

	/ *Effect to be applied when ArcticBlast Ability is Active* /
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> ArcticBlastOnEffectClass;

	FActiveGameplayEffectHandle ArcticBlastAbilityOnEffectHandle;

	FTimerHandle ArcticBlastAbilityTimerHandle;

	UNiagaraComponent* ArcticBlastNiagaraComponent;*/
};
