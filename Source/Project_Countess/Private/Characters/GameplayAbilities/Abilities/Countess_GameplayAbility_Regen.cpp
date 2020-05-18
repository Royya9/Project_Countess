// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Regen.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"

UCountess_GameplayAbility_Regen::UCountess_GameplayAbility_Regen()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Regen")));

	static ConstructorHelpers::FClassFinder<UGameplayEffect> Countess_Stamina_Regen_Effect_Class(TEXT("'/Script/Project_Countess.Countess_GE_Stamina_Regen'"));
	if (Countess_Stamina_Regen_Effect_Class.Succeeded())
	{
		Countess_Stamina_Regen_Effect = Cast<UGameplayEffect>(Countess_Stamina_Regen_Effect_Class.Class->GetDefaultObject(false));
	}
	
	static ConstructorHelpers::FClassFinder<UGameplayEffect> Countess_Health_Regen_Effect_Class(TEXT("'/Script/Project_Countess.Countess_GE_Health_Regen'"));
	if (Countess_Health_Regen_Effect_Class.Succeeded())
	{
		Countess_Health_Regen_Effect = Cast<UGameplayEffect>(Countess_Health_Regen_Effect_Class.Class->GetDefaultObject(false));
	}
}

void UCountess_GameplayAbility_Regen::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		CommitAbility(Handle, ActorInfo, ActivationInfo);
		
		if (Countess_Stamina_Regen_Effect)
		{
// 			UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get(false);
// 			AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Buff.FullStamina")));
			
			ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, Countess_Stamina_Regen_Effect, 1.f);
			ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, Countess_Health_Regen_Effect, 1.f);
			UE_LOG(Countess_Log, Warning, TEXT(" HERE!!!. Acquired Regen Ability. From %s"), TEXT(__FUNCTION__));
		}

		
	}
}

void UCountess_GameplayAbility_Regen::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCountess_GameplayAbility_Regen::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
