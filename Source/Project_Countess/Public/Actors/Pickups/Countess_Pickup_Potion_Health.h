// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Actors/Pickups/Countess_Pickup_Base.h"
#include "Countess_Pickup_Potion_Health.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_Pickup_Potion_Health : public ACountess_Pickup_Base
{
	GENERATED_BODY()

public:

	ACountess_Pickup_Potion_Health();

	virtual void OnItemPickedUp_Implementation(bool bItemPickedUp, int32& NumberOfItemsLeft) override;
	
};
