// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Countess_GameplayAbility_Lens.generated.h"

/**
 *  Our LensOfTruth Ability
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GameplayAbility_Lens : public UCountess_GameplayAbility_Base
{
	GENERATED_BODY()

public:

	UCountess_GameplayAbility_Lens();

	/*Overrides*/
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const override;

	void TogglePlatforms(const bool bAbilityActive, TArray<AActor*>& OutActors) const;

	FTimerHandle LensAbilityTimerHandle;

	UFUNCTION()
	void OnLensAbilityDurationCompleted();

private:

	UPROPERTY()
	TArray<AActor*> PlatformActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float VignetteIntensity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FieldOfView;
};

