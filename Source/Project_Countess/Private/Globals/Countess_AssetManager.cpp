// Free to play. By SoNa Games.


#include "Globals/Countess_AssetManager.h"
#include "Items/Countess_Item.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType UCountess_AssetManager::PotionItemType = TEXT("PotionItem");
const FPrimaryAssetType UCountess_AssetManager::AbilityItemType = TEXT("AbilityItem");
const FPrimaryAssetType UCountess_AssetManager::QuestItemType = TEXT("QuestItem");
const FPrimaryAssetType UCountess_AssetManager::WeaponItemType = TEXT("WeaponItem");

void UCountess_AssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();
    UAbilitySystemGlobals::Get().InitGlobalData();
}

UCountess_AssetManager& UCountess_AssetManager::Get()
{
    UCountess_AssetManager* This = Cast<UCountess_AssetManager>(GEngine->AssetManager);
    if(This)
        return *This;
    else
    {
        UE_LOG(Countess_Log, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be Countess_AssetManager!"));
        return *NewObject<UCountess_AssetManager>(); // never calls this.
    }
}

UCountess_Item* UCountess_AssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, const bool bLogWarning)
{
    const FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);
    UCountess_Item* LoadedItem = Cast<UCountess_Item>(ItemPath.TryLoad());

    if(bLogWarning && LoadedItem == nullptr)
    {
        UE_LOG(Countess_Log, Warning, TEXT("Failed to Load Item for Identifier %s"), *PrimaryAssetId.ToString());
    }
    return LoadedItem;
}
