// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Regen.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "Characters/Countess_Character_Base.h"
#include "Player/Countess_PlayerState.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Health_Regen.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Mana_Regen.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Stamina_Regen.h"

UCountess_GameplayAbility_Regen::UCountess_GameplayAbility_Regen()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(CountessTags::FAbilityTags::RegenAbilityTag);

	Countess_Health_Regen_Effect = UCountess_GE_Health_Regen::StaticClass();
	Countess_Mana_Regen_Effect = UCountess_GE_Mana_Regen::StaticClass();
	Countess_Stamina_Regen_Effect = UCountess_GE_Stamina_Regen::StaticClass();
	
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

		ACountess_Character_Base* Character_Base = Cast<ACountess_Character_Base>(ActorInfo->AvatarActor.Get(false));
		if(!Character_Base)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}
		const int32 CharacterLevel = Character_Base->GetCharacterLevel();
		
		ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, Countess_Health_Regen_Effect.GetDefaultObject(), CharacterLevel);
		ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, Countess_Mana_Regen_Effect.GetDefaultObject(), CharacterLevel);
		ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, Countess_Stamina_Regen_Effect.GetDefaultObject(), CharacterLevel);
		
		/*if (Countess_Stamina_Regen_Effect)
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
			//UE_LOG(Countess_Log, Warning, TEXT(" HERE!!!. Acquired Regen Ability. Player Level is %d. From %s"), Countess_PlayerLevel, TEXT(__FUNCTION__));
		}*/

		
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
		const FGameplayTagContainer Stamina_Regen_Effect_TagContainer = Countess_Stamina_Regen_Effect.GetDefaultObject()->
		                                                                                        InheritableGameplayEffectTags
		                                                                                        .CombinedTags;
		const FGameplayTagContainer Health_Regen_Effect_TagContainer = Countess_Health_Regen_Effect.GetDefaultObject()->
		                                                                                      InheritableGameplayEffectTags.
		                                                                                      CombinedTags;
		const FGameplayTagContainer Mana_Regen_Effect_TagContainer = Countess_Mana_Regen_Effect.GetDefaultObject()->
                                                                                      InheritableGameplayEffectTags.
                                                                                      CombinedTags;
		FGameplayEffectQuery const Health_Regen_Rate_Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(Health_Regen_Effect_TagContainer);
		FGameplayEffectQuery const Stamina_Regen_Rate_Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(Stamina_Regen_Effect_TagContainer);
		FGameplayEffectQuery const Mana_Regen_Rate_Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(Mana_Regen_Effect_TagContainer);
		AbilitySystemComponent->RemoveActiveEffects(Health_Regen_Rate_Query);
		AbilitySystemComponent->RemoveActiveEffects(Stamina_Regen_Rate_Query);
		AbilitySystemComponent->RemoveActiveEffects(Mana_Regen_Rate_Query);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
