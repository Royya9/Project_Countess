// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Mana_Regen.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GE_Mana_Regen::UCountess_GE_Mana_Regen()
{
    DurationPolicy = EGameplayEffectDurationType::Infinite;

    if (AbilityDetailsTable)
    {
		
        FScalableFloat ScalableFloat = FScalableFloat(1.f);

        FCurveTableRowHandle AbilityDetailsRowHandle;
        AbilityDetailsRowHandle.CurveTable = AbilityDetailsTable;
        AbilityDetailsRowHandle.RowName = FName("ManaRegenRate");

        ScalableFloat.Curve = AbilityDetailsRowHandle;
		
        if (Countess_AttributeSet)
        {
            FGameplayModifierInfo ModifierInfo;
            ModifierInfo.Attribute = Countess_AttributeSet->GetManaAttribute();
            ModifierInfo.ModifierOp = EGameplayModOp::Additive;
            ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);
            Modifiers.Add(ModifierInfo);
        }

        FScalableFloat ScalableFloat_Period = FScalableFloat(1.f);

        FCurveTableRowHandle AbilityDetailsRowHandle_Period;
        AbilityDetailsRowHandle_Period.CurveTable = AbilityDetailsTable;
        AbilityDetailsRowHandle_Period.RowName = FName("ManaRegenTickInterval");

        ScalableFloat_Period.Curve = AbilityDetailsRowHandle_Period;
        Period = ScalableFloat_Period;

    }
    InheritableGameplayEffectTags.AddTag(CountessTags::FEffectTags::ManaRegenTag);
    //	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Regen")));
    OngoingTagRequirements.RequireTags.AddTag(CountessTags::FStatusTags::ManaNotFullTag);
}
