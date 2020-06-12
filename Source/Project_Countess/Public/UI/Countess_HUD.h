// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/SlateWrapperTypes.h"
#include "Countess_HUD.generated.h"

class APlayerController;
class UCountess_HUD_Widget;
class UCountess_Notify_Widget;
class UCountess_SkillAcquired_Widget;
class UCountess_BMagic_Menu_Widget;

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

	FORCEINLINE UCountess_HUD_Widget* Get_Countess_HUDWidget() { return HUD_Widget; }

	FORCEINLINE UCountess_Notify_Widget* Get_Countess_Notify_Widget() { return Notify_Widget; }

	FORCEINLINE UCountess_SkillAcquired_Widget* Get_Countess_Skill_Acquired_Widget() { return Skill_Acquired_Widget; }

	FORCEINLINE UCountess_BMagic_Menu_Widget* Get_Countess_BMagic_Menu_Widget() { return BMagic_Menu_Widget; }

	/*This creates all necessary widgets. However this function is called from Player Controller which is the master of HUD*/
	void CreateWidgets(APlayerController* PlayerController, ESlateVisibility Visibility = ESlateVisibility::Visible);

	bool CreateNotifyWidget(APlayerController* PlayerController, ESlateVisibility Visibility = ESlateVisibility::Visible);

	bool CreateSkillAcquiredWidget(APlayerController* PlayerController, ESlateVisibility Visibility = ESlateVisibility::Visible);

	bool CreateBMagicMenuWidget(APlayerController* PlayerController, ESlateVisibility Visibility = ESlateVisibility::Visible);

private:
	UPROPERTY()
	UCountess_HUD_Widget* HUD_Widget;

	UPROPERTY()
	UCountess_Notify_Widget* Notify_Widget;

	UPROPERTY()
	UCountess_SkillAcquired_Widget* Skill_Acquired_Widget;

	UPROPERTY()
	UCountess_BMagic_Menu_Widget* BMagic_Menu_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCountess_HUD_Widget> Countess_HUD_Widget_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCountess_Notify_Widget> Countess_Notify_Widget_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCountess_SkillAcquired_Widget> Countess_SkillAcquired_Widget_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCountess_BMagic_Menu_Widget> Countess_BMagic_Menu_Widget_Class;
	
};
