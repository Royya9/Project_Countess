// Free to play. By SoNa Games.


#include "UI/Countess_HUD_Widget.h"
#include "UI/Inventory/Countess_Widget_Drag.h"
#include "UI/Inventory/Countess_Item_Drag.h"
#include "UI/Inventory/Countess_Inventory_Slot.h"

bool UCountess_HUD_Widget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    UCountess_Widget_Drag* WidgetDragOperation = Cast<UCountess_Widget_Drag>(InOperation);
    if(WidgetDragOperation)
    {
        const FVector2D NewPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition()) - WidgetDragOperation->MouseOffset;
        WidgetDragOperation->WidgetToDrag->AddToViewport();
        WidgetDragOperation->WidgetToDrag->SetPositionInViewport(NewPosition, false);
        return true;
    }
    UCountess_Item_Drag* ItemDragOperation = Cast<UCountess_Item_Drag>(InOperation);
    if (ItemDragOperation)
    {
        UCountess_Inventory_Slot* DropWidget = Cast<UCountess_Inventory_Slot>(ItemDragOperation->InventoryItemToDrag);
        if (DropWidget)
        {
            InventoryDropDetected.Broadcast(DropWidget->Item_Slot_Native->SlotNumber);
        }
    }
    return false;
}
