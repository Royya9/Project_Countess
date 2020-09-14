// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_BloodLust_Bonus.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Characters/GameplayAbilities/Calculations/Countess_CooldownModCalculation.h"
#include "Characters/GameplayAbilities/Calculations/Countess_GE_BloodLust_HealthPen.h"

UCountess_GE_BloodLust_Bonus::UCountess_GE_BloodLust_Bonus()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	if (AbilityDetailsTable)
	{

		FScalableFloat ScalableFloat_Duration = FScalableFloat(1.f);
		FCurveTableRowHandle CurveTableRowHandle_Duration;

		CurveTableRowHandle_Duration.CurveTable = AbilityDetailsTable;
		CurveTableRowHandle_Duration.RowName = FName("BloodLustAbilityDuration");

		ScalableFloat_Duration.Curve = CurveTableRowHandle_Duration;
		FCustomCalculationBasedFloat CustomCalculationBasedFloat;
		CustomCalculationBasedFloat.CalculationClassMagnitude = UCountess_CooldownModCalculation::StaticClass();
		CustomCalculationBasedFloat.Coefficient = ScalableFloat_Duration;
		//CustomCalculationBasedFloat.FinalLookupCurve = CurveTableRowHandle;
		DurationMagnitude = FGameplayEffectModifierMagnitude(CustomCalculationBasedFloat);

/*
		FGameplayEffectExecutionDefinition Definition;
		Definition.CalculationClass = UCountess_GE_BloodLust_Regen_Exec::StaticClass();
		this->Executions.Add(Definition);*/

		FScalableFloat ScalableFloat_DamageMul = FScalableFloat(1.f);
		FCurveTableRowHandle CurveTableRowHandle_Modifier_DamageMul;

		CurveTableRowHandle_Modifier_DamageMul.CurveTable = AbilityDetailsTable;
		CurveTableRowHandle_Modifier_DamageMul.RowName = FName("BloodLustAbilityBonusDamageMultiplier");
		ScalableFloat_DamageMul.Curve = CurveTableRowHandle_Modifier_DamageMul;

		FScalableFloat ScalableFloat_HealthPenalty = FScalableFloat(1.f);
/*
		FCurveTableRowHandle CurveTableRowHandle_Modifier_HealthPenalty;

		CurveTableRowHandle_Modifier_HealthPenalty.CurveTable = AbilityDetailsTable;
		CurveTableRowHandle_Modifier_HealthPenalty.RowName = FName("BloodLustAbilityHealthRegenPenaltyPercentage");
		ScalableFloat_HealthPenalty.Curve = CurveTableRowHandle_Modifier_HealthPenalty;*/

		if (Countess_AttributeSet)
		{
			FGameplayModifierInfo ModifierInfo_DamageMul;
			ModifierInfo_DamageMul.Attribute = Countess_AttributeSet->GetDamageMultiplierAttribute();
			ModifierInfo_DamageMul.ModifierOp = EGameplayModOp::Multiplicitive;
			ModifierInfo_DamageMul.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat_DamageMul);


			FGameplayModifierInfo ModifierInfo_HealthRegen_Cost;
			ModifierInfo_HealthRegen_Cost.Attribute = Countess_AttributeSet->GetHealthRegenRateAttribute();
			ModifierInfo_HealthRegen_Cost.ModifierOp = EGameplayModOp::Division;

			FCustomCalculationBasedFloat CustomCalculationBasedFloat_HealthPenalty;
			CustomCalculationBasedFloat_HealthPenalty.CalculationClassMagnitude = UCountess_GE_BloodLust_HealthPen::StaticClass();
			CustomCalculationBasedFloat_HealthPenalty.Coefficient = ScalableFloat_HealthPenalty;
			ModifierInfo_HealthRegen_Cost.ModifierMagnitude = FGameplayEffectModifierMagnitude(CustomCalculationBasedFloat_HealthPenalty);

			Modifiers.Add(ModifierInfo_DamageMul); // This gives bonus damage until this effect duration ends
			Modifiers.Add(ModifierInfo_HealthRegen_Cost); // This reduces our Health Regen Rate in this effect duration
		}

	}
	InheritableOwnedTagsContainer.AddTag(CountessTags::FStatusTags::BloodLustAbilityOnTag);
	InheritableGameplayEffectTags.AddTag(CountessTags::FStatusTags::BloodLustAbilityOnTag);
}