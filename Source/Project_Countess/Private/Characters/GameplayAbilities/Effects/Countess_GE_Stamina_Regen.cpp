// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Stamina_Regen.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GE_Stamina_Regen::UCountess_GE_Stamina_Regen()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	//Stamina Regen Modifier 
	if (AbilityDetailsTable) // Curve Table(.csv file) which holds the regen_rate per level 
	{
		
		FScalableFloat ScalableFloat = FScalableFloat(1.f);

		FCurveTableRowHandle AbilityDetailsRowHandle;
		AbilityDetailsRowHandle.CurveTable = AbilityDetailsTable;
		AbilityDetailsRowHandle.RowName = FName("StaminaRegenRate");

		ScalableFloat.Curve = AbilityDetailsRowHandle;
		

		//FScalableFloat ScalableFloat = FScalableFloat(Countess_AttributeSet->GetStaminaRegenRate()); not working
		if (Countess_AttributeSet)
		{
			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = Countess_AttributeSet->GetStaminaAttribute();
			ModifierInfo.ModifierOp = EGameplayModOp::Additive;
			ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);
			Modifiers.Add(ModifierInfo);
		}
		
		FScalableFloat ScalableFloat_Period = FScalableFloat(1.f);

		FCurveTableRowHandle AbilityDetailsRowHandle_Period;
		AbilityDetailsRowHandle_Period.CurveTable = AbilityDetailsTable;
		AbilityDetailsRowHandle_Period.RowName = FName("StaminaRegenTickInterval");

		ScalableFloat_Period.Curve = AbilityDetailsRowHandle_Period;
		Period = ScalableFloat_Period;

		//Period = FScalableFloat(1.f);
	}

	InheritableGameplayEffectTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Stamina.Regen")));
	OngoingTagRequirements.RequireTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Stamina.NotFull")));

}
