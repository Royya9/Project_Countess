// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"

void UCountess_AbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

void UCountess_AbilitySystemComponent::OnDamageReceived(UCountess_AbilitySystemComponent* SourceASC, float UnMitigatedDamage, float MitigatedDamage)
{
	OnDamageReceivedDelegate.Broadcast(SourceASC, UnMitigatedDamage, MitigatedDamage);
}

bool UCountess_AbilitySystemComponent::SetGameplayEffectDurationHandle(FActiveGameplayEffectHandle Handle, float NewDuration)
{
	if (!Handle.IsValid())
	{
		return false;
	}

	const FActiveGameplayEffect* ActiveGameplayEffect = GetActiveGameplayEffect(Handle);
	if (!ActiveGameplayEffect)
	{
		return false;
	}

	FActiveGameplayEffect* AGE = const_cast<FActiveGameplayEffect*>(ActiveGameplayEffect);
	if (NewDuration > 0)
	{
		AGE->Spec.Duration = NewDuration;
	}
	else
	{
		AGE->Spec.Duration = 0.01f;
	}

	AGE->StartServerWorldTime = ActiveGameplayEffects.GetServerWorldTime();
	AGE->CachedStartServerWorldTime = AGE->StartServerWorldTime;
	AGE->StartWorldTime = ActiveGameplayEffects.GetWorldTime();
	ActiveGameplayEffects.MarkItemDirty(*AGE);
	ActiveGameplayEffects.CheckDuration(Handle);

	AGE->EventSet.OnTimeChanged.Broadcast(AGE->Handle, AGE->StartWorldTime, AGE->GetDuration());
	OnGameplayEffectDurationChange(*AGE);

	return true;
}

void UCountess_AbilitySystemComponent::ScalePeriodOfAllActiveEffects(float NewScale)
{
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles = ActiveGameplayEffects.GetAllActiveEffectHandles();
	for(FActiveGameplayEffectHandle& ActiveGameplayEffectHandle: ActiveGameplayEffectHandles)
	{
		if(!ActiveGameplayEffectHandle.IsValid())
			continue;
			
		const FActiveGameplayEffect* ActiveGameplayEffect = GetActiveGameplayEffect(ActiveGameplayEffectHandle);
		if(!ActiveGameplayEffect || ActiveGameplayEffect->Spec.Period == 0.f)
			continue;

		FActiveGameplayEffect* AGE = const_cast<FActiveGameplayEffect*>(ActiveGameplayEffect);

		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		const float PeriodRemaining = TimerManager.GetTimerRemaining(AGE->PeriodHandle);
		
		// Check if it needs to execute one last time before modifying Period Timer.
		if(PeriodRemaining <= KINDA_SMALL_NUMBER && !AGE->bIsInhibited)
		{
			this->ExecutePeriodicEffect(ActiveGameplayEffectHandle);
		}
		
		TimerManager.ClearTimer(AGE->PeriodHandle);
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UCountess_AbilitySystemComponent::ExecutePeriodicEffect, ActiveGameplayEffectHandle);

		AGE->Spec.Period *= NewScale;
		// The timer manager moves things from the pending list to the active list after checking the active list on the first tick so we need to execute here
		if (AGE->Spec.Def->bExecutePeriodicEffectOnApplication)
		{
			TimerManager.SetTimerForNextTick(Delegate);
		}

		TimerManager.SetTimer(AGE->PeriodHandle, Delegate, AGE->Spec.GetPeriod(), true);

		ActiveGameplayEffects.MarkItemDirty(*AGE);
		this->OnPeriodicGameplayEffectExecuteOnSelf(this, AGE->Spec, ActiveGameplayEffectHandle);
	}
}
