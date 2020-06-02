// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Calculations/Countess_GE_Damage_Execution.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

struct Countess_DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireballDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(ElectroSparkDamage);

	Countess_DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, MagicResistance, Target, false); // capture Armor of defender and don't snapshot it (don't capture at the time of Spec creation. we want this value at the time of Spec application) 

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, Health, Target, false); // capture Health of defender and don't snapshot it 

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, FireballDamage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, ElectroSparkDamage, Source, true);
	}
};

static const Countess_DamageStatics& Countess_Statics()
{
	static Countess_DamageStatics Statics;
	return Statics;
}

UCountess_GE_Damage_Execution::UCountess_GE_Damage_Execution()
{
	RelevantAttributesToCapture.Add(Countess_Statics().MagicResistanceDef);
	RelevantAttributesToCapture.Add(Countess_Statics().FireballDamageDef);
	RelevantAttributesToCapture.Add(Countess_Statics().DamageDef);
	RelevantAttributesToCapture.Add(Countess_Statics().HealthDef);
}

void UCountess_GE_Damage_Execution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics().MagicResistanceDef, EvaluationParameters, MagicResistance)); //Getting Armor Snapshot from AttributeSet
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Magic Resistance Percentage value from %s is %f"), TEXT(__FUNCTION__), MagicResistance);

	float FireballDamage = 0.f;
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics().FireballDamageDef, EvaluationParameters, FireballDamage)); // Get Health Snapshot from AttributeSet
	//
	
	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics().HealthDef, EvaluationParameters, Health);
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Health value from %s is %f"), TEXT(__FUNCTION__), Health);

	//float UnMitigatedDamage = Spec.GetModifierMagnitude(0, true); // Getting our Damage from Fireball Damage CurveTable in Countess_GE_Fireball_Damage Effect
	float UnMitigatedDamage = FireballDamage;

	//UE_LOG(Countess_Log, Warning, TEXT("UnMitigated Damage value from %s is %f"), TEXT(__FUNCTION__), UnMitigatedDamage);

	float MitigatedDamage = UnMitigatedDamage * (1 - (MagicResistance / 100)); // Damage is reduced by Magic Resistance Attribute Percentage

	//UE_LOG(Countess_Log, Warning, TEXT("Mitigated value from %s is %f"), TEXT(__FUNCTION__), MitigatedDamage);

	if (MitigatedDamage > 0.f)
	{
		// Set the Source's Damage attribute after calculation
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Countess_Statics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
	UCountess_AbilitySystemComponent* Target = Cast<UCountess_AbilitySystemComponent>(TargetASC);
	if (Target)
	{
		//UE_LOG(Countess_Log, Warning, TEXT("TargetASC is %s"), *Target->GetFName().ToString());
		Target->OnDamageReceived(Cast<UCountess_AbilitySystemComponent>(SourceASC), UnMitigatedDamage, MitigatedDamage);
	}
}
