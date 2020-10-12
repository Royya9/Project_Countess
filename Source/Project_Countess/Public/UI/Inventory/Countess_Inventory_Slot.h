// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Blueprint/UserWidget.h"
#include "Items/Countess_Item.h"
#include "Countess_Inventory_Slot.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionMenuShowPressed, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessInventorySlotDoubleClick, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCountessInventorySlotAddItemFromIndex, int32, FromIndex, int32, ToIndex);

/**
 *  Base class for Inventory Slot Widget
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_Inventory_Slot : public UUserWidget
{
	GENERATED_BODY()

public:

	// Event called to populate our WBP InventorySlot
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InventorySlot")
	void UpdateSlot(const FCountess_Item_Slot& Item_Slot);

	// Native Listener for RMB click to show Action Menu
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// Native Listener for drag detection to drag items in inventory
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// Native Listener for LMB double click. Fires Use item at Index delegate caught by Inventory container 
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InventorySlot")
	void SetIconBrushOnDrag();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InventorySlot")
	void SetIconBrushOnDragOver(const int32 FromIndex);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InventorySlot")
	void SetIconBrushOnDragLeave();

	// Delegate to notify whenever right click is pressed on our Slot to show Action Menu
	// Carries Index as payload to notify and populate ActionMenu
	FActionMenuShowPressed ActionMenuShowPressed;

	// Delegate to notify LMB double click on our slot to Use Item
	FCountessInventorySlotDoubleClick CountessInventorySlotDoubleClick;

	// Delegate to notify AddItemFromIndexToAnotherIndex
	FCountessInventorySlotAddItemFromIndex CountessInventorySlotAddItemFromIndex;

	// Widget BP class set in BP to get the class of the actual WBP. Used in NativeDragDetected Function to create an identical widget for visual representation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "InventorySlot")
	TSubclassOf<UCountess_Inventory_Slot> Inventory_Slot_Widget_Class;

	// Native variable to hold our Item_Slot details
	FCountess_Item_Slot* Item_Slot_Native;
	
private:

	bool bDraggedOver;
};
