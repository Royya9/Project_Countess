// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Blueprint/UserWidget.h"
#include "Items/Countess_Item.h"
#include "Countess_Inventory_ActionMenu.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessActionMenuUsePressed, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCountessActionMenuThrowPressed, int32, Index, int32, Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessActionMenuDropMenuShowPressed, int32, Index);

/**
 *  Action Menu that appears when Mouse RightClick on Inventory Slot.
 *  Corresponding Delegates to handle button clicks are declared in Global Header and implemented in PlayerController
 */ 
UCLASS()
class PROJECT_COUNTESS_API UCountess_Inventory_ActionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
    
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ActionMenu")
    void UpdateActionMenu(int32 Index, const FCountess_Item_Slot& Item_Slot);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionMenu")
    FCountessActionMenuUsePressed ActionMenuUsePressed;

    UPROPERTY(BlueprintAssignable, Category = "ActionMenu")
    FCountessActionMenuThrowPressed ActionMenuDropPressed;

    UPROPERTY(BlueprintAssignable, Category = "ActionMenu")
    FCountessActionMenuDropMenuShowPressed ActionMenuDropMenuShowPressed;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionMenu")
    void UseButtonPressed(int32 Index);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionMenu")
    void DropButtonPressed(int32 Index, int32 Count);

};
