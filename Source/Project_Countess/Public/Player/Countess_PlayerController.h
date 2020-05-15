// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/PlayerController.h"
//#include "GameplayEffectTypes.h"
#include "Countess_PlayerController.generated.h"

class AProject_CountessCharacter;
class ACountess_PlayerState;
class ACountess_HUD;
class UCountess_HUD_Widget;

/**
 *  Player Controller Base Class for Project_Countess Player
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/*Override to setup input bindings*/
	virtual void OnPossess(APawn* aPawn) override;

	/*Bound function to move player right/left */
	void MoveRight(float value);

	/*Bound function to make player jump on button press*/
	void Ability_Jump();

	/*Bound function to make player stop jumping on button release*/
	void Ability_StopJumping();

	virtual void BeginPlay() override;

	//Delegates to handle attribute changes

	FDelegateHandle OnHealthChangedDelegateHandle;

	//Corresponding functions where necessary logic takes place
	
	UFUNCTION()
	void OnHealthChanged(float NewHealthValue);

	UFUNCTION()
	void OnAbilityAcquired(FSlateBrush AbilityIcon);

// Inputs
public:

	virtual void SetupInputComponent() override;

protected:

	AProject_CountessCharacter* PlayerCharacter = nullptr;

private:

	ACountess_HUD* Countess_HUD;
	UCountess_HUD_Widget* Countess_HUD_Widget;
	ACountess_PlayerState* PlayerState;

	/*Variables that hold which magic skills are slotted currently*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	E_BMagic BMagicSlotted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	E_WMagic WMagicSlotted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	E_Skills SkillAcquired;
};
