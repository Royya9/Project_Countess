// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_ESpark_Damage.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Characters/GameplayAbilities/Calculations/Countess_GE_ESpark_Dmg_Execution.h"

UCountess_GE_ESpark_Damage::UCountess_GE_ESpark_Damage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	if (AbilityDetailsTable)
	{
		FScalableFloat ScalableFloat = FScalableFloat(1.f);
		FCurveTableRowHandle AbilityDetailsTableRowHandle;

		AbilityDetailsTableRowHandle.CurveTable = AbilityDetailsTable;
		AbilityDetailsTableRowHandle.RowName = FName("ElectroSparkAbilityDamage");

		ScalableFloat.Curve = AbilityDetailsTableRowHandle;
		if (Countess_AttributeSet)
		{
			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = Countess_AttributeSet->GetDamageAttribute();
			ModifierInfo.ModifierOp = EGameplayModOp::Override;
			ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);
			Modifiers.Add(ModifierInfo);

		}

		FGameplayEffectExecutionDefinition Definition;
		Definition.CalculationClass = UCountess_GE_ESpark_Dmg_Execution::StaticClass();
		this->Executions.Add(Definition);
	}
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Fireball.Damage")));
}
