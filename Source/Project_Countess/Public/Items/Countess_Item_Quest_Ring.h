// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Items/Countess_Item.h"
#include "Globals/Countess_AssetManager.h"
#include "Countess_Item_Quest_Ring.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_Item_Quest_Ring : public UCountess_Item
{
	GENERATED_BODY()

public:
	
	UCountess_Item_Quest_Ring()
	{
		ItemType = UCountess_AssetManager::QuestItemType;
		ItemName = FName("Deadly Ring");
		ItemDescription = FName("Wear it and become Awesome!");

		static ConstructorHelpers::FObjectFinder<UTexture2D> IconFinder(TEXT("Texture2D'/Game/MyProjectMain/Textures/T_Icon_Ring.T_Icon_Ring'"));
		if(IconFinder.Succeeded())
		{
			ItemIcon.SetResourceObject(static_cast<UObject*>(IconFinder.Object));
			ItemIcon.SetImageSize(FVector2D(128.f, 128.f));
		}
		else
			UE_LOG(Countess_Log, Warning, TEXT("Cannot find Texture in %s."), TEXT(__FUNCTION__));
        
		Price = 500;
		bCanBeStacked = false;
		MaxCount = 1;
		MaxLevel = 1;

	}
};
