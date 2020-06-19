// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Mist_Cost.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GE_Mist_Cost::UCountess_GE_Mist_Cost()
{
	//We instantly deduct Mana
	DurationPolicy = EGameplayEffectDurationType::Instant;

	if (AbilityDetailsTable)
	{
		FScalableFloat ScalableFloat = FScalableFloat(-1.f);
		FCurveTableRowHandle AbilityDetailsTableRowHandle;

		AbilityDetailsTableRowHandle.CurveTable = AbilityDetailsTable;
		AbilityDetailsTableRowHandle.RowName = FName("MistAbilityCost");

		ScalableFloat.Curve = AbilityDetailsTableRowHandle;
		if (Countess_AttributeSet)
		{
			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = Countess_AttributeSet->GetManaAttribute();
			ModifierInfo.ModifierOp = EGameplayModOp::Additive;
			ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);
			Modifiers.Add(ModifierInfo);
		}

	}
	InheritableOwnedTagsContainer.AddTag(CountessTags::FCostTags::MistAbilityCostTag);
}
