// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Countess_GE_CooldownExecution.generated.h"

class UCurveTable;

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GE_CooldownExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	
	UCountess_GE_CooldownExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:

	UCurveTable* AbilityDetailsTable;

};
