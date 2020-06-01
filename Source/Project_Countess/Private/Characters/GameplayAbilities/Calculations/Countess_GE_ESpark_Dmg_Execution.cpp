// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Calculations/Countess_GE_ESpark_Dmg_Execution.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"



UCountess_GE_ESpark_Dmg_Execution::UCountess_GE_ESpark_Dmg_Execution()
{
	RelevantAttributesToCapture.Add(Countess_Statics().MagicResistanceDef);
	RelevantAttributesToCapture.Add(Countess_Statics().HealthDef);
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
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics().MagicResistanceDef, EvaluationParameters, MagicResistance)); //Getting Armor Snapshot from AttributeSet
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Magic Resistance Percentage value from %s is %f"), TEXT(__FUNCTION__), MagicResistance);

	float Health = 0.f;
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics().HealthDef, EvaluationParameters, Health)); // Get Health Snapshot from AttributeSet
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Health value from %s is %f"), TEXT(__FUNCTION__), Health);


	float UnMitigatedDamage = Spec.GetModifierMagnitude(0, true); // Getting our Damage from Fireball Damage CurveTable in Countess_GE_Fireball_Damage Effect

	//UE_LOG(Countess_Log, Warning, TEXT("UnMitigated Damage value from %s is %f"), TEXT(__FUNCTION__), UnMitigatedDamage);

	float MitigatedDamage = UnMitigatedDamage * (1 - (MagicResistance / 100)); // Damage is reduced by Magic Resistance Attribute Percentage

	//UE_LOG(Countess_Log, Warning, TEXT("Mitigated value from %s is %f"), TEXT(__FUNCTION__), MitigatedDamage);

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's health attribute after calculation
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Countess_Statics().HealthProperty, EGameplayModOp::Additive, -MitigatedDamage));
	}
}
