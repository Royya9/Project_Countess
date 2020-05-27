// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Jump_CoolDown.h"

UCountess_GE_Jump_CoolDown::UCountess_GE_Jump_CoolDown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	if(AbilityDetailsTable)
	{
		//FString ContextString;
		//FSimpleCurve* myCurve = AbilityDetailsTable->FindSimpleCurve(FName("JumpAbilityCooldown"),ContextString);

		//if(myCurve)
		//	UE_LOG(LogTemp, Warning, TEXT("curve found"));
		//float val = myCurve->Eval(0.f);
		//UE_LOG(LogTemp, Warning, TEXT("key value is %f"), val);

		FScalableFloat ScalableFloat = FScalableFloat(1.f);
		FCurveTableRowHandle CurveTableRowHandle;

		CurveTableRowHandle.CurveTable = AbilityDetailsTable;
		CurveTableRowHandle.RowName = FName("JumpAbilityCooldown");

		ScalableFloat.Curve = CurveTableRowHandle;

		DurationMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);

		//UE_LOG(Countess_Log, Warning, TEXT("Value reading from curve table in %s is %f"), TEXT(__FUNCTION__), ScalableFloat.GetValueAtLevel(2.f));
	}

	

	//Our Data table not found. So defaulting to a hardcoded value.
	if(!AbilityDetailsTable)
	{
		//UE_LOG(Countess_Log, Warning, TEXT("AbilityDetails curve table not found in /Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails. Check if it is moved. From %s"), TEXT(__FUNCTION__));
		DurationMagnitude = FGameplayEffectModifierMagnitude(3.f);
	}

	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump.Cooldown")));
	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump.Cooldown")));
}
