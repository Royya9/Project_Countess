// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Countess_AI_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_AI_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float Percentage);

};
