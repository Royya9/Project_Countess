// Free to play. By SoNa Games.


#include "UI/INventory/Countess_Inventory_Drop.h"

void UCountess_Inventory_Drop::DropMenuConfirmClicked_Implementation(const int32 Index, const int32 CountToDrop)
{
    DropMenu_Confirm.Broadcast(Index, CountToDrop);
    //UE_LOG(Countess_Log, Warning, TEXT("DropMenu Confirm Button clicked"));
    UE_LOG(Countess_Log, Warning, TEXT("From %s. Confirm to drop %d from Slot with index %d"), TEXT(__FUNCTION__), CountToDrop, Index);
}

void UCountess_Inventory_Drop::DropMenuCancelClicked_Implementation()
{
    DropMenu_Cancel.Broadcast();
}
