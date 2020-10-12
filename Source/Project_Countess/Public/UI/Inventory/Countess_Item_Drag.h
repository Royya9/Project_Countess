// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Countess_Item_Drag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_Item_Drag : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DragDrop")
	UUserWidget* InventoryItemToDrag;
};
