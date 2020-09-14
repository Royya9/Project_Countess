// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/PlayerController.h"
#include "Countess_PlayerController.generated.h"

class ACountess_Character_Base;
class ACountess_Character_Player;
class ACountess_PlayerState;
class ACountess_HUD;
class UCountess_HUD_Widget;
class UCountess_BMagic_Menu_Widget;
class UCountess_WMagic_Menu_Widget;
class UCountess_GameplayAbility_Base;
class USoundBase;
class ICountess_Interface_AbilityDetail;
class UGameplayAbility;

/**
 *  Player Controller Base Class for Project_Countess Player
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ACountess_PlayerController();

	/*Override to setup input bindings*/
	virtual void OnPossess(APawn* aPawn) override;

	/*Bound function to move player right/left */
	void MoveRight(float value);

	/*Bound function to move player right/left */
	void MoveUp(float value);

	void MenuOp();
	
	/*Bound function to make player jump on button press*/
	void Ability_Jump();

	void OpenBMagicMenu();

	void CloseBMagicMenu();

	void OpenWMagicMenu();

	void CloseWMagicMenu();

	void Ability_BackDash();

	void Ability_Primary();

	//Generic function to activate Slotted BMagic Ability. Will replace specific Ability function calls.
	void ActivateBMagicAbility();

	//Generic function to activate Slotted WMagic Ability.
	void ActivateWMagicAbility();

	void StartCooldownTimer(TSubclassOf<UGameplayAbility> Ability, bool bIsAbilityWMagic);

	// Function which registers for CountessTimerComponent and updates CooldownPercentage for this ability
	UFUNCTION()
	void SetWMagicAbilityCooldown(float StartValue, float EndValue, float LerpedValue);

	UFUNCTION()
	void SetBMagicAbilityCooldown(float StartValue, float EndValue, float LerpedValue);

	virtual void BeginPlay() override;
	
	void Interact();

	UFUNCTION(BlueprintCallable)
	void EndInteract();

	bool Handle_Acquire_Ability_OnOverlap(TSubclassOf<UCountess_GameplayAbility_Base>& AbilityToAcquire);

	bool Handle_Acquire_Ability_EndOverlap();

	UFUNCTION()
	void Remove_Notify_Widget_From_Parent();

	void Populate_Skill_Acquired_Widget(TSubclassOf<UCountess_GameplayAbility_Base>& AbilityToAcquire);

	void Populate_BMagicMenu_Widget(UCountess_BMagic_Menu_Widget* BMagic_Menu_Widget);

	void Populate_WMagicMenu_Widget(UCountess_WMagic_Menu_Widget* WMagic_Menu_Widget);

	void ShowDamageNumber(float Damage, ACountess_Character_Base* TargetCharacter);

	void ShowTimerBarWidget(const FText& AbilityText, const float Duration);

	//Timers

	FTimerHandle NotifyWidgetDelayHandle;

	//Corresponding functions where necessary logic takes place
	
	UFUNCTION()
	void OnHealthChanged(float NewHealthValue);

	UFUNCTION()
	void OnStaminaChanged(float NewStaminaValue);

	UFUNCTION()
	void OnManaChanged(float NewManaValue);

	UFUNCTION()
	void OnExpChanged(float NewExpValue);

	UFUNCTION()
	void OnMaxHealthChanged(float NewMaxHealthValue);

	UFUNCTION()
	void OnMaxStaminaChanged(float NewMaxStaminaValue);

	UFUNCTION()
	void OnMaxManaChanged(float NewMaxManaValue);

	UFUNCTION()
	void OnMaxExpChanged(float NewMaxExpValue);

	UFUNCTION()
	void OnHealthRegenRateChanged(float NewHealthRegenValue);

	UFUNCTION()
	void OnManaRegenRateChanged(float NewManaRegenValue);

	UFUNCTION()
	void OnStaminaRegenRateChanged(float NewStaminaRegenValue);

	UFUNCTION()
	void OnArmorChanged(float NewArmorValue);

	UFUNCTION()
	void OnAbilityAcquired(TSubclassOf<UCountess_GameplayAbility_Base> AbilityAcquiredClass, FSlateBrush AbilityIcon, float Cooldown);

	UFUNCTION()
	void OnPlayerLevelIncreased(int32 NewPlayerLevel);

// Inputs
public:

	virtual void SetupInputComponent() override;

protected:
	UPROPERTY()
	ACountess_Character_Player* PlayerCharacter = nullptr;
	
	ICountess_Interface_AbilityDetail* PlayerStateInterface;
	/*UPROPERTY()
	TScriptInterface<ICountess_Interface_AbilityDetail> PlayerStateInterface;*/

private:
	UPROPERTY()
	ACountess_HUD* Countess_HUD;
	UPROPERTY()
	UCountess_HUD_Widget* Countess_HUD_Widget;

	TSubclassOf<UGameplayAbility> JumpAbility;
	TSubclassOf<UGameplayAbility> BackDashAbility;
	TSubclassOf<UCountess_GameplayAbility_Base> m_AbilityToAcquire;
	TSubclassOf<UGameplayAbility> BlackMagicAbility;
	TSubclassOf<UGameplayAbility> WhiteMagicAbility;
	TSubclassOf<UGameplayAbility> PrimaryAbility;
	TSubclassOf<UGameplayAbility> SkillAbility;

	bool bHandlingAbilityAcquire;
	bool bAbilityAcquired;

	UPROPERTY()
	USoundBase* SkillAcquiredSound;
	UPROPERTY()
	USoundBase* NotifyWidgetOpenSound;
	UPROPERTY()
	USoundBase* NotifyWidgetCloseSound;
	UPROPERTY()
	USoundBase* BMagicMenuOpenSound;
	UPROPERTY()
	USoundBase* BMagicMenuCloseSound;
	/*Variables that hold which magic skills are slotted currently*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	E_BMagic BMagicSlotted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	E_WMagic WMagicSlotted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	E_Skill SkillAcquired;

	bool bBlackMagicMenuOpened;

	bool bWhiteMagicMenuOpened;
};
