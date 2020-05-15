// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Countess_HUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_HUD : public AHUD
{
	GENERATED_BODY()


public:

	ACountess_HUD();

	virtual void BeginPlay() override;

	FORCEINLINE class UCountess_HUD_Widget* Get_Countess_HUDWidget() {return HUD_Widget;}

	/*This creates all necessary widgets. However this function is called from Player Controller which is the master of HUD*/
	void CreateWidgets(class APlayerController* PlayerController);

private:

	class UCountess_HUD_Widget* HUD_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCountess_HUD_Widget> Countess_HUD_Widget_Class;
	
};
