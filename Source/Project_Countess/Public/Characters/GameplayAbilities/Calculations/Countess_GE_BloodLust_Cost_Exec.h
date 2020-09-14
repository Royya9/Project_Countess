// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Countess_GE_BloodLust_Cost_Exec.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GE_BloodLust_Cost_Exec : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UCountess_GE_BloodLust_Cost_Exec();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:

	FCurveTableRowHandle BloodLustCostRowHandle;
};
