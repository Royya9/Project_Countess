// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "UObject/Interface.h"
#include "Countess_Interface_Inventory.generated.h"

class UCountess_Item;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCountess_Interface_Inventory : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface b/w Pickup Actor and Player Character (which in turn passes this info to Player Character) 
 */
class PROJECT_COUNTESS_API ICountess_Interface_Inventory
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Interface")
	bool GiveItemOnOverlap(TSubclassOf<UCountess_Item>& ItemClassToGive, int32& NumberOfItemsToGive);
};
