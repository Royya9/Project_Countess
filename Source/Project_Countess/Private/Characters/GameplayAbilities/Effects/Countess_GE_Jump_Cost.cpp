// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities//Effects/Countess_GE_Jump_Cost.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GE_Jump_Cost::UCountess_GE_Jump_Cost()
{
	//We instantly deduct stamina
	DurationPolicy = EGameplayEffectDurationType::Instant;

	if (AbilityDetailsTable)
	{
		UCountess_AttributeSet_Base* AttributeSet = nullptr;
		static ConstructorHelpers::FClassFinder<UCountess_AttributeSet_Base> AttributeSetClass(TEXT("'/Script/Project_Countess.Countess_AttributeSet_Base'"));
		if (AttributeSetClass.Succeeded())
		{
			AttributeSet = Cast<UCountess_AttributeSet_Base>(AttributeSetClass.Class->GetDefaultObject(false));
			if(AttributeSet)
				UE_LOG(Countess_Log, Warning, TEXT("Found AttributeSet CDO in %s"), TEXT(__FUNCTION__));
		}

		FScalableFloat ScalableFloat = FScalableFloat(-1.f);
		FCurveTableRowHandle AbilityDetailsTableRowHandle;

		AbilityDetailsTableRowHandle.CurveTable = AbilityDetailsTable;
		AbilityDetailsTableRowHandle.RowName = FName("JumpAbilityStaminaCost");

		ScalableFloat.Curve = AbilityDetailsTableRowHandle;
		if (AttributeSet)
		{
			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = AttributeSet->GetStaminaAttribute();
			ModifierInfo.ModifierOp = EGameplayModOp::Additive;
			ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloat);
			Modifiers.Add(ModifierInfo);
		}


	}
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump.Cost")));
}
