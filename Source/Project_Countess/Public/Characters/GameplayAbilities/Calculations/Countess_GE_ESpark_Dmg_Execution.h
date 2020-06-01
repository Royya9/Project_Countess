// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Countess_GE_ESpark_Dmg_Execution.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GE_ESpark_Dmg_Execution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UCountess_GE_ESpark_Dmg_Execution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
