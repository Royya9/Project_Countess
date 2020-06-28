// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameplayModMagnitudeCalculation.h"
#include "Countess_CooldownModCalculation.generated.h"

/**
 *  Our Custom CooldownCalculation Class
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_CooldownModCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
