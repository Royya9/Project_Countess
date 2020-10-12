// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "Countess_Item.generated.h"

class UCountess_GameplayAbility_Base;
class UCountess_GameplayEffect_Base;

/**
 *  Base class for all Items. Not intended to Blueprint directly
 */
UCLASS(Abstract, BlueprintType)
class PROJECT_COUNTESS_API UCountess_Item : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

    UCountess_Item()
        : Price(0)
        , bCanBeStacked(false)
        , MaxCount(1)
        , MaxLevel(1)
        , AbilityLevel(1)
        , EffectLevel(1)
    {}
    
    /** Type of this item, set in native parent class */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    FPrimaryAssetType ItemType;

    /* Name visible to the Player */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FName ItemName;

    /* Description of the item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FName ItemDescription;

    /* Icon shown in the UI for the item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FSlateBrush ItemIcon;

    /* Price of the item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 Price;

    /* Whether Items of the same type can be stacked in to the same Item Slot or Not */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    bool bCanBeStacked;
    
    /* Max number of items that can be present in the inventory at once <=0 means Infinite*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
    int32 MaxCount;

    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Max")
    bool IsConsumable() const;

    /* Max Level that this Item can be*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
    int32 MaxLevel;

    /* Ability Granted by the item if equipped */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
    TSubclassOf<UCountess_GameplayAbility_Base> GrantedAbility;

    /* Ability Level this item grants. <=0 means Player level */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
    int32 AbilityLevel;

    /* Passive Effect this item grants if equipped */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    TSubclassOf<UCountess_GameplayEffect_Base> GrantedEffect;

    /* Effect Level this item grants. <=0 means Player level */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    int32 EffectLevel;

    UFUNCTION(BlueprintCallable, Category = "Item")
    FString GetIdentifierString() const;

    /* Overridden to use saved type */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
    void OnItemUse();
};

/* Struct representing our Item Slot*/
USTRUCT(BlueprintType, Blueprintable)
struct PROJECT_COUNTESS_API FCountess_Item_Slot
{
    GENERATED_BODY()

    FCountess_Item_Slot()
        : SlotItem(nullptr), SlotNumber(-1), Count(0)
    {}

    FCountess_Item_Slot(TSubclassOf<UCountess_Item> InSlotItem , int32 InSlotNumber, int32 InCount)
        : SlotItem(InSlotItem), SlotNumber(InSlotNumber), Count(InCount)
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSubclassOf<UCountess_Item> SlotItem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 SlotNumber;

    // Number of Items of the type an inventory slot currently has
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 Count;

    bool operator==(const FCountess_Item_Slot& Other) const
    {
        return SlotItem.GetDefaultObject()->GetFName() == Other.SlotItem.GetDefaultObject()->GetFName() && SlotNumber == Other.SlotNumber;
    }

    bool operator!=(const FCountess_Item_Slot& Other) const
    {
        return !(*this == Other);
    }

    bool IsValid() const
    {
        return (SlotItem? true : false) && SlotNumber >=0 && Count >=1;
    }
};