// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Countess_GE_Damage_Execution.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GE_Damage_Execution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UCountess_GE_Damage_Execution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
