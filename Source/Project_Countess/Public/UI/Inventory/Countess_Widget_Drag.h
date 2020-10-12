// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Countess_Widget_Drag.generated.h"

class UUserWidget;

/**
 *  Drag and Drop Operation for Widgets (like Inventory)
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_Widget_Drag : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DragDrop")
	UUserWidget* WidgetToDrag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DragDrop")
	FVector2D MouseOffset;
	
};
