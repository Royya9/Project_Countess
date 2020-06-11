// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Calculations/Countess_GE_Primary_Execution.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

struct Countess_DamageStatics_Primary
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireballDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(ElectroSparkDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	DECLARE_ATTRIBUTE_CAPTUREDEF(PrimaryAbilityDamage);

	Countess_DamageStatics_Primary()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, MagicResistance, Target, false); // capture Armor of defender and don't snapshot it (don't capture at the time of Spec creation. we want this value at the time of Spec application) 

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, Health, Target, false); // capture Health of defender and don't snapshot it 

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, FireballDamage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, ElectroSparkDamage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, PrimaryAbilityDamage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, Armor, Target, false);
	}
};

static const Countess_DamageStatics_Primary& Countess_Statics_Primary()
{
	static Countess_DamageStatics_Primary Statics;
	return Statics;
}

UCountess_GE_Primary_Execution::UCountess_GE_Primary_Execution()
{
    RelevantAttributesToCapture.Add(Countess_Statics_Primary().ArmorDef);
    RelevantAttributesToCapture.Add(Countess_Statics_Primary().DamageDef);
	RelevantAttributesToCapture.Add(Countess_Statics_Primary().PrimaryAbilityDamageDef);
}

void UCountess_GE_Primary_Execution::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.f;
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics_Primary().ArmorDef, EvaluationParameters, Armor)); //Getting Armor Snapshot from AttributeSet
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Armor value from %s is %f"), TEXT(__FUNCTION__), Armor);

	float PrimaryAbilityDamage = 0.f;
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics_Primary().PrimaryAbilityDamageDef, EvaluationParameters, PrimaryAbilityDamage)); // Get Health Snapshot from AttributeSet
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Unmitigated Damage from %s is %f"), TEXT(__FUNCTION__), PrimaryAbilityDamage);


	//float UnMitigatedDamage = Spec.GetModifierMagnitude(0, true); // Getting our Damage from Fireball Damage CurveTable in Countess_GE_Fireball_Damage Effect
	const float UnMitigatedDamage = PrimaryAbilityDamage;
	//UE_LOG(Countess_Log, Warning, TEXT("UnMitigated Damage value from %s is %f"), TEXT(__FUNCTION__), UnMitigatedDamage);
	const float PhysicalDamageResistance = (0.052 * Armor)/(0.9 + 0.048 * Armor);
	const float MitigatedDamage = UnMitigatedDamage * (1 - PhysicalDamageResistance); // Damage is reduced by Physical Damage Resistance

	//UE_LOG(Countess_Log, Warning, TEXT("Mitigated value from %s is %f"), TEXT(__FUNCTION__), MitigatedDamage);

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's health attribute after calculation
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Countess_Statics_Primary().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}
