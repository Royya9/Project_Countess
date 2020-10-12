// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Shield_Bonus.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Characters/GameplayAbilities/Calculations/Countess_CooldownModCalculation.h"

UCountess_GE_Shield_Bonus::UCountess_GE_Shield_Bonus()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	if (AbilityDetailsTable)
	{
		FScalableFloat ScalableFloat = FScalableFloat(1.f);
		FCurveTableRowHandle AbilityDetailsTableRowHandle;

		AbilityDetailsTableRowHandle.CurveTable = AbilityDetailsTable;
		AbilityDetailsTableRowHandle.RowName = FName("ShieldBonusArmor");

		ScalableFloat.Curve = AbilityDetailsTableRowHandle;
		if (Countess_AttributeSet)
		{
			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = Countess_AttributeSet->GetArmorAttribute();
			ModifierInfo.ModifierOp = EGameplayModOp::Additive;
			ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);
			Modifiers.Add(ModifierInfo);
		}

		FScalableFloat ScalableFloat_Duration = FScalableFloat(1.f);
		FCurveTableRowHandle CurveTableRowHandle_Duration;

		CurveTableRowHandle_Duration.CurveTable = AbilityDetailsTable;
		CurveTableRowHandle_Duration.RowName = FName("ShieldAbilityDuration");

		/*
		ScalableFloat.Curve = CurveTableRowHandle_Duration;
		FCustomCalculationBasedFloat CustomCalculationBasedFloat;
		CustomCalculationBasedFloat.CalculationClassMagnitude = UCountess_CooldownModCalculation::StaticClass();
		CustomCalculationBasedFloat.Coefficient = ScalableFloat;
		//CustomCalculationBasedFloat.FinalLookupCurve = CurveTableRowHandle;
		DurationMagnitude = FGameplayEffectModifierMagnitude(CustomCalculationBasedFloat);
		*/

		FSetByCallerFloat SetByCallerFloat;
		SetByCallerFloat.DataTag = CountessTags::FStatusTags::ShieldAbilityOnTag;
		DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCallerFloat);
	}
	InheritableOwnedTagsContainer.AddTag(CountessTags::FStatusTags::ShieldAbilityOnTag);
	InheritableGameplayEffectTags.AddTag(CountessTags::FStatusTags::ShieldAbilityOnTag);
}
