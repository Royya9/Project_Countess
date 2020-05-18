// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Countess_GameplayAbility_Regen.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GameplayAbility_Regen : public UCountess_GameplayAbility_Base
{
	GENERATED_BODY()
	
private:

	UGameplayEffect* Countess_Stamina_Regen_Effect;
	UGameplayEffect* Countess_Health_Regen_Effect;

public:

	UCountess_GameplayAbility_Regen();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};
