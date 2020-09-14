// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameplayModMagnitudeCalculation.h"
#include "Countess_GE_BloodLust_HealthPen.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GE_BloodLust_HealthPen : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	UCountess_GE_BloodLust_HealthPen();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	//CurveTable that contains all Player Ability/Effect Details. 
	
	UPROPERTY()
	UCurveTable* AbilityDetailsTable;
};
