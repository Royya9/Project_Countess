// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Regen.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GE_Regen::UCountess_GE_Regen()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	

	if (AbilityDetailsTable)
	{
		FScalableFloat ScalableFloat = FScalableFloat(1.f);

		FCurveTableRowHandle AbilityDetailsRowHandle;
		AbilityDetailsRowHandle.CurveTable = AbilityDetailsTable;
		AbilityDetailsRowHandle.RowName = FName("StaminaRegenRate");

		ScalableFloat.Curve = AbilityDetailsRowHandle;

		if (Countess_AttributeSet)
		{
			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = Countess_AttributeSet->GetStaminaAttribute();
			ModifierInfo.ModifierOp = EGameplayModOp::Additive;
			ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);

			Modifiers.Add(ModifierInfo);
		}
		// #TODO Get this hardcoded period value into a curve table or something
		Period = FScalableFloat(0.5f);
	}
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Regen")));
	OngoingTagRequirements.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Stamina.NotFull")));

}
