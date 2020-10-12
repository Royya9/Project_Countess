// Free to play. By SoNa Games.


#include "UI/Inventory/Countess_Inventory_Slot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Inventory/Countess_Item_Drag.h"

void UCountess_Inventory_Slot::UpdateSlot_Implementation(const FCountess_Item_Slot& Item_Slot)
{
    // This function gets called from WBP_Inventory_Slot: UpdateSlot event -> call parent event
    // and our Native variable is set from there
    Item_Slot_Native = &(const_cast<FCountess_Item_Slot&>(Item_Slot));


    
}

FReply UCountess_Inventory_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if(Item_Slot_Native && Item_Slot_Native->IsValid())
    {
        if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
        {
            // Broadcast to announce that RMB is clicked. This event is caught by container which then shows ActionMenu
            ActionMenuShowPressed.Broadcast(Item_Slot_Native->SlotNumber);
        }
        else
        {
            return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
        }
    }
    return OnMouseButtonDown( InGeometry, InMouseEvent ).NativeReply;
}

void UCountess_Inventory_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
    UDragDropOperation*& OutOperation)
{
   if(Item_Slot_Native && Item_Slot_Native->IsValid() && Inventory_Slot_Widget_Class)
   {
       UCountess_Item_Drag* DragOperation = NewObject<UCountess_Item_Drag>(this);
       UCountess_Inventory_Slot* DragVisualItem = Cast<UCountess_Inventory_Slot>(CreateWidget(this, Inventory_Slot_Widget_Class, FName("DragVisualItem")));
       FCountess_Item_Slot Item_Slot = *this->Item_Slot_Native;
       DragVisualItem->UpdateSlot(Item_Slot);
       DragOperation->DefaultDragVisual = DragVisualItem;
       DragOperation->Pivot = EDragPivot::MouseDown;
       DragOperation->InventoryItemToDrag = this;
       this->SetIconBrushOnDrag();
       OutOperation = DragOperation;
       Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
   }
}

FReply UCountess_Inventory_Slot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
                                                                const FPointerEvent& InMouseEvent)
{
    ensure(Item_Slot_Native);
    ensure(Item_Slot_Native->SlotItem.GetDefaultObject());
    if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && Item_Slot_Native && Item_Slot_Native->IsValid() && Item_Slot_Native->SlotItem.GetDefaultObject()->IsConsumable())
    {
        UE_LOG(Countess_Log, Warning, TEXT("From %s. Double click on Item %s at Index %d"), TEXT(__FUNCTION__), *Item_Slot_Native->SlotItem.GetDefaultObject()->ItemName.ToString(), Item_Slot_Native->SlotNumber);
        CountessInventorySlotDoubleClick.Broadcast(Item_Slot_Native->SlotNumber);
    }

    return OnMouseButtonDoubleClick(InGeometry, InMouseEvent).NativeReply;
}

/*
bool UCountess_Inventory_Slot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    if(bDraggedOver)
        return true;

    UCountess_Item_Drag* Item_Drag = Cast<UCountess_Item_Drag>(InOperation);
    if(Item_Drag)
    {
        bDraggedOver = true;
        UCountess_Inventory_Slot* Item_Slot = Cast<UCountess_Inventory_Slot>(Item_Drag->InventoryItemToDrag);
        if(Item_Slot)
        {
            SetIconBrushOnDragOver(Item_Slot->Item_Slot_Native->SlotNumber);
            //CountessInventorySlotAddItemFromIndex.Broadcast(Item_Slot->Item_Slot_Native->SlotNumber, this->Item_Slot_Native->SlotNumber);
        }
    }
    return true;
}*/

bool UCountess_Inventory_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    UCountess_Item_Drag* Item_Drag = Cast<UCountess_Item_Drag>(InOperation);
    if(Item_Drag)
    {
        UCountess_Inventory_Slot* DraggedInventory_Slot = Cast<UCountess_Inventory_Slot>(Item_Drag->InventoryItemToDrag);
        if(this->Item_Slot_Native->SlotNumber != DraggedInventory_Slot->Item_Slot_Native->SlotNumber)
        {
            bDraggedOver = false;
            CountessInventorySlotAddItemFromIndex.Broadcast(DraggedInventory_Slot->Item_Slot_Native->SlotNumber, this->Item_Slot_Native->SlotNumber);
        }
        else
        {
            bDraggedOver = false;
            UpdateSlot(*this->Item_Slot_Native);
        }
        return true;
    }
    return false;
}

void UCountess_Inventory_Slot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if(bDraggedOver)
    {
        UCountess_Item_Drag* Item_Drag = Cast<UCountess_Item_Drag>(InOperation);
        if(Item_Drag)
        {
            bDraggedOver = false;
            SetIconBrushOnDragLeave();
        }
    }
}

