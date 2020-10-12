// Free to play. By SoNa Games.


#include "UI/Inventory/Countess_Inventory_Container.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Inventory/Countess_Widget_Drag.h"
#include "UI/Inventory/Countess_Inventory_ActionMenu.h"
#include "UI/Inventory/Countess_Inventory_Drop.h"

void UCountess_Inventory_Container::GenerateSlots_Implementation(int32 SlotsPerRow, int32 MaxSlots,
    const TArray<FCountess_Item_Slot>& InventorySlots)
{
    InventorySlots_Native = InventorySlots;
    //InventorySlots_Native = &(const_cast<TArray<FCountess_Item_Slot>&>(InventorySlots));
    // Binding when RMB on an inventory slot is clicked.
    int32 Index = 0;
    for(auto& InventoryWidgetSlot : InventorySlot_Widgets_Native)
    {
        InventoryWidgetSlot->ActionMenuShowPressed.Clear();
        InventoryWidgetSlot->ActionMenuShowPressed.AddDynamic(this, &UCountess_Inventory_Container::ShowActionMenu);

        InventoryWidgetSlot->CountessInventorySlotDoubleClick.Clear();
        InventoryWidgetSlot->CountessInventorySlotDoubleClick.AddDynamic(this, &UCountess_Inventory_Container::InventorySlotDoubleClick);

        InventoryWidgetSlot->CountessInventorySlotAddItemFromIndex.Clear();
        InventoryWidgetSlot->CountessInventorySlotAddItemFromIndex.AddDynamic(this, &UCountess_Inventory_Container::InventorySlotAddItemFromIndex);

//        InventoryWidgetSlot->Item_Slot_Native->SlotNumber = Index;
//        FCountess_Item_Slot* Item_Slot_Temp = &(const_cast<FCountess_Item_Slot&>(InventorySlots[Index]));
//        InventorySlots_Native[Index].SlotNumber = Index;
//        Item_Slot_Temp->SlotNumber = Index++;


        UE_LOG(Countess_Log, Warning, TEXT("From %s. ItemSlot class is __ at Index %d with Count %d"), TEXT(__FUNCTION__), InventoryWidgetSlot->Item_Slot_Native->SlotNumber, InventoryWidgetSlot->Item_Slot_Native->Count);
    }
    ActionMenu_Native->ActionMenuDropMenuShowPressed.Clear();
    ActionMenu_Native->ActionMenuDropMenuShowPressed.AddDynamic(this, &UCountess_Inventory_Container::ShowDropMenu);

    ActionMenu_Native->ActionMenuDropPressed.Clear();
    ActionMenu_Native->ActionMenuDropPressed.AddDynamic(this, &UCountess_Inventory_Container::DropMenuConfirmClicked);

    ActionMenu_Native->ActionMenuUsePressed.Clear();
    ActionMenu_Native->ActionMenuUsePressed.AddDynamic(this, &UCountess_Inventory_Container::InventoryActionMenuUseClicked);

    DropMenu_Native->DropMenu_Cancel.Clear();
    DropMenu_Native->DropMenu_Cancel.AddDynamic(this, &UCountess_Inventory_Container::DropMenuCancelClicked);

    DropMenu_Native->DropMenu_Confirm.Clear();
    DropMenu_Native->DropMenu_Confirm.AddDynamic(this, &UCountess_Inventory_Container::DropMenuConfirmClicked);
}

void UCountess_Inventory_Container::UpdateSlotAtIndex_Implementation(int32 AtIndex, const FCountess_Item_Slot& InventorySlot)
{
    //InventorySlots_Native->Insert(InventorySlot, AtIndex);// = const_cast<FCountess_Item_Slot&>(InventorySlot);
    InventorySlots_Native[AtIndex] = InventorySlot;
    InventorySlot_Widgets_Native[AtIndex]->ActionMenuShowPressed.Clear();
    InventorySlot_Widgets_Native[AtIndex]->ActionMenuShowPressed.AddDynamic(this, &UCountess_Inventory_Container::ShowActionMenu);

    ActionMenu_Native->ActionMenuDropMenuShowPressed.Clear();
    ActionMenu_Native->ActionMenuDropMenuShowPressed.AddDynamic(this, &UCountess_Inventory_Container::ShowDropMenu);
}


FReply UCountess_Inventory_Container::NativeOnMouseButtonDown(const FGeometry& InGeometry,
                                                              const FPointerEvent& InMouseEvent)
{
      return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UCountess_Inventory_Container::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
     UDragDropOperation*& OutOperation)
{
     UCountess_Widget_Drag* DragOperation = NewObject<UCountess_Widget_Drag>(this);
     DragOperation->DefaultDragVisual = this;
     DragOperation->Pivot = EDragPivot::MouseDown;
     DragOperation->MouseOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
     DragOperation->WidgetToDrag = this;
     this->RemoveFromParent();
     OutOperation = DragOperation;
}

void UCountess_Inventory_Container::ShowActionMenu(int32 ReceivedIndex)
{
    UE_LOG(Countess_Log, Warning, TEXT("From %s Received Index %d click"), TEXT(__FUNCTION__), ReceivedIndex);
    OnSlotClicked(ReceivedIndex);
}

void UCountess_Inventory_Container::InventorySlotDoubleClick(const int32 Index)
{
    Countess_Inventory_Use.Broadcast(Index);
}

void UCountess_Inventory_Container::InventorySlotAddItemFromIndex(const int32 FromIndex, const int32 ToIndex)
{
    Countess_Inventory_AddItemFromIndex.Broadcast(FromIndex, ToIndex);
}

void UCountess_Inventory_Container::InventoryActionMenuUseClicked_Implementation(const int32 Index)
{
    Countess_Inventory_Use.Broadcast(Index);
}

void UCountess_Inventory_Container::DropMenuConfirmClicked_Implementation(const int32 Index, const int32 CountToDrop)
{
    Countess_Inventory_Drop.Broadcast(Index, CountToDrop);
}

void UCountess_Inventory_Container::ShowDropMenu_Implementation(int32 ReceivedIndex)
{
    DropMenu_Native->UpdateDropMenu(ReceivedIndex, InventorySlots_Native[ReceivedIndex]);
    DropMenu_Native->SetVisibility(ESlateVisibility::Visible);
}
