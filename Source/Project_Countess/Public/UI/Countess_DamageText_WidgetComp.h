// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Components/WidgetComponent.h"
#include "Countess_DamageText_WidgetComp.generated.h"

class UCountess_DamageNumber_Widget;

/**
 *  For floating Damage Numbers when character receives damage
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_DamageText_WidgetComp : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UCountess_DamageText_WidgetComp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetDamageText(float Damage);
	virtual void SetDamageText_Implementation(float Damage);
	
	UPROPERTY()
	UCountess_DamageNumber_Widget* DamageNumberWidget;

	TSubclassOf<UCountess_DamageNumber_Widget> DamageNumberWidgetClass;

	FTimerHandle DamageTextWidgetTimerHandle;
	
	UFUNCTION()
	void DestroyDamageTextWidgetComponent();
};
