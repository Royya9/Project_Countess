// Free to play. By SoNa Games.


#include "Player/Countess_PlayerController.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "UI/Countess_HUD.h"
#include "UI/Countess_HUD_Widget.h"
#include "UI/Countess_Notify_Widget.h"
#include "UI/Countess_SkillAcquired_Widget.h"
#include "UI/Countess_DamageText_WidgetComp.h"
#include "UI/Countess_BMagic_Menu_Widget.h"
#include "UI/Countess_WMagic_Menu_Widget.h"
#include "UI/Countess_TimerBar_WidgetComp.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Interfaces/Countess_Interface_AbilityDetail.h"
#include "Camera/Countess_CameraManager.h"
#include "Components/Countess_Timer_Component.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Items/Countess_Item.h"
#include "UI/Inventory/Countess_Inventory_Container.h"


ACountess_PlayerController::ACountess_PlayerController()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> SkillAcquiredSoundObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/SFX_SkillAcquired.SFX_SkillAcquired'"));
	if (SkillAcquiredSoundObject.Succeeded())
	{
		SkillAcquiredSound = SkillAcquiredSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> NotifyWidgetOpenSoundObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/Floating_UI_Open-002.Floating_UI_Open-002'"));
	if (NotifyWidgetOpenSoundObject.Succeeded())
	{
		NotifyWidgetOpenSound = NotifyWidgetOpenSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> NotifyWidgetCloseSoundObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/Floating_UI_Close.Floating_UI_Close'"));
	if (NotifyWidgetCloseSoundObject.Succeeded())
	{
		NotifyWidgetCloseSound = NotifyWidgetCloseSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> BMagicOpenSoundObject(TEXT("SoundCue'/Game/MyProjectMain/Audio/SFX_BMagicMenuOpen_Cue.SFX_BMagicMenuOpen_Cue'"));
	if(BMagicOpenSoundObject.Succeeded())
	{
		BMagicMenuOpenSound = BMagicOpenSoundObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> BMagicCloseSoundObject(TEXT("SoundCue'/Game/MyProjectMain/Audio/SFX_BMagicMenuClose_Cue.SFX_BMagicMenuClose_Cue'"));
	if(BMagicCloseSoundObject.Succeeded())
	{
		BMagicMenuCloseSound = BMagicCloseSoundObject.Object;
	}

	this->PlayerCameraManagerClass = ACountess_CameraManager::StaticClass();
	
	/* Inventory Related */
	NumberOfSlots = 20;
	MaxStackSize = 99;
	InventorySlots.SetNum(NumberOfSlots);
	SlotsPerRow = 4;
}

void ACountess_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	if (aPawn)
	{
		PlayerCharacter = Cast<ACountess_Character_Player>(aPawn);
		if (PlayerCharacter)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s Possessed %s"), *this->GetName(), *PlayerCharacter->GetName());
			SetViewTarget(PlayerCharacter);

			InputComponent->BindAction("Jump", IE_Pressed, this, &ACountess_PlayerController::Ability_Jump);
			//InputComponent->BindAction("Jump", IE_Released, this, &ACountess_PlayerController::Ability_StopJumping);
			InputComponent->BindAxis("MoveRight", this, &ACountess_PlayerController::MoveRight);
			InputComponent->BindAxis("MoveUp", this, &ACountess_PlayerController::MoveUp);
			InputComponent->BindAction("MenuOps", IE_Pressed, this, &ACountess_PlayerController::MenuOp).bExecuteWhenPaused = true;
			InputComponent->BindAction("Interact", IE_Pressed, this, &ACountess_PlayerController::Interact);
			InputComponent->BindAction("Inventory", IE_Pressed, this, &ACountess_PlayerController::ToggleInventory).bExecuteWhenPaused = true;
			/*InputComponent->BindAction("EndInteract", IE_Pressed, this, &ACountess_PlayerController::EndInteract).bExecuteWhenPaused = true;*/
			FInputActionBinding& EndInteractBinding = InputComponent->BindAction("EndInteract", IE_Pressed, this, &ACountess_PlayerController::EndInteract);
			EndInteractBinding.bConsumeInput = false;
			EndInteractBinding.bExecuteWhenPaused = true;
			InputComponent->BindAction("BackDash", IE_Pressed, this, &ACountess_PlayerController::Ability_BackDash);
			InputComponent->BindAction("ActivateBlackMagicAbility", IE_Pressed, this, &ACountess_PlayerController::ActivateBMagicAbility);
			InputComponent->BindAction("ActivateWhiteMagicAbility", IE_Pressed, this, &ACountess_PlayerController::ActivateWMagicAbility);
			InputComponent->BindAction("Primary", IE_Pressed, this, &ACountess_PlayerController::Ability_Primary);
			FInputActionBinding& OpenBMagicMenuBinding = InputComponent->BindAction("BMagicMenu", IE_Pressed, this, &ACountess_PlayerController::OpenBMagicMenu);
			OpenBMagicMenuBinding.bExecuteWhenPaused = true;
			FInputActionBinding& CloseBMagicMenuBinding = InputComponent->BindAction("BMagicMenu", IE_Released, this, &ACountess_PlayerController::CloseBMagicMenu);
			CloseBMagicMenuBinding.bExecuteWhenPaused = true;

			FInputActionBinding& OpenWMagicMenuBinding = InputComponent->BindAction("WMagicMenu", IE_Pressed, this, &ACountess_PlayerController::OpenWMagicMenu);
			OpenWMagicMenuBinding.bExecuteWhenPaused = true;
			FInputActionBinding& CloseWMagicMenuBinding = InputComponent->BindAction("WMagicMenu", IE_Released, this, &ACountess_PlayerController::CloseWMagicMenu);
			CloseWMagicMenuBinding.bExecuteWhenPaused = true;

			// Set Our CameraManager's view target to our player. This is done so that our cameramanager can use player's camera for camera zoom, shake, postprocess etc., effects
			this->PlayerCameraManager->SetViewTarget(PlayerCharacter);			
		}
	}
}


void ACountess_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ACountess_PlayerController::MoveRight(float value)
{
	if (PlayerCharacter)
		PlayerCharacter->AddMovementInput(FVector(1.f, 0.f, 0.f), value);
}


void ACountess_PlayerController::MoveUp(float value)
{
	if (PlayerCharacter)
		PlayerCharacter->AddMovementInput(FVector(0.f, 0.f, 1.f), value);
}

void ACountess_PlayerController::Ability_Jump()
{
	/*Check with PlayerState whether we have the ability to Jump*/
	if (PlayerStateInterface->CanJump(JumpAbility)) /* Ref sent to canJump. This is OUT Param */
	{
		//PlayerState->Countess_TryActivateAbilityByClass(JumpAbility);
		PlayerStateInterface->Execute_Countess_Interface_TryActivateAbilityByClass(GetPlayerState<APlayerState>(), JumpAbility);
	}
}

void ACountess_PlayerController::Ability_BackDash()
{
	/*Check with PlayerState whether we have the ability to BackDash*/
	if (PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::SkillTag[E_Skill::BackDash], SkillAbility)) /* Ref sent to CanBackDash. This is OUT Param */
	{
		//UE_LOG(Countess_Log, Warning, TEXT("Can Back Dash. from %s"), TEXT(__FUNCTION__));
		PlayerStateInterface->Execute_Countess_Interface_TryActivateAbilityByClass(GetPlayerState<APlayerState>(), SkillAbility);
	}
	//else
	//	UE_LOG(Countess_Log, Warning, TEXT("Can't Back Dash yet. Check if it is acquired. From %s"), TEXT(__FUNCTION__));
}

void ACountess_PlayerController::Ability_Primary()
{
	if(PlayerStateInterface->CanPrimary(PrimaryAbility))
	{
		//UE_LOG(Countess_Log, Warning, TEXT("Can do Primary Attack. from %s"), TEXT(__FUNCTION__));
		PlayerStateInterface->Execute_Countess_Interface_TryActivateAbilityByClass(GetPlayerState<APlayerState>(), PrimaryAbility);
	}
}

void ACountess_PlayerController::ActivateBMagicAbility()
{
	//Get the tag of our slotted ability (can be None) and check whether we did acquire it. If we have it then try to activate it.
	if(PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[BMagicSlotted], BlackMagicAbility))
	{
		const bool bBlackMagicActivated = PlayerStateInterface->Execute_Countess_Interface_TryActivateAbilityByClass(GetPlayerState<APlayerState>(), BlackMagicAbility);
		if (bBlackMagicActivated)
		{
			StartCooldownTimer(BlackMagicAbility, false);
		}
	}
}

void ACountess_PlayerController::ActivateWMagicAbility()
{
	//Get the tag of our slotted ability (can be None) and check whether we did acquire it. If we have it then try to activate it.
	if(PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::WMagicTag[WMagicSlotted], WhiteMagicAbility))
	{
		const bool bWhiteMagicActivated = PlayerStateInterface->Execute_Countess_Interface_TryActivateAbilityByClass(GetPlayerState<APlayerState>(), WhiteMagicAbility);
		if(bWhiteMagicActivated) // Our WhiteMagic Ability is activated now
		{
			StartCooldownTimer(WhiteMagicAbility, true);
		}
	}
}


void ACountess_PlayerController::StartCooldownTimer(TSubclassOf<UGameplayAbility> Ability, bool bIsAbilityWMagic)
{
	UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(Ability.GetDefaultObject());
	if (Ability_Base)
	{
		UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
		if (AbilityData)
		{
			const FString ContextString;
			if (!AbilityData->CoolDownRowHandle.IsValid(ContextString))
				return;

			const float Cooldown = AbilityData->CoolDownRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString);

			// Create a Timer Component which linearly interpolates b/w Start and End float value
			UCountess_Timer_Component* TimerComponent = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
			if (TimerComponent)
			{
				TimerComponent->RegisterComponent();
				if (Ability_Base->AbilityTags.HasTag(CountessTags::FAbilityTags::TimeSlowAbilityTag))
					TimerComponent->bIsAbilityTimeSlow = true;

				if(bIsAbilityWMagic)
				{
					TimerComponent->CountessTimerDelegate.AddUObject(this, &ACountess_PlayerController::SetWMagicAbilityCooldown, WMagicSlotted);
					//TimerComponent->CountessTestDelegate.BindUObject(this, &ACountess_PlayerController::DelegateTestFunction1, WMagicSlotted);
					//TimerComponent->CountessTestDelegate1.AddUObject(this, &ACountess_PlayerController::DelegateTestFunction1, WMagicSlotted);
				}
				else
				{
					TimerComponent->CountessTimerDelegate.AddUObject(this, &ACountess_PlayerController::SetBMagicAbilityCooldown, BMagicSlotted);
					//TimerComponent->CountessTestDelegate.BindUObject(this, &ACountess_PlayerController::DelegateTestFunction, BMagicSlotted);
					//TimerComponent->CountessTestDelegate1.AddUObject(this, &ACountess_PlayerController::DelegateTestFunction, BMagicSlotted);
				}

				TimerComponent->StartLerp(0, Cooldown);
			}
		}
	}
}

void ACountess_PlayerController::SetWMagicAbilityCooldown(float LerpedValue, E_WMagic WMagic)
{
	const float PercentageRemaining = 1 - LerpedValue;
	if (Countess_HUD->Get_Countess_HUDWidget())
		Countess_HUD->Get_Countess_HUDWidget()->SetWMagicAbilityCooldownPercentage(WMagic, PercentageRemaining);
		
}


void ACountess_PlayerController::SetBMagicAbilityCooldown(float LerpedValue, E_BMagic BMagic)
{
	 const float PercentageRemaining = 1 - LerpedValue;
	 if (Countess_HUD->Get_Countess_HUDWidget())
	 	Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityCooldownPercentage(BMagic, PercentageRemaining);
}

void ACountess_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*Get HUD*/
	Countess_HUD = Cast<ACountess_HUD>(GetHUD());
	if (Countess_HUD)
	{
		//TODO: check for all widgets instead of only Countess_HUD_Widget
		/*create widgets and populate them only if they are not already created*/
		if (!Countess_HUD->Get_Countess_HUDWidget())
		{
			Countess_HUD->CreateWidgets(Cast<APlayerController>(this));
			Countess_HUD_Widget = Countess_HUD->Get_Countess_HUDWidget();
		}
	}

	if(!Countess_HUD_Widget)
		return;

	PlayerStateInterface = GetPlayerState<ICountess_Interface_AbilityDetail>();

	if (!PlayerStateInterface)
		UE_LOG(Countess_Log, Error, TEXT("Player State Interface NOT found in %s."), TEXT(__FUNCTION__));

	/*Load Player Character Attribute values in our HUD Widget*/
	Countess_HUD_Widget->SetCurrentHealth(PlayerStateInterface->GetCurrentHealth());
	Countess_HUD_Widget->SetMaxHealth(PlayerStateInterface->GetMaxHealth());
	Countess_HUD_Widget->SetHealthPercentage(PlayerStateInterface->GetCurrentHealth() / PlayerStateInterface->GetMaxHealth());
	Countess_HUD_Widget->SetHealthRegenRate(PlayerStateInterface->GetHealthRegenRate());
	Countess_HUD_Widget->SetCurrentMana(PlayerStateInterface->GetCurrentMana());
	Countess_HUD_Widget->SetMaxMana(PlayerStateInterface->GetMaxMana());
	Countess_HUD_Widget->SetManaPercentage(PlayerStateInterface->GetCurrentMana() / PlayerStateInterface->GetMaxMana());
	Countess_HUD_Widget->SetManaRegenRate(PlayerStateInterface->GetManaRegenRate());
	Countess_HUD_Widget->SetArmor(PlayerStateInterface->GetArmor());
	Countess_HUD_Widget->SetCurrentStamina(PlayerStateInterface->GetCurrentStamina());
	Countess_HUD_Widget->SetMaxStamina(PlayerStateInterface->GetMaxStamina());
	Countess_HUD_Widget->SetStaminaPercentage(PlayerStateInterface->GetCurrentStamina() / PlayerStateInterface->GetMaxStamina());
	Countess_HUD_Widget->SetStaminaRegenRate(PlayerStateInterface->GetStaminaRegenRate());
	Countess_HUD_Widget->SetExp(PlayerStateInterface->GetCurrentExp());
	Countess_HUD_Widget->SetMaxExp(PlayerStateInterface->GetMaxExp());
	Countess_HUD_Widget->SetExpPercentage(PlayerStateInterface->GetCurrentExp() / PlayerStateInterface->GetMaxExp());
	Countess_HUD_Widget->SetPlayerLevel(PlayerStateInterface->GetPlayerLevel());

	/*Delegates to handle Player attribute changes from PlayerState which inturn receives changes from ASC*/

	PlayerStateInterface->GetCountessHealthChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnHealthChanged);
	PlayerStateInterface->GetCountessStaminaChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnStaminaChanged);
	PlayerStateInterface->GetCountessManaChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnManaChanged);
	PlayerStateInterface->GetCountessMaxHealthChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnMaxHealthChanged);
	PlayerStateInterface->GetCountessMaxStaminaChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnMaxStaminaChanged);
	PlayerStateInterface->GetCountessMaxManaChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnMaxManaChanged);
	PlayerStateInterface->GetCountessHealthRegenRateChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnHealthRegenRateChanged);
	PlayerStateInterface->GetCountessManaRegenRateChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnManaRegenRateChanged);
	PlayerStateInterface->GetCountessStaminaRegenRateChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnStaminaRegenRateChanged);
	PlayerStateInterface->GetCountessArmorChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnArmorChanged);
	PlayerStateInterface->GetCountessExpChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnExpChanged);
	PlayerStateInterface->GetCountessMaxExpChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnMaxExpChanged);
	PlayerStateInterface->GetCountessLevelChangedDelegate().AddDynamic(this, &ACountess_PlayerController::OnPlayerLevelIncreased);
	// #TODO: handle UI after Player loses Abilities too..
	/*Delegates to handle UI after Player acquires Abilities */
	PlayerStateInterface->GetCountessAbilityAcquiredDelegate().AddDynamic(this, &ACountess_PlayerController::OnAbilityAcquired);

	BMagicSlotted = E_BMagic::None;
	WMagicSlotted = E_WMagic::None;
	SkillAcquired = E_Skill::None;

	bHandlingAbilityAcquire = false;
	bBlackMagicMenuOpened = false;
	bWhiteMagicMenuOpened = false;

	// #TODO This 8 represents number of active abilities at a time. We currently have 8. Replace this with better logic?
	TimerBarWidgetComponents.Init(nullptr, 8);
	//Countess_HUD->CreateInventoryContainerWidget(this, ESlateVisibility::Hidden);

}

void ACountess_PlayerController::Interact()
{
	if (!bHandlingAbilityAcquire)
		return;
	if (Countess_HUD->Get_Countess_Notify_Widget())
	{
		if (Countess_HUD->Get_Countess_Notify_Widget()->IsInViewport())
		{
			bAbilityAcquired = PlayerStateInterface->Execute_Countess_Interface_AcquireAbilitiy(GetPlayerState<APlayerState>(), m_AbilityToAcquire);
			// Creating Countess_SkillAcquired_Widget is done in OnAbilityAcquired bound Delegate
		}
	}
}

void ACountess_PlayerController::EndInteract()
{
	if (!this->IsPaused())
		return;

	this->SetPause(false);
	FInputModeGameOnly GameOnly;
	this->SetInputMode(GameOnly);

	if (Countess_HUD->Get_Countess_Skill_Acquired_Widget())
	{
		if (Countess_HUD->Get_Countess_Skill_Acquired_Widget()->IsInViewport())
		{
			Countess_HUD->Get_Countess_Skill_Acquired_Widget()->RemoveFromParent();
		}
	}
	Remove_Notify_Widget_From_Parent();
}

bool ACountess_PlayerController::Handle_Acquire_Ability_OnOverlap(TSubclassOf<UCountess_GameplayAbility_Base>& AbilityToAcquire)
{
	if (PlayerStateInterface->GetAcquiredAbilities().Contains(AbilityToAcquire))
		return true;

	bAbilityAcquired = false;
	if (Countess_HUD->CreateNotifyWidget(this))
	{
		if (Countess_HUD->Get_Countess_Notify_Widget())
			if (!Countess_HUD->Get_Countess_Notify_Widget()->IsInViewport())
				Countess_HUD->Get_Countess_Notify_Widget()->AddToViewport();

		UGameplayStatics::PlaySound2D(this, NotifyWidgetOpenSound, 3.f);
		m_AbilityToAcquire = AbilityToAcquire;
		UCountess_GameplayAbility_Base* AbilityToAcquireCDO = Cast<UCountess_GameplayAbility_Base>(AbilityToAcquire->GetDefaultObject(false));
		if (AbilityToAcquireCDO->AbilityData->IsValidLowLevel())
			Countess_HUD->Get_Countess_Notify_Widget()->SetInteractiveTextMessage(AbilityToAcquireCDO->AbilityData->InteractiveMessage);

		bHandlingAbilityAcquire = true;
	}
	//return PlayerState->AcquireAbilitiy(AbilityToAcquire);
	return bAbilityAcquired;
}

bool ACountess_PlayerController::Handle_Acquire_Ability_EndOverlap()
{
	if (Countess_HUD->Get_Countess_Notify_Widget())
	{
		if(Countess_HUD->Get_Countess_Notify_Widget()->IsInViewport())
		{
			float Animation_Duration = Countess_HUD->Get_Countess_Notify_Widget()->Intro_Animation->GetEndTime() - Countess_HUD->Get_Countess_Notify_Widget()->Intro_Animation->GetStartTime();
			//UE_LOG(Countess_Log, Warning, TEXT("Animation Duration is %f"), Animation_Duration);
			if (NotifyWidgetCloseSound)
				UGameplayStatics::PlaySound2D(this, NotifyWidgetCloseSound, 3.f);
			Countess_HUD->Get_Countess_Notify_Widget()->PlayAnimationReverse(Countess_HUD->Get_Countess_Notify_Widget()->Intro_Animation);

			//Remove Notify Widget after a delay of length = playing intro animation in reverse
			GetWorld()->GetTimerManager().SetTimer(NotifyWidgetDelayHandle, this, &ACountess_PlayerController::Remove_Notify_Widget_From_Parent, Animation_Duration, false);
			bHandlingAbilityAcquire = false;
			return true;
		}
		return false;
	}
	return false;
}

void ACountess_PlayerController::Remove_Notify_Widget_From_Parent()
{
	Countess_HUD->Get_Countess_Notify_Widget()->RemoveFromParent();
	GetWorld()->GetTimerManager().ClearTimer(NotifyWidgetDelayHandle);
	//UE_LOG(Countess_Log, Warning, TEXT("Removed Notify Widget from Parent"));
}

void ACountess_PlayerController::Populate_Skill_Acquired_Widget(TSubclassOf<UCountess_GameplayAbility_Base>& AbilityToAcquire)
{
	if (Countess_HUD->Get_Countess_Skill_Acquired_Widget())
	{
		UCountess_GameplayAbility_Base* AbilityToAcquireCDO = Cast<UCountess_GameplayAbility_Base>(AbilityToAcquire->GetDefaultObject(false));
		if (AbilityToAcquireCDO)
		{
			// #TODO Populate border, image, stamina/mana cost etc
			if (AbilityToAcquireCDO->AbilityData.Get(false))
			{
				const UAbilityData* AbilityData = AbilityToAcquireCDO->AbilityData.Get(false);
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetWidgetTitle(AbilityData->Title);
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetWidgetDescription(AbilityData->Description);
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetWidgetAbilityIcon(AbilityData->AbilityIcon);
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetWidgetScreenShotImage(AbilityData->AbilityImage);

				const EAbilityCostType CostType = AbilityData->AbilityCostType;
				const FName CostName = AbilityData->AbilityCostString[CostType];
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetCostName(CostName);

				const int32 PlayerLevel = PlayerStateInterface->GetPlayerLevel();
				const int32 PlayerNextLevel = (PlayerLevel == PlayerStateInterface->GetPlayerMaxLevel()) ? PlayerLevel : (PlayerLevel + 1);
				const FString ContextString;
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetCost(AbilityData->CostRowHandle.Eval(PlayerLevel, ContextString));
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetNextLevelCost(AbilityData->CostRowHandle.Eval(PlayerNextLevel, ContextString));
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetCooldown(AbilityData->CoolDownRowHandle.Eval(PlayerLevel,ContextString));
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetNextLevelCooldown(AbilityData->CoolDownRowHandle.Eval(PlayerNextLevel,ContextString));
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetDamage(AbilityData->DamageRowHandle.Eval(PlayerLevel,ContextString));
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetNextLevelDamage(AbilityData->DamageRowHandle.Eval(PlayerNextLevel,ContextString));
			}
		}
	}
}

void ACountess_PlayerController::OpenBMagicMenu()
{
	if(Countess_HUD->Get_Countess_WMagic_Menu_Widget())
		if(Countess_HUD->Get_Countess_WMagic_Menu_Widget()->IsInViewport())
			return;
	
	if(Countess_HUD->Get_Countess_Skill_Acquired_Widget()) // End Skill Acquire Interaction if we are handling Skill Acquire and Player wanted to Open BMagic Menu
	{
		if(Countess_HUD->Get_Countess_Skill_Acquired_Widget()->IsInViewport())
			EndInteract();
	}
	
	if(Countess_HUD->Get_Countess_HUDWidget())
		Countess_HUD->Get_Countess_HUDWidget()->RemoveFromParent();
	
	if(!Countess_HUD->Get_Countess_BMagic_Menu_Widget())
		Countess_HUD->CreateBMagicMenuWidget(this);

	Populate_BMagicMenu_Widget(Countess_HUD->Get_Countess_BMagic_Menu_Widget());
	
	if(!Countess_HUD->Get_Countess_BMagic_Menu_Widget()->IsInViewport())
		Countess_HUD->Get_Countess_BMagic_Menu_Widget()->AddToViewport();

	bBlackMagicMenuOpened = true;
	
	this->SetPause(true);
	FInputModeGameAndUI GameAndUI;

	if(BMagicMenuOpenSound)
		UGameplayStatics::PlaySound2D(this, BMagicMenuOpenSound);
	
	GameAndUI.SetWidgetToFocus(Countess_HUD->Get_Countess_BMagic_Menu_Widget()->TakeWidget());
	this->SetInputMode(GameAndUI);
}

void ACountess_PlayerController::CloseBMagicMenu()
{
	if(Countess_HUD->Get_Countess_BMagic_Menu_Widget())
		Countess_HUD->Get_Countess_BMagic_Menu_Widget()->RemoveFromParent();

	if(!Countess_HUD->Get_Countess_HUDWidget()->IsInViewport())
		Countess_HUD->Get_Countess_HUDWidget()->AddToViewport();

	bBlackMagicMenuOpened = false;
	
	if(BMagicMenuCloseSound)
		UGameplayStatics::PlaySound2D(this, BMagicMenuCloseSound);
	
	this->SetPause(false);
	const FInputModeGameOnly GameOnly;
	this->SetInputMode(GameOnly);

	if(BMagicSlotted != E_BMagic::None)
		Countess_HUD->Get_Countess_HUDWidget()->PlayAnimOnBMagicSlotted();
}

void ACountess_PlayerController::OpenWMagicMenu()
{
	if(Countess_HUD->Get_Countess_BMagic_Menu_Widget())
		if(Countess_HUD->Get_Countess_BMagic_Menu_Widget()->IsInViewport())
			return;
	
	if(Countess_HUD->Get_Countess_Skill_Acquired_Widget()) // End Skill Acquire Interaction if we are handling Skill Acquire and Player wanted to Open BMagic Menu
		{
		if(Countess_HUD->Get_Countess_Skill_Acquired_Widget()->IsInViewport())
			EndInteract();
		}
	
	if(Countess_HUD->Get_Countess_HUDWidget())
		Countess_HUD->Get_Countess_HUDWidget()->RemoveFromParent();
	
	if(!Countess_HUD->Get_Countess_WMagic_Menu_Widget())
		Countess_HUD->CreateWMagicMenuWidget(this);

	Populate_WMagicMenu_Widget(Countess_HUD->Get_Countess_WMagic_Menu_Widget());
	
	if(!Countess_HUD->Get_Countess_WMagic_Menu_Widget()->IsInViewport())
		Countess_HUD->Get_Countess_WMagic_Menu_Widget()->AddToViewport();

	bWhiteMagicMenuOpened = true;
	
	this->SetPause(true);
	FInputModeGameAndUI GameAndUI;

	if(BMagicMenuOpenSound)
		UGameplayStatics::PlaySound2D(this, BMagicMenuOpenSound);
	
	GameAndUI.SetWidgetToFocus(Countess_HUD->Get_Countess_WMagic_Menu_Widget()->TakeWidget());
	this->SetInputMode(GameAndUI);
}

void ACountess_PlayerController::CloseWMagicMenu()
{
	if(Countess_HUD->Get_Countess_WMagic_Menu_Widget())
		Countess_HUD->Get_Countess_WMagic_Menu_Widget()->RemoveFromParent();

	if(!Countess_HUD->Get_Countess_HUDWidget()->IsInViewport())
		Countess_HUD->Get_Countess_HUDWidget()->AddToViewport();

	bWhiteMagicMenuOpened = false; 
	
	if(BMagicMenuCloseSound)
		UGameplayStatics::PlaySound2D(this, BMagicMenuCloseSound);
	
	this->SetPause(false);
	const FInputModeGameOnly GameOnly;
	this->SetInputMode(GameOnly);

	if(WMagicSlotted != E_WMagic::None)
		Countess_HUD->Get_Countess_HUDWidget()->PlayAnimOnWMagicSlotted();
}

void ACountess_PlayerController::Populate_BMagicMenu_Widget(UCountess_BMagic_Menu_Widget* BMagic_Menu_Widget)
{
	BMagic_Menu_Widget->SetCurrentMana(PlayerStateInterface->GetCurrentMana());
	BMagic_Menu_Widget->SetManaPercentage(PlayerStateInterface->GetCurrentMana()/PlayerStateInterface->GetMaxMana());
	if(WMagicSlotted != E_WMagic::None && SlottedWMagicAbilityData_WeakPtr.IsValid())
	{
		const FString ContextString;
		BMagic_Menu_Widget->SetSlottedWMagicAbilityImage(WMagicSlotted, SlottedWMagicAbilityData_WeakPtr.Get()->AbilityMenuImage);
		BMagic_Menu_Widget->SetSlottedWMagicAbilityCost(WMagicSlotted, SlottedWMagicAbilityData_WeakPtr.Get()->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
	}

	//Populate ElectroSpark
	if(PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::ElectroSpark],BlackMagicAbility))
	{
		UCountess_GameplayAbility_Base* BlackMagicAbilityCDO = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
		UAbilityData* AbilityData = BlackMagicAbilityCDO->AbilityData.Get();
		if(BlackMagicAbilityCDO && AbilityData)
		{
			const FString ContextString;
			BMagic_Menu_Widget->SetAbilityName(E_BMagic::ElectroSpark, AbilityData->Title);
			BMagic_Menu_Widget->SetAbilityCost(E_BMagic::ElectroSpark, AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			BMagic_Menu_Widget->SetAbilityImage(E_BMagic::ElectroSpark, AbilityData->AbilityMenuImage);
		}
	}

	//Populate Fireball
	if(PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::FireBall],BlackMagicAbility))
	{
		UCountess_GameplayAbility_Base* BlackMagicAbilityCDO = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
		UAbilityData* AbilityData = BlackMagicAbilityCDO->AbilityData.Get();
		if(BlackMagicAbilityCDO && AbilityData)
		{
			const FString ContextString;
			BMagic_Menu_Widget->SetAbilityName(E_BMagic::FireBall, AbilityData->Title);
			BMagic_Menu_Widget->SetAbilityCost(E_BMagic::FireBall, AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			BMagic_Menu_Widget->SetAbilityImage(E_BMagic::FireBall, AbilityData->AbilityMenuImage);
		}
	}

	//Populate BloodLust
	if (PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::BloodLust], BlackMagicAbility))
	{
		UCountess_GameplayAbility_Base* BlackMagicAbilityCDO = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
		UAbilityData* AbilityData = BlackMagicAbilityCDO->AbilityData.Get();
		if (BlackMagicAbilityCDO && AbilityData)
		{
			const FString ContextString;
			BMagic_Menu_Widget->SetAbilityName(E_BMagic::BloodLust, AbilityData->Title);
			BMagic_Menu_Widget->SetAbilityCost(E_BMagic::BloodLust, AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			BMagic_Menu_Widget->SetAbilityImage(E_BMagic::BloodLust, AbilityData->AbilityMenuImage);
		}
	}

	//Populate ArcticBlast
	if (PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::ArcticBlast], BlackMagicAbility))
	{
		UCountess_GameplayAbility_Base* BlackMagicAbilityCDO = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
		UAbilityData* AbilityData = BlackMagicAbilityCDO->AbilityData.Get();
		if (BlackMagicAbilityCDO && AbilityData)
		{
			const FString ContextString;
			BMagic_Menu_Widget->SetAbilityName(E_BMagic::ArcticBlast, AbilityData->Title);
			BMagic_Menu_Widget->SetAbilityCost(E_BMagic::ArcticBlast, AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			BMagic_Menu_Widget->SetAbilityImage(E_BMagic::ArcticBlast, AbilityData->AbilityMenuImage);
		}
	}
}

void ACountess_PlayerController::Populate_WMagicMenu_Widget(UCountess_WMagic_Menu_Widget* WMagic_Menu_Widget)
{
	WMagic_Menu_Widget->SetCurrentMana(PlayerStateInterface->GetCurrentMana());
	WMagic_Menu_Widget->SetManaPercentage(PlayerStateInterface->GetCurrentMana()/PlayerStateInterface->GetMaxMana());

	if (BMagicSlotted != E_BMagic::None && SlottedBMagicAbilityData_WeakPtr.IsValid())
	{
		const FString ContextString;
		WMagic_Menu_Widget->SetSlottedBMagicAbilityImage(BMagicSlotted, SlottedBMagicAbilityData_WeakPtr.Get()->AbilityMenuImage);
		WMagic_Menu_Widget->SetSlottedBMagicAbilityCost(BMagicSlotted, SlottedBMagicAbilityData_WeakPtr.Get()->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
	}

	//Populate Lens
	if(PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::WMagicTag[E_WMagic::LensOfTruth],WhiteMagicAbility))
	{
		UCountess_GameplayAbility_Base* WhiteMagicAbilityCDO = Cast<UCountess_GameplayAbility_Base>(WhiteMagicAbility.GetDefaultObject());
		UAbilityData* AbilityData = WhiteMagicAbilityCDO->AbilityData.Get();
		if(WhiteMagicAbilityCDO && AbilityData)
		{
			const FString ContextString;
			WMagic_Menu_Widget->SetAbilityName(E_WMagic::LensOfTruth, AbilityData->Title);
			WMagic_Menu_Widget->SetAbilityCost(E_WMagic::LensOfTruth, AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			WMagic_Menu_Widget->SetAbilityImage(E_WMagic::LensOfTruth, AbilityData->AbilityMenuImage);
		}
	}
	//Populate Mist
	if (PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::WMagicTag[E_WMagic::Mist], WhiteMagicAbility))
	{
		UCountess_GameplayAbility_Base* WhiteMagicAbilityCDO = Cast<UCountess_GameplayAbility_Base>(WhiteMagicAbility.GetDefaultObject());
		UAbilityData* AbilityData = WhiteMagicAbilityCDO->AbilityData.Get();
		if (WhiteMagicAbilityCDO && AbilityData)
		{
			const FString ContextString;
			WMagic_Menu_Widget->SetAbilityName(E_WMagic::Mist, AbilityData->Title);
			WMagic_Menu_Widget->SetAbilityCost(E_WMagic::Mist, AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			WMagic_Menu_Widget->SetAbilityImage(E_WMagic::Mist, AbilityData->AbilityMenuImage);
		}
	}
	//Populate TimeSlow
	if (PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::WMagicTag[E_WMagic::TimeSlow], WhiteMagicAbility))
	{
		UCountess_GameplayAbility_Base* WhiteMagicAbilityCDO = Cast<UCountess_GameplayAbility_Base>(WhiteMagicAbility.GetDefaultObject());
		UAbilityData* AbilityData = WhiteMagicAbilityCDO->AbilityData.Get();
		if (WhiteMagicAbilityCDO && AbilityData)
		{
			const FString ContextString;
			WMagic_Menu_Widget->SetAbilityName(E_WMagic::TimeSlow, AbilityData->Title);
			WMagic_Menu_Widget->SetAbilityCost(E_WMagic::TimeSlow, AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			WMagic_Menu_Widget->SetAbilityImage(E_WMagic::TimeSlow, AbilityData->AbilityMenuImage);
		}
	}
	//Populate Shield
	if (PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::WMagicTag[E_WMagic::Shield], WhiteMagicAbility))
	{
		UCountess_GameplayAbility_Base* WhiteMagicAbilityCDO = Cast<UCountess_GameplayAbility_Base>(WhiteMagicAbility.GetDefaultObject());
		UAbilityData* AbilityData = WhiteMagicAbilityCDO->AbilityData.Get();
		if (WhiteMagicAbilityCDO && AbilityData)
		{
			const FString ContextString;
			WMagic_Menu_Widget->SetAbilityName(E_WMagic::Shield, AbilityData->Title);
			WMagic_Menu_Widget->SetAbilityCost(E_WMagic::Shield, AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			WMagic_Menu_Widget->SetAbilityImage(E_WMagic::Shield, AbilityData->AbilityMenuImage);
		}
	}
}


void ACountess_PlayerController::ShowDamageNumber(float Damage, ACountess_Character_Base* TargetCharacter)
{
	//UE_LOG(Countess_Log, Warning, TEXT("From %s. %s received %f Damage."), TEXT(__FUNCTION__), *TargetCharacter->GetFName().ToString(), Damage);
	UCountess_DamageText_WidgetComp* DamageTextWC = NewObject<UCountess_DamageText_WidgetComp>(TargetCharacter, UCountess_DamageText_WidgetComp::StaticClass());
	if (DamageTextWC)
	{
		DamageTextWC->RegisterComponent();
		DamageTextWC->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextWC->SetDamageText(Damage);
	}
}

void ACountess_PlayerController::ShowTimerBarWidget(const FText& AbilityText, const float Duration, bool bIsAbilityTimeSlow, UCountess_Timer_Component* Timer_Component)
{
	// We are storing all the active timerbar_widgetcomponents in this array.
	// This helps in calculating the relative position of the newly created timerbar
	int32 Id = -1;
	for(int32 i = 0; i< TimerBarWidgetComponents.Num(); i++)
		if(TimerBarWidgetComponents[i] == nullptr)
		{
			Id = i;
			break;
		}

	UCountess_TimerBar_WidgetComp* TimerBar_WidgetComp = NewObject<UCountess_TimerBar_WidgetComp>(this->GetPawn(), UCountess_TimerBar_WidgetComp::StaticClass());
	if(TimerBar_WidgetComp)
	{
		TimerBar_WidgetComp->RegisterComponent();
		TimerBar_WidgetComp->AttachToComponent(this->GetPawn()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	
		// Tell the timerbarcomponent its position
		TimerBar_WidgetComp->ComponentId = Id;
		TimerBarWidgetComponents[Id] = TimerBar_WidgetComp;
		TimerBar_WidgetComp->bIsAbilityTimeSlow = bIsAbilityTimeSlow;
		// When the timerbarcomp is destroyed, we bind to this delegate, which helps in moving the relative positions of other active timerbars, if any
		TimerBar_WidgetComp->TimerBarComponentDestroyedDelegate.AddDynamic(this, &ACountess_PlayerController::ClearTimerBarId);
		TimerBar_WidgetComp->SetRelativeLocation(FVector(0,0,100.f * (0.5f * Id + 1)));
		TimerBar_WidgetComp->SetAbilityTextAndDuration(AbilityText, Duration, Timer_Component);
	}
}

void ACountess_PlayerController::ClearTimerBarId(const int32 Id)
{
	// Clear the position of the destroyed timerbarcomp
	TimerBarWidgetComponents[Id] = nullptr;

	// Go through the array and correct the positions of the next-inline timerbars' positions
	for(int32 i = Id+1; i < TimerBarWidgetComponents.Num(); i++)
	{
		// Stop when we encounter nullptr because all the next comps are also nullptr
		if(TimerBarWidgetComponents[i] == nullptr)
			break;

		// Set the position and id of this timerbar to the previous one (which has become vacant)
		TimerBarWidgetComponents[i-1] = TimerBarWidgetComponents[i];
		TimerBarWidgetComponents[i-1]->ComponentId = i-1;
		TimerBarWidgetComponents[i-1]->SetRelativeLocation(FVector(0,0,100.f * (0.5f * (i-1) + 1)));
		TimerBarWidgetComponents[i] = nullptr;

	}
}


void ACountess_PlayerController::OnHealthChanged(float NewHealthValue)
{
	//UE_LOG(Countess_Log, Warning, TEXT("Here from %s"), TEXT(__FUNCTION__));
	Countess_HUD_Widget->SetCurrentHealth(FMath::TruncateToHalfIfClose(NewHealthValue));
	Countess_HUD_Widget->SetHealthPercentage(NewHealthValue / PlayerStateInterface->GetMaxHealth());
}

void ACountess_PlayerController::OnStaminaChanged(float NewStaminaValue)
{
	Countess_HUD_Widget->SetCurrentStamina(FMath::TruncateToHalfIfClose(NewStaminaValue));
	Countess_HUD_Widget->SetStaminaPercentage(NewStaminaValue / PlayerStateInterface->GetMaxStamina());
}

void ACountess_PlayerController::OnManaChanged(float NewManaValue)
{
	Countess_HUD_Widget->SetCurrentMana(NewManaValue);
	Countess_HUD_Widget->SetManaPercentage(NewManaValue / PlayerStateInterface->GetMaxMana());
}

void ACountess_PlayerController::OnExpChanged(float NewExpValue)
{
	Countess_HUD_Widget->SetExp(NewExpValue);
	Countess_HUD_Widget->SetExpPercentage(NewExpValue / PlayerStateInterface->GetMaxExp());
}

void ACountess_PlayerController::OnMaxHealthChanged(float NewMaxHealthValue)
{
	Countess_HUD_Widget->SetMaxHealth(NewMaxHealthValue);
}

void ACountess_PlayerController::OnMaxStaminaChanged(float NewMaxStaminaValue)
{
	Countess_HUD_Widget->SetMaxStamina(NewMaxStaminaValue);
}

void ACountess_PlayerController::OnMaxManaChanged(float NewMaxManaValue)
{
	Countess_HUD_Widget->SetMaxMana(NewMaxManaValue);
}

void ACountess_PlayerController::OnMaxExpChanged(float NewMaxExpValue)
{
	Countess_HUD_Widget->SetMaxExp(NewMaxExpValue);
}

void ACountess_PlayerController::OnHealthRegenRateChanged(float NewHealthRegenValue)
{
	Countess_HUD_Widget->SetHealthRegenRate(NewHealthRegenValue);
}

void ACountess_PlayerController::OnManaRegenRateChanged(float NewManaRegenValue)
{
	Countess_HUD_Widget->SetManaRegenRate(NewManaRegenValue);
}

void ACountess_PlayerController::OnStaminaRegenRateChanged(float NewStaminaRegenValue)
{
	Countess_HUD_Widget->SetStaminaRegenRate(NewStaminaRegenValue);
}

void ACountess_PlayerController::OnArmorChanged(float NewArmorValue)
{
	Countess_HUD_Widget->SetArmor(NewArmorValue);
}

void ACountess_PlayerController::OnAbilityAcquired(TSubclassOf<UCountess_GameplayAbility_Base> AbilityAcquiredClass, FSlateBrush AbilityIcon, float Cooldown)
{
	UE_LOG(Countess_Log, Warning, TEXT("Success! Handling UI Ability Icons now. From %s. Coolddown for this abillity is %f"), TEXT(__FUNCTION__), Cooldown);
	if (SkillAcquiredSound)
		UGameplayStatics::PlaySound2D(this, SkillAcquiredSound, 3.f);
	this->SetPause(true);
	const FInputModeGameAndUI GameAndUI;
	this->SetInputMode(GameAndUI);
	Countess_HUD->CreateSkillAcquiredWidget(this);
	Populate_Skill_Acquired_Widget(AbilityAcquiredClass);
	Countess_HUD->Get_Countess_Skill_Acquired_Widget()->AddToViewport();

}

void ACountess_PlayerController::OnPlayerLevelIncreased(int32 NewPlayerLevel)
{
	Countess_HUD_Widget->SetPlayerLevel(NewPlayerLevel);
}

void ACountess_PlayerController::MenuOp()
{
	if(!bBlackMagicMenuOpened && !bWhiteMagicMenuOpened) 
		return;

	//Function to Handle Menu.
	//For black magic: Right->Fireball; Left->ElectroSpark; Up->Bloodlust; Down->ArcticBlast;
	//For white magic: Right->LensOfTruth; Left->Mist; Up->Shield; Down->TimeSlow
	

	//Equip that skill
	if(bBlackMagicMenuOpened && !bWhiteMagicMenuOpened)
	{
		if(this->IsInputKeyDown(FKey(FName("Right"))))
		{
			//UE_LOG(Countess_Log, Warning, TEXT("From %s. Right key was pressed."), TEXT(__FUNCTION__));
			if(!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::FireBall],BlackMagicAbility)) // Do Nothing if we don't have Fireball Ability
				return;

			Countess_HUD->Get_Countess_BMagic_Menu_Widget()->SelectedAbility(E_BMagic::FireBall);
			BMagicSlotted = E_BMagic::FireBall;
			Countess_HUD->Get_Countess_HUDWidget()->SetBMagicSlotted(BMagicSlotted);
			const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
			UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
			SlottedBMagicAbilityData_WeakPtr = AbilityData;
			if(AbilityData)
			{
				Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityImage(AbilityData->AbilityMenuImage);
				const FString ContextString;
				Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			}
		}
		else if(this->IsInputKeyDown(FKey(FName("Left"))))
		{
			//UE_LOG(Countess_Log, Warning, TEXT("From %s. Left key was pressed."), TEXT(__FUNCTION__));
			if(!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::ElectroSpark],BlackMagicAbility)) // Do Nothing if we don't have ElectroSpark Ability
				return;

			Countess_HUD->Get_Countess_BMagic_Menu_Widget()->SelectedAbility(E_BMagic::ElectroSpark);
			BMagicSlotted = E_BMagic::ElectroSpark;
			Countess_HUD->Get_Countess_HUDWidget()->SetBMagicSlotted(BMagicSlotted);
			const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
			UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
			SlottedBMagicAbilityData_WeakPtr = AbilityData;
			if(AbilityData)
			{
				Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityImage(AbilityData->AbilityMenuImage);
				const FString ContextString;
				Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			}
		}
		else if(this->IsInputKeyDown(FKey(FName("Up")))) //Bloodlust
		{
			//UE_LOG(Countess_Log, Warning, TEXT("From %s : BlackMagicMenu. Up key was pressed."), TEXT(__FUNCTION__));
			if (!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::BloodLust], BlackMagicAbility)) // Do Nothing if we don't have ElectroSpark Ability
				return;

			Countess_HUD->Get_Countess_BMagic_Menu_Widget()->SelectedAbility(E_BMagic::BloodLust);
			BMagicSlotted = E_BMagic::BloodLust;
			Countess_HUD->Get_Countess_HUDWidget()->SetBMagicSlotted(BMagicSlotted);
			const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
			UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
			SlottedBMagicAbilityData_WeakPtr = AbilityData;
			if (AbilityData)
			{
				Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityImage(AbilityData->AbilityMenuImage);
				const FString ContextString;
				Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			}
		}
		else if(this->IsInputKeyDown(FKey(FName("Down")))) //ArcticBlast
		{
			//UE_LOG(Countess_Log, Warning, TEXT("From %s : BlackMagicMenu. Up key was pressed."), TEXT(__FUNCTION__));
			if (!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::ArcticBlast], BlackMagicAbility)) // Do Nothing if we don't have ElectroSpark Ability
				return;

			Countess_HUD->Get_Countess_BMagic_Menu_Widget()->SelectedAbility(E_BMagic::ArcticBlast);
			BMagicSlotted = E_BMagic::ArcticBlast;
			Countess_HUD->Get_Countess_HUDWidget()->SetBMagicSlotted(BMagicSlotted);
			const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
			UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
			SlottedBMagicAbilityData_WeakPtr = AbilityData;
			if (AbilityData)
			{
				Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityImage(AbilityData->AbilityMenuImage);
				const FString ContextString;
				Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			}
		}
	}
	else if(!bBlackMagicMenuOpened && bWhiteMagicMenuOpened)
	{
		if(this->IsInputKeyDown(FKey(FName("Right")))) //Lens of Truth
		{
			//UE_LOG(Countess_Log, Warning, TEXT("From %s. Right key was pressed."), TEXT(__FUNCTION__));
			if(!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::WMagicTag[E_WMagic::LensOfTruth],WhiteMagicAbility)) // Do Nothing if we don't have LensOfTruth Ability
				return;

			Countess_HUD->Get_Countess_WMagic_Menu_Widget()->SelectedAbility(E_WMagic::LensOfTruth);
			WMagicSlotted = E_WMagic::LensOfTruth;
			Countess_HUD->Get_Countess_HUDWidget()->SetWMagicSlotted(WMagicSlotted);
			const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(WhiteMagicAbility.GetDefaultObject());
			UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
			SlottedWMagicAbilityData_WeakPtr = AbilityData;
			if(AbilityData)
			{
				Countess_HUD->Get_Countess_HUDWidget()->SetWMagicAbilityImage(AbilityData->AbilityMenuImage);
				const FString ContextString;
				Countess_HUD->Get_Countess_HUDWidget()->SetWMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			}
		}
		else if(this->IsInputKeyDown(FKey(FName("Left")))) // Mist
		{
			if (!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::WMagicTag[E_WMagic::Mist], WhiteMagicAbility)) // Do Nothing if we don't have Mist Ability
				return;

			Countess_HUD->Get_Countess_WMagic_Menu_Widget()->SelectedAbility(E_WMagic::Mist);
			WMagicSlotted = E_WMagic::Mist;
			Countess_HUD->Get_Countess_HUDWidget()->SetWMagicSlotted(WMagicSlotted);
			const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(WhiteMagicAbility.GetDefaultObject());
			UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
			SlottedWMagicAbilityData_WeakPtr = AbilityData;
			if (AbilityData)
			{
				Countess_HUD->Get_Countess_HUDWidget()->SetWMagicAbilityImage(AbilityData->AbilityMenuImage);
				const FString ContextString;
				Countess_HUD->Get_Countess_HUDWidget()->SetWMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			}
		}
		else if(this->IsInputKeyDown(FKey(FName("Up")))) //Shield
		{
			if (!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::WMagicTag[E_WMagic::Shield], WhiteMagicAbility)) // Do Nothing if we don't have Mist Ability
				return;

			Countess_HUD->Get_Countess_WMagic_Menu_Widget()->SelectedAbility(E_WMagic::Shield);
			WMagicSlotted = E_WMagic::Shield;
			Countess_HUD->Get_Countess_HUDWidget()->SetWMagicSlotted(WMagicSlotted);
			const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(WhiteMagicAbility.GetDefaultObject());
			UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
			SlottedWMagicAbilityData_WeakPtr = AbilityData;
			if (AbilityData)
			{
				Countess_HUD->Get_Countess_HUDWidget()->SetWMagicAbilityImage(AbilityData->AbilityMenuImage);
				const FString ContextString;
				Countess_HUD->Get_Countess_HUDWidget()->SetWMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			}
		}
		else if(this->IsInputKeyDown(FKey(FName("Down")))) //TimeSlow
		{
			if (!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::WMagicTag[E_WMagic::TimeSlow], WhiteMagicAbility)) // Do Nothing if we don't have Mist Ability
				return;

			Countess_HUD->Get_Countess_WMagic_Menu_Widget()->SelectedAbility(E_WMagic::TimeSlow);
			WMagicSlotted = E_WMagic::TimeSlow;
			Countess_HUD->Get_Countess_HUDWidget()->SetWMagicSlotted(WMagicSlotted);
			const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(WhiteMagicAbility.GetDefaultObject());
			UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
			SlottedWMagicAbilityData_WeakPtr = AbilityData;
			if (AbilityData)
			{
				Countess_HUD->Get_Countess_HUDWidget()->SetWMagicAbilityImage(AbilityData->AbilityMenuImage);
				const FString ContextString;
				Countess_HUD->Get_Countess_HUDWidget()->SetWMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
			}
		}
	}
}

/*****************************************************/
/* Inventory related */

void ACountess_PlayerController::ToggleInventory()
{
	if(!Countess_HUD->Get_Countess_Inventory_Container())
	{
		Countess_HUD->CreateInventoryContainerWidget(this);
		for(int32 Index = 0; Index < NumberOfSlots; Index++)
		{
			InventorySlots[Index].SlotNumber = Index;
		}
	}

	if(Countess_HUD->Get_Countess_Inventory_Container()->IsInViewport())
	{
		Countess_HUD->Get_Countess_Inventory_Container()->RemoveFromParent();
		this->SetPause(false);

		const FInputModeGameOnly GameOnly;
		this->SetInputMode(GameOnly);
		this->bShowMouseCursor = false;
		if(BMagicMenuCloseSound)
			UGameplayStatics::PlaySound2D(this, BMagicMenuCloseSound);
	}
	else
	{
		Countess_HUD->Get_Countess_Inventory_Container()->SetRenderTransformPivot(Countess_HUD->Get_Countess_HUDWidget()->RenderTransform.Translation);
		Countess_HUD->Get_Countess_Inventory_Container()->GenerateSlots(SlotsPerRow, NumberOfSlots, InventorySlots);



		Countess_HUD->Get_Countess_Inventory_Container()->Countess_Inventory_Use.Clear();
		Countess_HUD->Get_Countess_Inventory_Container()->Countess_Inventory_Use.AddDynamic(this, &ACountess_PlayerController::InventoryActionMenuUsePressed);

		Countess_HUD->Get_Countess_Inventory_Container()->Countess_Inventory_Drop.Clear();
		Countess_HUD->Get_Countess_Inventory_Container()->Countess_Inventory_Drop.AddDynamic(this, &ACountess_PlayerController::InventoryActionMenuDropPressed);

		Countess_HUD->Get_Countess_Inventory_Container()->Countess_Inventory_AddItemFromIndex.Clear();
		Countess_HUD->Get_Countess_Inventory_Container()->Countess_Inventory_AddItemFromIndex.AddDynamic(this, &ACountess_PlayerController::AddItemFromIndex);
		
		Countess_HUD->Get_Countess_Inventory_Container()->AddToViewport();
		this->SetPause(true);
		FInputModeGameAndUI GameAndUI;

		if(BMagicMenuOpenSound)
			UGameplayStatics::PlaySound2D(this, BMagicMenuOpenSound);
	
		GameAndUI.SetWidgetToFocus(Countess_HUD->Get_Countess_Inventory_Container()->TakeWidget());
		this->SetInputMode(GameAndUI);
		this->bShowMouseCursor = true;
	}
}

bool ACountess_PlayerController::IsSlotEmpty(int32 AtIndex) const
{
	return !(InventorySlots[AtIndex].IsValid());
}

UCountess_Item* ACountess_PlayerController::GetItemAtIndex(int32 AtIndex, int32& OutCountAtIndex) const
{
	UCountess_Item* ItemAtIndex = nullptr;
	if(InventorySlots[AtIndex].IsValid())
	{
		OutCountAtIndex = InventorySlots[AtIndex].Count;
		ItemAtIndex =  Cast<UCountess_Item>(InventorySlots[AtIndex].SlotItem.GetDefaultObject());
	}
	return ItemAtIndex;
}

bool ACountess_PlayerController::GetEmptySlot(int32& OutEmptySlotIndex) const
{
	bool bFoundEmptySlot = false;
	for(int32 Index = 0; Index< InventorySlots.Num(); Index++)
	{
		if(!InventorySlots[Index].IsValid())
		{
			OutEmptySlotIndex = Index;
			bFoundEmptySlot = true;
			break;
		}
	}
	return bFoundEmptySlot;
}

bool ACountess_PlayerController::GetFreeStackSlotForClass(TSubclassOf<UCountess_Item> ItemClass, int32& OutStackSlotIndex) const
{
	bool bFoundSlotIndex = false;
	for(int32 Index = 0; Index< InventorySlots.Num(); Index++)
	{
		if(InventorySlots[Index].IsValid() && InventorySlots[Index].SlotItem == ItemClass && InventorySlots[Index].Count < MaxStackSize)
		{
			OutStackSlotIndex = Index;
			bFoundSlotIndex = true;
			break;
		}
	}
	return bFoundSlotIndex;
}

bool ACountess_PlayerController::AddItemToInventory(TSubclassOf<UCountess_Item>& ItemClassToAdd, int32& CountToAdd)
{
	bool bItemAdded = false;
	int32 FreeIndex = -1;
	UCountess_Item* ItemToAddCDO = Cast<UCountess_Item>(ItemClassToAdd.GetDefaultObject());
	if(ItemToAddCDO->bCanBeStacked)
	{
		if(GetFreeStackSlotForClass(ItemClassToAdd, FreeIndex))
		{
			if(InventorySlots[FreeIndex].Count + CountToAdd <= MaxStackSize)
			{
				InventorySlots[FreeIndex].Count+= CountToAdd;
				CountToAdd =0;
				bItemAdded = true;
			}
			else
			{
				CountToAdd = InventorySlots[FreeIndex].Count + CountToAdd - MaxStackSize;
				InventorySlots[FreeIndex].Count = MaxStackSize;
				bItemAdded|= AddItemToInventory(ItemClassToAdd, CountToAdd);
			}
		}
		else
		{
			const bool bFoundEmptySlot = GetEmptySlot(FreeIndex);
			if(bFoundEmptySlot && CountToAdd > MaxStackSize)
			{
				SetItemAtInventorySlot(ItemClassToAdd, MaxStackSize, FreeIndex);
				CountToAdd-= MaxStackSize;
				bItemAdded|= AddItemToInventory(ItemClassToAdd, CountToAdd);
			}
			else if(bFoundEmptySlot && CountToAdd <= MaxStackSize)
			{
				SetItemAtInventorySlot(ItemClassToAdd, CountToAdd, FreeIndex);
				CountToAdd = 0;
				bItemAdded = true;
			}
		}
	}
	else
	{
		if(GetEmptySlot(FreeIndex))
		{
			SetItemAtInventorySlot(ItemClassToAdd, 1, FreeIndex); // Count =1 because this item can't be stacked
			CountToAdd--;
			bItemAdded = true;
			if(CountToAdd >= 1) // Add remaining items to next available free slots
				bItemAdded|= AddItemToInventory(ItemClassToAdd, CountToAdd);
			
		}
	}

	return bItemAdded;
}

bool ACountess_PlayerController::RemoveItemAtIndex(int32 Index, int32 NumberOfItemsToRemove)
{
	if(!IsSlotEmpty(Index) && NumberOfItemsToRemove >0)
	{
		int32 NumberOfItemsAtIndex = 0;
		if(GetItemAtIndex(Index, NumberOfItemsAtIndex))
		{
			if(NumberOfItemsAtIndex > NumberOfItemsToRemove)
			{
				InventorySlots[Index].Count = NumberOfItemsAtIndex - NumberOfItemsToRemove;
			}
			else
			{
				SetItemAtInventorySlot(nullptr, 0, Index);
				//Countess_HUD->Get_Countess_Inventory_Container()->RefreshSlots(InventorySlots);
			}
			if(Countess_HUD->Get_Countess_Inventory_Container())
				Countess_HUD->Get_Countess_Inventory_Container()->UpdateSlotAtIndex(Index, InventorySlots[Index]);
			return true;
		}
	}
	return false;
}

bool ACountess_PlayerController::SwapItemsAtIndices(int32 IndexA, int32 IndexB)
{
	if(IndexA >= InventorySlots.Num() || IndexB >= InventorySlots.Num())
		return false;

	const int32 Temp = InventorySlots[IndexA].SlotNumber;
	InventorySlots[IndexA].SlotNumber = InventorySlots[IndexB].SlotNumber;
	InventorySlots[IndexB].SlotNumber = Temp;
	InventorySlots.Swap(IndexA, IndexB);
	//Countess_HUD->Get_Countess_Inventory_Container()->RefreshSlots(InventorySlots);
	if(Countess_HUD->Get_Countess_Inventory_Container())
	{
		Countess_HUD->Get_Countess_Inventory_Container()->UpdateSlotAtIndex(IndexA, InventorySlots[IndexA]);
		Countess_HUD->Get_Countess_Inventory_Container()->UpdateSlotAtIndex(IndexB, InventorySlots[IndexB]);
	}
	return true;
}

void ACountess_PlayerController::UseItemAtIndex(int32 Index)
{
	if(InventorySlots[Index].IsValid())
	{
		UCountess_Item* ItemToUse = InventorySlots[Index].SlotItem.GetDefaultObject();
		if(ItemToUse)
		{
			ItemToUse->OnItemUse();
			InventorySlots[Index].Count--;
			if(InventorySlots[Index].Count <=0)
				InventorySlots[Index].SlotItem = nullptr;
			if(Countess_HUD->Get_Countess_Inventory_Container())
				Countess_HUD->Get_Countess_Inventory_Container()->UpdateSlotAtIndex(Index, InventorySlots[Index]);
		}
	}
}

void ACountess_PlayerController::InventoryActionMenuUsePressed(int32 Index)
{
	UE_LOG(Countess_Log, Warning, TEXT("From %s. ActionMenu Use detected. Index is %d"), TEXT(__FUNCTION__), Index);
	UseItemAtIndex(Index);
}

void ACountess_PlayerController::InventoryActionMenuDropPressed(int32 Index, int32 Count)
{
	UE_LOG(Countess_Log, Warning, TEXT("From %s. ActionMenu Drop detected. Index is %d and Count is %d"), TEXT(__FUNCTION__), Index, Count);
	RemoveItemAtIndex(Index, Count);
}

void ACountess_PlayerController::AddItemFromIndex(const int32 FromIndex, const int32 ToIndex)
{
	//
	if(InventorySlots[FromIndex].IsValid() && InventorySlots[ToIndex].IsValid())
	{
		// Items can be stacked. So we can add items to ToIndex
		if(InventorySlots[FromIndex].SlotItem == InventorySlots[ToIndex].SlotItem && InventorySlots[FromIndex].SlotItem.GetDefaultObject()->bCanBeStacked)
		{
			if(InventorySlots[ToIndex].Count + InventorySlots[FromIndex].Count <= MaxStackSize)
			{
				// Transfer all items from FromIndex to To Index and invalidate FromIndex
				SetItemAtInventorySlot(InventorySlots[ToIndex].SlotItem, InventorySlots[ToIndex].Count + InventorySlots[FromIndex].Count, ToIndex);
				SetItemAtInventorySlot(nullptr, -1, FromIndex);
			}
			else
			{
				SetItemAtInventorySlot(InventorySlots[FromIndex].SlotItem, InventorySlots[FromIndex].Count - (MaxStackSize - InventorySlots[ToIndex].Count), FromIndex);
				SetItemAtInventorySlot(InventorySlots[ToIndex].SlotItem, MaxStackSize, ToIndex);
			}
		}
		// Items can't be stacked. Or their classes are different. Only option is to swap them
		else
		{
			SwapItemsAtIndices(FromIndex, ToIndex);
		}
	}
	else if(InventorySlots[FromIndex].IsValid() && !InventorySlots[ToIndex].IsValid())
	{
		SwapItemsAtIndices(FromIndex, ToIndex);
	}
}

void ACountess_PlayerController::SetItemAtInventorySlot(const TSubclassOf<UCountess_Item>& ItemClassToSet, const int32& CountToSet, const int32& IndexToSet)
{
	InventorySlots[IndexToSet].SlotItem = ItemClassToSet;
	InventorySlots[IndexToSet].Count = CountToSet;
	InventorySlots[IndexToSet].SlotNumber = IndexToSet;
	if(Countess_HUD->Get_Countess_Inventory_Container())
	{
		Countess_HUD->Get_Countess_Inventory_Container()->UpdateSlotAtIndex(IndexToSet, InventorySlots[IndexToSet]);
	}
}
