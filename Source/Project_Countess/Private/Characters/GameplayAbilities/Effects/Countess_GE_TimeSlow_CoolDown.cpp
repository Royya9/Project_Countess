// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_TimeSlow_CoolDown.h"
#include "Characters/GameplayAbilities/Calculations/Countess_CooldownModCalculation.h"

UCountess_GE_TimeSlow_CoolDown::UCountess_GE_TimeSlow_CoolDown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	if (AbilityDetailsTable)
	{
		FScalableFloat ScalableFloat = FScalableFloat(1.f);
		FCurveTableRowHandle CurveTableRowHandle;

		CurveTableRowHandle.CurveTable = AbilityDetailsTable;
		CurveTableRowHandle.RowName = FName("TimeSlowAbilityCooldown");

		ScalableFloat.Curve = CurveTableRowHandle;
		FCustomCalculationBasedFloat CustomCalculationBasedFloat;
		CustomCalculationBasedFloat.CalculationClassMagnitude = UCountess_CooldownModCalculation::StaticClass();
		CustomCalculationBasedFloat.Coefficient = ScalableFloat;
		//CustomCalculationBasedFloat.FinalLookupCurve = CurveTableRowHandle;
		DurationMagnitude = FGameplayEffectModifierMagnitude(CustomCalculationBasedFloat);
	}
	else
	{
		//Our Data table not found. So defaulting to a hardcoded value.
		DurationMagnitude = FGameplayEffectModifierMagnitude(3.f);
	}

	InheritableOwnedTagsContainer.AddTag(CountessTags::FCooldownTags::TimeSlowAbilityCooldownTag);
	InheritableGameplayEffectTags.AddTag(CountessTags::FCooldownTags::TimeSlowAbilityCooldownTag);
}
