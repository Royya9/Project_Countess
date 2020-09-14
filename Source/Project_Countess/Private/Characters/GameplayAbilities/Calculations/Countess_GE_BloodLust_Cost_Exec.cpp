// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Calculations/Countess_GE_BloodLust_Cost_Exec.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Engine/CurveTable.h"

struct Countess_DamageStatics_BloodLust_Cost
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	Countess_DamageStatics_BloodLust_Cost()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCountess_AttributeSet_Base, Health, Source, true); // capture Health of Source and snapshot it 
	}
};

static const Countess_DamageStatics_BloodLust_Cost& Countess_Statics_BloodLust_Cost()
{
	static Countess_DamageStatics_BloodLust_Cost Statics;
	return Statics;
}

UCountess_GE_BloodLust_Cost_Exec::UCountess_GE_BloodLust_Cost_Exec()
{
	RelevantAttributesToCapture.Add(Countess_Statics_BloodLust_Cost().HealthDef);

	static ConstructorHelpers::FObjectFinder<UCurveTable> AbilityDetailsTableHelper(TEXT("CurveTable'/Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails.AbilityDetails'"));
	if (AbilityDetailsTableHelper.Succeeded())
	{
		BloodLustCostRowHandle.CurveTable = AbilityDetailsTableHelper.Object;
		BloodLustCostRowHandle.RowName = FName("BloodLustAbilityHealthCostPercentage");
	}
	else
	{
		UE_LOG(Countess_Log, Error, TEXT("Cannot find ability details table from %s."), TEXT(__FUNCTION__));
	}
}

void UCountess_GE_BloodLust_Cost_Exec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;

	float CurrentHealthValue = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Countess_Statics_BloodLust_Cost().HealthDef, EvaluationParameters, CurrentHealthValue);
	//UE_LOG(Countess_Log, Warning, TEXT("Captured Magic Resistance Percentage value from %s is %f"), TEXT(__FUNCTION__), MagicResistance);

	float Multiplier = 100.f;
	const FString ContextString;

	if (BloodLustCostRowHandle.IsValid(ContextString))
	{
		UE_LOG(Countess_Log, Warning, TEXT("From %s. Player Level is %f"), TEXT(__FUNCTION__), Spec.GetLevel());
		Multiplier = BloodLustCostRowHandle.Eval(Spec.GetLevel(), ContextString);
	}

	UE_LOG(Countess_Log, Warning, TEXT("From %s. Previous Health value is %f. Multiplier percent is %f."), TEXT(__FUNCTION__), CurrentHealthValue, Multiplier);
	CurrentHealthValue *= (1 - Multiplier / 100); //Reduce our current health by Health Cost Percentage amount
	UE_LOG(Countess_Log, Warning, TEXT("From %s. Current Health value is %f"), TEXT(__FUNCTION__), CurrentHealthValue);
	if (CurrentHealthValue > 0.f)
	{
		// Set the Source's health attribute after calculation
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Countess_Statics_BloodLust_Cost().HealthProperty, EGameplayModOp::Override, CurrentHealthValue));
	}
}

