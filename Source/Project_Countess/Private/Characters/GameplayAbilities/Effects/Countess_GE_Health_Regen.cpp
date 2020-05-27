// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Health_Regen.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GE_Health_Regen::UCountess_GE_Health_Regen()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	if (AbilityDetailsTable)
	{
		
		FScalableFloat ScalableFloat = FScalableFloat(1.f);

		FCurveTableRowHandle AbilityDetailsRowHandle;
		AbilityDetailsRowHandle.CurveTable = AbilityDetailsTable;
		AbilityDetailsRowHandle.RowName = FName("HealthRegenRate");

		ScalableFloat.Curve = AbilityDetailsRowHandle;
		

		//FScalableFloat ScalableFloat = FScalableFloat(Countess_AttributeSet->GetHealthRegenRate());
		if (Countess_AttributeSet)
		{
			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = Countess_AttributeSet->GetHealthAttribute();
			ModifierInfo.ModifierOp = EGameplayModOp::Additive;
			ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);
			//ModifierInfo.SourceTags.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Stamina.NotFull")));
			Modifiers.Add(ModifierInfo);
		}

		FScalableFloat ScalableFloat_Period = FScalableFloat(1.f);

		FCurveTableRowHandle AbilityDetailsRowHandle_Period;
		AbilityDetailsRowHandle_Period.CurveTable = AbilityDetailsTable;
		AbilityDetailsRowHandle_Period.RowName = FName("HealthRegenTickInterval");

		ScalableFloat_Period.Curve = AbilityDetailsRowHandle_Period;
		Period = ScalableFloat_Period;

	}
	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Health.Regen")));
//	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Regen")));
	OngoingTagRequirements.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Health.NotFull")));
}
