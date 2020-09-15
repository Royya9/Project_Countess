// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Calculations/Countess_GE_ArcBlast_Dmg_Exec.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

struct Countess_DamageStatics_ArcticBlast
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageMultiplier);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcticBlastDamagePerSec);

	Countess_DamageStatics_ArcticBlast()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, MagicResistance, Target, false); // capture Armor of defender and don't snapshot it (don't capture at the time of Spec creation. we want this value at the time of Spec application) 

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, DamageMultiplier, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, ArcticBlastDamagePerSec, Source, true);

	}
};

static const Countess_DamageStatics_ArcticBlast& Countess_Statics_ArcticBlast()
{
	static Countess_DamageStatics_ArcticBlast Statics;
	return Statics;
}

UCountess_GE_ArcBlast_Dmg_Exec::UCountess_GE_ArcBlast_Dmg_Exec()
{
	RelevantAttributesToCapture.Add(Countess_Statics_ArcticBlast().MagicResistanceDef);
	RelevantAttributesToCapture.Add(Countess_Statics_ArcticBlast().ArcticBlastDamagePerSecDef);
	RelevantAttributesToCapture.Add(Countess_Statics_ArcticBlast().DamageDef);
	RelevantAttributesToCapture.Add(Countess_Statics_ArcticBlast().DamageMultiplierDef);
}

void UCountess_GE_ArcBlast_Dmg_Exec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics_ArcticBlast().MagicResistanceDef, EvaluationParameters, MagicResistance)); //Getting Armor Snapshot from AttributeSet
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Magic Resistance Percentage value from %s is %f"), TEXT(__FUNCTION__), MagicResistance);

	float ArcticBlastDamagePerSec = 0.f;
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics_ArcticBlast().ArcticBlastDamagePerSecDef, EvaluationParameters, ArcticBlastDamagePerSec)); // Get Health Snapshot from AttributeSet

	float DamageMultiplierValue = 1.f;
	(ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics_ArcticBlast().DamageMultiplierDef, EvaluationParameters, DamageMultiplierValue)); // Get Health Snapshot from AttributeSet

	//float UnMitigatedDamage = Spec.GetModifierMagnitude(0, true); // Getting our Damage from Fireball Damage CurveTable in Countess_GE_Fireball_Damage Effect
	float UnMitigatedDamage = ArcticBlastDamagePerSec * DamageMultiplierValue;

	//UE_LOG(Countess_Log, Warning, TEXT("UnMitigated Damage value from %s is %f"), TEXT(__FUNCTION__), UnMitigatedDamage);

	float MitigatedDamage = UnMitigatedDamage * (1 - (MagicResistance / 100)); // Damage is reduced by Magic Resistance Attribute Percentage

	//UE_LOG(Countess_Log, Warning, TEXT("Mitigated value from %s is %f"), TEXT(__FUNCTION__), MitigatedDamage);

	if (MitigatedDamage > 0.f)
	{
		// Set the Source's Damage attribute after calculation
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Countess_Statics_ArcticBlast().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}
