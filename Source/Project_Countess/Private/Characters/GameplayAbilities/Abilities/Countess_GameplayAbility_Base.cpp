// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "GameplayTagContainer.h"
#include "Characters/GameplayAbilities/Effects/Countess_GameplayEffect_Base.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"


UCountess_GameplayAbility_Base::UCountess_GameplayAbility_Base()
{
	//InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityType = EAbilityType::None;
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::DeadTag);
	
}

void UCountess_GameplayAbility_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}


bool UCountess_GameplayAbility_Base::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /*= nullptr*/, const FGameplayTagContainer* TargetTags /*= nullptr*/, OUT FGameplayTagContainer* OptionalRelevantTags /*= nullptr*/) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCountess_GameplayAbility_Base::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCountess_GameplayAbility_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

float UCountess_GameplayAbility_Base::GetCDTimeRemaining(const UAbilitySystemComponent* ASC) const
{
	if (ASC)
	{
		UCountess_GameplayEffect_Base* CoolDownGE = Cast<UCountess_GameplayEffect_Base>(CooldownGameplayEffectClass.GetDefaultObject());
		if(CoolDownGE)
		{
			const FGameplayTagContainer CooldownTags = CoolDownGE->InheritableOwnedTagsContainer.CombinedTags;

			FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
			TArray< float > Durations = ASC->GetActiveEffectsTimeRemaining(Query);
			if (Durations.Num() > 0)
			{
				Durations.Sort();
				return Durations[Durations.Num() - 1];
			}
		}
	}
	return 0.f;
}

void UCountess_GameplayAbility_Base::HandleAbilityDurationAndCooldownOnTimeSlowActivate(const float TimeDilationAmount,
	const float TimeRemaining, const float ActualDurationTime, float CurrentRemainingDuration,
	const FActiveGameplayEffectHandle& DurationHandle, const FGameplayTag& DurationTag)
{
	//UE_LOG(Countess_Log, Warning, TEXT("From %s. TimeDilationAmount is %f and TimeRemaining is %f"), TEXT(__FUNCTION__), TimeDilationAmount, TimeRemaining);
	UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(PlayerASC)
	{
		if(CooldownHandle.IsValid())
		{

			CurrentRemainingDuration = (TimeRemaining != ActualDurationTime) ? (CurrentRemainingDuration / UGameplayStatics::GetGlobalTimeDilation(this)) : CurrentRemainingDuration;
			float NewDuration = CurrentRemainingDuration + TimeRemaining/TimeDilationAmount;
			//UE_LOG(Countess_Log, Warning, TEXT("From %s. BloodLust Actual Remaining Cooldown is %f \n and New Cooldown is %f."), TEXT(__FUNCTION__), CurrentRemainingDuration, NewDuration);
			PlayerASC->SetGameplayEffectDurationHandle(CooldownHandle, NewDuration);
		}
		if(DurationHandle.IsValid())
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(CountessTags::FStatusTags::BloodLustAbilityOnTag);
			FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(TagContainer);
			TArray<float> TimeRemainingArray = PlayerASC->GetActiveEffectsTimeRemaining(Query);
			if(TimeRemainingArray.Num() >0)
			{
				CurrentRemainingDuration = TimeRemainingArray[0];
				float NewDuration = CurrentRemainingDuration + TimeRemaining/TimeDilationAmount;
				UE_LOG(Countess_Log, Warning, TEXT("From %s. BloodLust Remaining Duration is %f."), TEXT(__FUNCTION__), CurrentRemainingDuration);
				PlayerASC->SetGameplayEffectDurationHandle(DurationHandle, NewDuration);
			}
		}
		//PlayerASC->CountessTimeSlowActivated.RemoveDynamic(this, &UCountess_GameplayAbility_Blood::HandleDurationAndCooldownEffectsOnTimeSlow);
	}
}

