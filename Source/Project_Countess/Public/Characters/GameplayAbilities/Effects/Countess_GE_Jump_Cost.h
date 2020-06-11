// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Characters/GameplayAbilities/Effects/Countess_GameplayEffect_Base.h"
#include "Countess_GE_Jump_Cost.generated.h"

/**
 *  Gameplay Effect that handles cost for Jump Ability
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_GE_Jump_Cost : public UCountess_GameplayEffect_Base
{
	GENERATED_BODY()
	
public:

	UCountess_GE_Jump_Cost();

};
