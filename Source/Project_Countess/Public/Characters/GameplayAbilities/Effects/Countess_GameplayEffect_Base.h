// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameplayEffect.h"
#include "Countess_GameplayEffect_Base.generated.h"

class UCurveTable;
class UCountess_AttributeSet_Base;

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

	//CurveTable that contains all Player Ability/Effect Details. 
	UCurveTable* AbilityDetailsTable;

	UCurveTable* MinionDetailsTable;

	//AttributeSet needed by subclasses to find the attribute the corresponding Effect to be applied to.
	UCountess_AttributeSet_Base* Countess_AttributeSet;
};
