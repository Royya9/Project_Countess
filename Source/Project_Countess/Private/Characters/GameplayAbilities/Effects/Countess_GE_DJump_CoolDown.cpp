// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_DJump_CoolDown.h"

UCountess_GE_DJump_CoolDown::UCountess_GE_DJump_CoolDown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	if (AbilityDetailsTable)
	{

		FScalableFloat ScalableFloat = FScalableFloat(1.f);
		FCurveTableRowHandle CurveTableRowHandle;

		CurveTableRowHandle.CurveTable = AbilityDetailsTable;
		CurveTableRowHandle.RowName = FName("DoubleJumpAbilityCooldown");

		ScalableFloat.Curve = CurveTableRowHandle;

		DurationMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);

	}



	//Our Data table not found. So defaulting to a hardcoded value.
	if (!AbilityDetailsTable)
	{
		//UE_LOG(Countess_Log, Warning, TEXT("AbilityDetails curve table not found in /Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails. Check if it is moved. From %s"), TEXT(__FUNCTION__));
		DurationMagnitude = FGameplayEffectModifierMagnitude(3.f);
	}

	InheritableOwnedTagsContainer.AddTag(CountessTags::FCooldownTags::DoubleJumpAbilityCooldownTag);
	InheritableGameplayEffectTags.AddTag(CountessTags::FCooldownTags::DoubleJumpAbilityCooldownTag);
}
