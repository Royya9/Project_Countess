// Free to play. By SoNa Games.


#include "Items/Countess_Item.h"

bool UCountess_Item::IsConsumable() const
{
    if(MaxCount <= 0)
        return true;
    return false;
}

FString UCountess_Item::GetIdentifierString() const
{
    return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UCountess_Item::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(ItemType, GetFName());
}

void UCountess_Item::OnItemUse_Implementation()
{
    UE_LOG(Countess_Log, Warning, TEXT("From %s. You used %s"), TEXT(__FUNCTION__), *ItemName.ToString());
    //OnItemUse();
}
