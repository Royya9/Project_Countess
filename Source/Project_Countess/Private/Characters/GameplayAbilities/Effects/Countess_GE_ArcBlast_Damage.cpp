// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_ArcBlast_Damage.h"
#include "Characters/GameplayAbilities/Calculations/Countess_GE_ArcBlast_Dmg_Exec.h"
#include "Characters/GameplayAbilities/Calculations/Countess_CooldownModCalculation.h"

UCountess_GE_ArcBlast_Damage::UCountess_GE_ArcBlast_Damage()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	if (AbilityDetailsTable)
	{

		FScalableFloat ScalableFloat_Duration = FScalableFloat(1.f);
		FCurveTableRowHandle CurveTableRowHandle_Duration;

		CurveTableRowHandle_Duration.CurveTable = AbilityDetailsTable;
		CurveTableRowHandle_Duration.RowName = FName("ArcticBlastDuration");

		ScalableFloat_Duration.Curve = CurveTableRowHandle_Duration;
		FCustomCalculationBasedFloat CustomCalculationBasedFloat;
		CustomCalculationBasedFloat.CalculationClassMagnitude = UCountess_CooldownModCalculation::StaticClass();
		CustomCalculationBasedFloat.Coefficient = ScalableFloat_Duration;
		//CustomCalculationBasedFloat.FinalLookupCurve = CurveTableRowHandle;
		DurationMagnitude = FGameplayEffectModifierMagnitude(CustomCalculationBasedFloat);

		FGameplayEffectExecutionDefinition Definition;
		Definition.CalculationClass = UCountess_GE_ArcBlast_Dmg_Exec::StaticClass();
		this->Executions.Add(Definition);

		FScalableFloat ScalableFloat_Period = FScalableFloat(1.f);
		Period = ScalableFloat_Period;
	}

	InheritableOwnedTagsContainer.AddTag(CountessTags::FStatusTags::ArcticBlastAbilityOnTag);
	InheritableGameplayEffectTags.AddTag(CountessTags::FStatusTags::ArcticBlastAbilityOnTag);
}
