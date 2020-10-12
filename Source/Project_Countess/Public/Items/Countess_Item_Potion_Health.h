// Free to play. By SoNa Games.

#pragma once

#include "Globals/Countess_AssetManager.h"
#include "Items/Countess_Item.h"
#include "Countess_Item_Potion_Health.generated.h"

/**
 *  Our Health Potion Item
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_Item_Potion_Health : public UCountess_Item
{
	GENERATED_BODY()

public:

    UCountess_Item_Potion_Health()
    {
        ItemType = UCountess_AssetManager::PotionItemType;
        ItemName = FName("Health Potion");
        ItemDescription = FName("Grants 20 health points increase in 2 seconds");

        static ConstructorHelpers::FObjectFinder<UTexture2D> IconFinder(TEXT("Texture2D'/Game/MyProjectMain/Textures/T_Icon_Health_Potion.T_Icon_Health_Potion'"));
        if(IconFinder.Succeeded())
        {
            ItemIcon.SetResourceObject(static_cast<UObject*>(IconFinder.Object));
            ItemIcon.SetImageSize(FVector2D(128.f, 128.f));
        }
        else
            UE_LOG(Countess_Log, Warning, TEXT("Cannot find Texture in %s."), TEXT(__FUNCTION__));
        
        Price = 100;
        bCanBeStacked = true;
        MaxCount = 0;
        MaxLevel = 1;

    }
};
