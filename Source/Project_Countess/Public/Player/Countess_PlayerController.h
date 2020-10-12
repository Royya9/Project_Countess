// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/PlayerController.h"
#include "Items/Countess_Item.h"
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
class UAbilityData;
class UCountess_Timer_Component;

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
	void SetWMagicAbilityCooldown(float LerpedValue, E_WMagic WMagic);

	UFUNCTION()
	void SetBMagicAbilityCooldown(float LerpedValue, E_BMagic BMagic);

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

	void ShowTimerBarWidget(const FText& AbilityText, const float Duration, bool bIsAbilityTimeSlow = false, UCountess_Timer_Component* Timer_Component = nullptr);

	//Timers

	FTimerHandle NotifyWidgetDelayHandle;

	// Container for TimerBarWidgetComponents
	TArray<class UCountess_TimerBar_WidgetComp*> TimerBarWidgetComponents;

	// Function to reposition active TimerBars if any one of them gets destroyed after their duration is over
	UFUNCTION()
	void ClearTimerBarId(const int32 Id);
	
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

	UPROPERTY()
	TWeakObjectPtr<UAbilityData> SlottedWMagicAbilityData_WeakPtr;
	UPROPERTY()
	TWeakObjectPtr<UAbilityData> SlottedBMagicAbilityData_WeakPtr;

	bool bBlackMagicMenuOpened;

	bool bWhiteMagicMenuOpened;

	/**********************************************************************/
	/* All Symbols related to Inventory*/

public:

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	//TMap<FCountess_Item_Slot, UCountess_Item*> SlottedItems;

	void ToggleInventory();

	/* Number Of Slots our Inventory should have*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 NumberOfSlots;

	/* Max number of items a slot should have*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxStackSize;

	/* Our Inventory Slots*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FCountess_Item_Slot> InventorySlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 SlotsPerRow;
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Inventory")
	bool IsSlotEmpty(int32 AtIndex) const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Inventory")
	UCountess_Item* GetItemAtIndex(int32 AtIndex, int32& OutCount) const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Inventory")
	bool GetEmptySlot(int32& OutEmptySlotIndex) const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Inventory")
	bool GetFreeStackSlotForClass(TSubclassOf<UCountess_Item> ItemClass, int32& OutStackSlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemToInventory(TSubclassOf<UCountess_Item>& ItemClassToAdd, int32& CountToAdd);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemAtIndex(int32 Index, int32 NumberOfItemsToRemove);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SwapItemsAtIndices(int32 IndexA, int32 IndexB);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItemAtIndex(int32 Index);

	UFUNCTION()
	void InventoryActionMenuUsePressed(int32 Index);

	UFUNCTION()
	void InventoryActionMenuDropPressed(int32 Index, int32 Count);

	UFUNCTION()
	void AddItemFromIndex(const int32 FromIndex, const int32 ToIndex);
	
private:

	void SetItemAtInventorySlot(const TSubclassOf<UCountess_Item>& ItemClassToSet, const int32& CountToSet, const int32& IndexToSet);
};
