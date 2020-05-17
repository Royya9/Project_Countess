// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameplayEffect.h"
#include "Countess_GameplayEffect_Base.generated.h"

class UCurveTable;

/**
 *  Base class that loads our AbilityDetails Curve Table for child classes' use
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GameplayEffect_Base : public UGameplayEffect
{
	GENERATED_BODY()
	
public:

	UCountess_GameplayEffect_Base();

protected:

	UCurveTable* AbilityDetailsTable;

};
