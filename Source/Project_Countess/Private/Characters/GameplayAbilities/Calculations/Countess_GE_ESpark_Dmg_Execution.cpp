// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Calculations/Countess_GE_ESpark_Dmg_Execution.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

struct Countess_DamageStatics_ESpark
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireballDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(ElectroSparkDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	DECLARE_ATTRIBUTE_CAPTUREDEF(PrimaryAbilityDamage);

	Countess_DamageStatics_ESpark()
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

static const Countess_DamageStatics_ESpark& Countess_Statics_ESpark()
{
	static Countess_DamageStatics_ESpark Statics;
	return Statics;
}

UCountess_GE_ESpark_Dmg_Execution::UCountess_GE_ESpark_Dmg_Execution()
{
	RelevantAttributesToCapture.Add(Countess_Statics_ESpark().MagicResistanceDef);
	RelevantAttributesToCapture.Add(Countess_Statics_ESpark().ElectroSparkDamageDef);
	RelevantAttributesToCapture.Add(Countess_Statics_ESpark().DamageDef);
}

void UCountess_GE_ESpark_Dmg_Execution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float MagicResistance = 0.f;
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics_ESpark().MagicResistanceDef, EvaluationParameters, MagicResistance)); //Getting Armor Snapshot from AttributeSet
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Magic Resistance Percentage value from %s is %f"), TEXT(__FUNCTION__), MagicResistance);

	float ElectroSparkDamage = 0.f;
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics_ESpark().ElectroSparkDamageDef, EvaluationParameters, ElectroSparkDamage)); // Get Health Snapshot from AttributeSet
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Health value from %s is %f"), TEXT(__FUNCTION__), Health);


	//float UnMitigatedDamage = Spec.GetModifierMagnitude(0, true); // Getting our Damage from Fireball Damage CurveTable in Countess_GE_Fireball_Damage Effect
	float UnMitigatedDamage = ElectroSparkDamage;
	//UE_LOG(Countess_Log, Warning, TEXT("UnMitigated Damage value from %s is %f"), TEXT(__FUNCTION__), UnMitigatedDamage);

	float MitigatedDamage = UnMitigatedDamage * (1 - (MagicResistance / 100)); // Damage is reduced by Magic Resistance Attribute Percentage

	//UE_LOG(Countess_Log, Warning, TEXT("Mitigated value from %s is %f"), TEXT(__FUNCTION__), MitigatedDamage);

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's health attribute after calculation
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Countess_Statics_ESpark().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}
