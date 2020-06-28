// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Calculations/Countess_GE_CooldownExecution.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/CurveTable.h"
#include "UObject/ConstructorHelpers.h"

UCountess_GE_CooldownExecution::UCountess_GE_CooldownExecution()
{
	//static ConstructorHelpers::FObjectFinder AbilityDetailsTableHelper(TEXT("CurveTable'/Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails.AbilityDetails'"));
	//if (AbilityDetailsTableHelper.Succeeded())
	//	AbilityDetailsTable = AbilityDetailsTableHelper.Object;
}

void UCountess_GE_CooldownExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
/*
	if (AbilityDetailsTable)
	{
		FCurveTableRowHandle AbilityDetailsRowHandle;
		AbilityDetailsRowHandle.CurveTable = AbilityDetailsTable;
		AbilityDetailsRowHandle.RowName = FName("PrimaryAbilityCooldown");
		const FString ContextString;
		float CooldownMagnitude = AbilityDetailsRowHandle.Eval(1.f, ContextString);
	}*/
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

/*
	for (auto& SourceTag : SourceTags)
	{
		UE_LOG(Countess_Log, Warning, TEXT("From %s. Tag is %s"), TEXT(__FUNCTION__), *SourceTag.ToString());
	}
*/

	UE_LOG(Countess_Log, Warning, TEXT("From %s. Actual Cooldown Duration is %f"), TEXT(__FUNCTION__), Spec.Duration);
	
	//Spec.SetSetByCallerMagnitude(CountessTags::FCooldownTags::PrimaryAbilityCooldownTag, Spec.Duration);

	//OutExecutionOutput.AddOutputModifier()
	//AssignSetByCallerMagnitude
	
}
