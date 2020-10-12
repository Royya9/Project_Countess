// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Blueprint/UserWidget.h"
#include "Items/Countess_Item.h"

#include "Countess_Inventory_Drop.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCountess_Inventory_DropMenu_Confirm, int32, Index, int32, Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCountess_Inventory_DropMenu_Cancel);

/**
 *  Base class for widget that shows on Clicking Drop Button in Action Menu
 *  Registers how many items to drop at once
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_Inventory_Drop : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InventoryDropMenu")
	void UpdateDropMenu(int32 Index, const FCountess_Item_Slot& Item_Slot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryDropMenu")
	void DropMenuConfirmClicked(const int32 Index, const int32 CountToDrop);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryDropMenu")
	void DropMenuCancelClicked();
	
	FCountess_Inventory_DropMenu_Confirm DropMenu_Confirm;

	FCountess_Inventory_DropMenu_Cancel DropMenu_Cancel;
};
