// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Countess_GE_ArcBlast_Dmg_Exec.generated.h"

/**
 *  ArcticBlast Damage Execution
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GE_ArcBlast_Dmg_Exec : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UCountess_GE_ArcBlast_Dmg_Exec();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
