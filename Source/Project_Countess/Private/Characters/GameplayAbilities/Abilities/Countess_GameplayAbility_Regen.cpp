// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Regen.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "Player/Countess_PlayerState.h"

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
			
			uint32 Countess_PlayerLevel = 1;
			ACountess_PlayerState* Countess_PlayerState = Cast<ACountess_PlayerState>(ActorInfo->OwnerActor.Get(false));
			if (Countess_PlayerState)
				Countess_PlayerLevel = Countess_PlayerState->GetPlayerLevel();

// 			FGameplayEffectContextHandle GameplayEffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
// 			GameplayEffectContextHandle.AddSourceObject(ActorInfo->OwnerActor.Get());
// 			FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(UCountess_GE_Stamina_Regen::StaticClass(), Countess_PlayerState->GetPlayerLevel(), GameplayEffectContextHandle);
// 			if (GameplayEffectSpecHandle.IsValid())
// 			{
// 				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), GetAbilitySystemComponentFromActorInfo());
// 			}

			ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, Countess_Stamina_Regen_Effect, Countess_PlayerLevel);
			ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, Countess_Health_Regen_Effect, Countess_PlayerLevel);
			UE_LOG(Countess_Log, Warning, TEXT(" HERE!!!. Acquired Regen Ability. Player Level is %d. From %s"), Countess_PlayerLevel, TEXT(__FUNCTION__));
		}

		
	}
}

void UCountess_GameplayAbility_Regen::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCountess_GameplayAbility_Regen::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* const AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer Health_Regen_Effect_TagContainer, Stamina_Regen_Effect_TagContainer;
		Stamina_Regen_Effect_TagContainer = Countess_Stamina_Regen_Effect->InheritableGameplayEffectTags.CombinedTags;
		Health_Regen_Effect_TagContainer = Countess_Health_Regen_Effect->InheritableGameplayEffectTags.CombinedTags;
		FGameplayEffectQuery const Health_Regen_Rate_Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(Health_Regen_Effect_TagContainer);
		FGameplayEffectQuery const Stamina_Regen_Rate_Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(Stamina_Regen_Effect_TagContainer);
		AbilitySystemComponent->RemoveActiveEffects(Health_Regen_Rate_Query);
		AbilitySystemComponent->RemoveActiveEffects(Stamina_Regen_Rate_Query);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
