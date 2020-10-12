// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Engine/AssetManager.h"
#include "Countess_AssetManager.generated.h"

class UCountess_Item;

/**
 *  Our own version of AssetManager.
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

    UCountess_AssetManager() {}
    virtual void StartInitialLoading() override;

    /* Static types for Items*/
    static const FPrimaryAssetType PotionItemType;
    static const FPrimaryAssetType QuestItemType;
    static const FPrimaryAssetType AbilityItemType;
    static const FPrimaryAssetType WeaponItemType;

    /** Returns the current AssetManager object */
    static UCountess_AssetManager& Get();

    /**
    * Synchronously loads an RPGItem subclass, this can hitch but is useful when you cannot wait for an async load
    * This does not maintain a reference to the item so it will garbage collect if not loaded some other way
    *
    * @param PrimaryAssetId The asset identifier to load
    * @param bLogWarning If true, this will log a warning if the item failed to load
    */
    UCountess_Item* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, const bool bLogWarning = true);
};
