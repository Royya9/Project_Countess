// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Countess_GameplayAbility_ESpark.generated.h"

class ACountess_Actor_ElectroSpark;
class UAnimMontage;

/**
 *  Our ElectroSpark Ability
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GameplayAbility_ESpark : public UCountess_GameplayAbility_Base
{
	GENERATED_BODY()
public:

	UCountess_GameplayAbility_ESpark();

	/*Overrides*/
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;


	TSubclassOf<ACountess_Actor_ElectroSpark> ElectroSparkClass;

	UPROPERTY()
	ACountess_Actor_ElectroSpark* ElectroSparkActor;

	UPROPERTY()
	UAnimMontage* ElectroSparkMontage;

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData);


	TSubclassOf<UGameplayEffect> ElectroSparkDamageEffectClass;
	
};
