// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities//Effects/Countess_GE_Jump_Cost.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GE_Jump_Cost::UCountess_GE_Jump_Cost()
{
	//We instantly deduct stamina
	DurationPolicy = EGameplayEffectDurationType::Instant;

	if (AbilityDetailsTable)
	{
		FScalableFloat ScalableFloat = FScalableFloat(-1.f);
		FCurveTableRowHandle AbilityDetailsTableRowHandle;

		AbilityDetailsTableRowHandle.CurveTable = AbilityDetailsTable;
		AbilityDetailsTableRowHandle.RowName = FName("JumpAbilityStaminaCost");

		ScalableFloat.Curve = AbilityDetailsTableRowHandle;
		if (Countess_AttributeSet)
		{
			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = Countess_AttributeSet->GetStaminaAttribute();
			ModifierInfo.ModifierOp = EGameplayModOp::Additive;
			ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);
			Modifiers.Add(ModifierInfo);
		}

	}
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump.Cost")));
}
