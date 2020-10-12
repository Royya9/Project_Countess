// Free to play. By SoNa Games.


#include "UI/Inventory/Countess_Inventory_ActionMenu.h"

void UCountess_Inventory_ActionMenu::UseButtonPressed_Implementation(int32 Index)
{
    UE_LOG(Countess_Log, Warning, TEXT("From %s. Use Button clicked at %d"), TEXT(__FUNCTION__), Index);
    ActionMenuUsePressed.Broadcast(Index);
}

void UCountess_Inventory_ActionMenu::DropButtonPressed_Implementation(int32 Index, int32 Count)
{
    UE_LOG(Countess_Log, Warning, TEXT("From %s. Drop Button clicked at %d"), TEXT(__FUNCTION__), Index);
    if(Count <=1)
        ActionMenuDropPressed.Broadcast(Index, Count);
    else
    {
        ActionMenuDropMenuShowPressed.Broadcast(Index);
        UE_LOG(Countess_Log, Warning, TEXT("From %s. ShowDropMenu Broadcasted %d"), TEXT(__FUNCTION__), Index);
    }
}
