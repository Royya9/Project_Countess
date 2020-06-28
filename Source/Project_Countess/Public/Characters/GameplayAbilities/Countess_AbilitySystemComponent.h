// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "AbilitySystemComponent.h"
#include "Countess_AbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageReceivedDelegate, class UCountess_AbilitySystemComponent*, SourceASC, float, UnMitigatedDamage, float, MitigatedDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAbilityOnCooldown, class UGameplayAbility*, Ability, float, CooldownDuration);

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	FOnDamageReceivedDelegate OnDamageReceivedDelegate;
	FAbilityOnCooldown AbilityOnCooldown;

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	virtual void OnDamageReceived(UCountess_AbilitySystemComponent* SourceASC, float UnMitigatedDamage, float MitigatedDamage);
};
