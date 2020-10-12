// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Blueprint/UserWidget.h"
#include "Items/Countess_Item.h"
#include "UI/Inventory/Countess_Inventory_Slot.h"
#include "UI/Inventory/Countess_Inventory_Drop.h"
#include "Countess_Inventory_Container.generated.h"

class UCountess_Inventory_ActionMenu;
class UCountess_Inventory_Drop;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCountess_Inventory_Drop_Final, int32, Index, int32, CountToDrop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountess_Inventory_Use, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCountess_Inventory_AddItemFromIndex, int32, FromIndex, int32, ToIndex);

/**
 *  Container for our InventorySlots
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_Inventory_Container : public UUserWidget
{
	GENERATED_BODY()

public:

	// Generate all slots when inventory is opened/closed. Taxing....
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryContainer")
	void GenerateSlots(int32 SlotsPerRow, int32 MaxSlots, const TArray<FCountess_Item_Slot>& InventorySlots);

	//Just for ref purposes. Refreshing all slots even for one slot change is silly. 
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InventoryContainer")
	void RefreshSlots(const TArray<FCountess_Item_Slot>& InventorySlots);

	// For above reason, we are updating only slots that are changed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryContainer")
	void UpdateSlotAtIndex(int32 AtIndex, const FCountess_Item_Slot& InventorySlot);
	virtual void UpdateSlotAtIndex_Implementation(int32 AtIndex, const FCountess_Item_Slot& InventorySlot);

	// Native override to detect drag
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// Native override to handle inventory widget dragging
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// This is the REAL DEAL. Pointer to actual WBP_ActionMenu created by this inventory. Set from child class WBP_Inventory: Event GenerateSlots
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InventoryContainer")
	UCountess_Inventory_ActionMenu* ActionMenu_Native;

	// Same as above. Pointer to actual WBP_ActionMenu created by this inventory. Set from child class WBP_Inventory: Event GenerateSlots
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InventoryContainer")
	UCountess_Inventory_Drop* DropMenu_Native;

	// Binding function to show Action Menu. Bound from Countess_Inventory_Slot when RMB is clicked
	UFUNCTION()
	void ShowActionMenu(int32 ReceivedIndex);

	// Binding function to show Drop Menu. Bound from Countess_Inventory_ActionMenu when Drop button is clicked
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryContainer")
	void ShowDropMenu(int32 ReceivedIndex);

	// Binding function to re-enable Slot panel of this container. Bound from Countess_Inventory_DropMenu Cancel button is clicked 
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InventoryContainer")
	void DropMenuCancelClicked();
	
	// Event implemented in BP to show where ActionMenu should be displayed
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InventoryContainer")
	void OnSlotClicked(int32 SlotIndex);

	FCountess_Inventory_Drop_Final Countess_Inventory_Drop;

	FCountess_Inventory_Use Countess_Inventory_Use;

	FCountess_Inventory_AddItemFromIndex Countess_Inventory_AddItemFromIndex;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryContainer")
	void DropMenuConfirmClicked(const int32 Index, const int32 CountToDrop);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InventoryContainer")
	void InventoryActionMenuUseClicked(const int32 Index);

	UFUNCTION()
	void InventorySlotDoubleClick(const int32 Index);

	UFUNCTION()
	void InventorySlotAddItemFromIndex(const int32 FromIndex, const int32 ToIndex);

private:

	 UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InventoryContainer", meta = (AllowPrivateAccess = "true"))	
	 TArray<FCountess_Item_Slot> InventorySlots_Native;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InventoryContainer", meta = (AllowPrivateAccess = "true"))
	TArray<UCountess_Inventory_Slot*> InventorySlot_Widgets_Native;

};
