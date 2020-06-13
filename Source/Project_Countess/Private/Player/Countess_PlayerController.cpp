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
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Interfaces/Countess_Interface_AbilityDetail.h"
#include "Camera/Countess_CameraManager.h"
#include "Globals/Countess_AbilityTypes.h"

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
			InputComponent->BindAction("MenuOps", IE_Pressed, this, &ACountess_PlayerController::MenuOp).bExecuteWhenPaused = true;
			InputComponent->BindAction("Interact", IE_Pressed, this, &ACountess_PlayerController::Interact);
			/*InputComponent->BindAction("EndInteract", IE_Pressed, this, &ACountess_PlayerController::EndInteract).bExecuteWhenPaused = true;*/
			FInputActionBinding& EndInteractBinding = InputComponent->BindAction("EndInteract", IE_Pressed, this, &ACountess_PlayerController::EndInteract);
			EndInteractBinding.bConsumeInput = false;
			EndInteractBinding.bExecuteWhenPaused = true;
			InputComponent->BindAction("BackDash", IE_Pressed, this, &ACountess_PlayerController::Ability_BackDash);
			InputComponent->BindAction("ActivateBlackMagicAbility", IE_Pressed, this, &ACountess_PlayerController::ActivateBMagicAbility);
			InputComponent->BindAction("Primary", IE_Pressed, this, &ACountess_PlayerController::Ability_Primary);
			FInputActionBinding& OpenBMagicMenuBinding = InputComponent->BindAction("BMagicMenu", IE_Pressed, this, &ACountess_PlayerController::OpenBMagicMenu);
			OpenBMagicMenuBinding.bExecuteWhenPaused = true;
			FInputActionBinding& CloseBMagicMenuBinding = InputComponent->BindAction("BMagicMenu", IE_Released, this, &ACountess_PlayerController::CloseBMagicMenu);
			CloseBMagicMenuBinding.bExecuteWhenPaused = true;
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
		PlayerStateInterface->Execute_Countess_Interface_TryActivateAbilityByClass(GetPlayerState<APlayerState>(), BlackMagicAbility);
	}
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
	//PlayerCharacter->LandedDelegate.AddDynamic(this, &ACountess_PlayerController::Ability_StopJumping);
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

void ACountess_PlayerController::Populate_BMagicMenu_Widget(UCountess_BMagic_Menu_Widget* BMagic_Menu_Widget)
{
	BMagic_Menu_Widget->SetCurrentMana(PlayerStateInterface->GetCurrentMana());
	BMagic_Menu_Widget->SetManaPercentage(PlayerStateInterface->GetCurrentMana()/PlayerStateInterface->GetMaxMana());

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
	if(!bBlackMagicMenuOpened)
		return;

	//Function to Handle Menu. For black magic: Right->Fireball; Left->ElectroSpark; Up->Bloodlust; Down->ArcticBlast;

	//Equip that skill

	if(this->IsInputKeyDown(FKey(FName("Right"))))
	{
		//UE_LOG(Countess_Log, Warning, TEXT("From %s. Right key was pressed."), TEXT(__FUNCTION__));
		if(!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::FireBall],BlackMagicAbility)) // Do Nothing if we don't have Fireball Ability
			return;

		Countess_HUD->Get_Countess_BMagic_Menu_Widget()->SelectedAbility(E_BMagic::FireBall);
		BMagicSlotted = E_BMagic::FireBall;
		const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
		const UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
		if(AbilityData)
		{
			Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityImage(AbilityData->AbilityMenuImage);
			const FString ContextString;
			Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
		}
	}
	if(this->IsInputKeyDown(FKey(FName("Left"))))
	{
		//UE_LOG(Countess_Log, Warning, TEXT("From %s. Left key was pressed."), TEXT(__FUNCTION__));
		if(!PlayerStateInterface->CanActivateAbilityByTagGeneric(CountessTags::BMagicTag[E_BMagic::ElectroSpark],BlackMagicAbility)) // Do Nothing if we don't have ElectroSpark Ability
			return;

		Countess_HUD->Get_Countess_BMagic_Menu_Widget()->SelectedAbility(E_BMagic::ElectroSpark);
		BMagicSlotted = E_BMagic::ElectroSpark;
		const UCountess_GameplayAbility_Base* Ability_Base = Cast<UCountess_GameplayAbility_Base>(BlackMagicAbility.GetDefaultObject());
		const UAbilityData* AbilityData = Ability_Base->AbilityData.Get();
		if(AbilityData)
		{
			Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityImage(AbilityData->AbilityMenuImage);
			const FString ContextString;
			Countess_HUD->Get_Countess_HUDWidget()->SetBMagicAbilityCost(AbilityData->CostRowHandle.Eval(PlayerStateInterface->GetPlayerLevel(), ContextString));
		}
	}
	
	if(this->IsInputKeyDown(FKey(FName("Up"))))
		UE_LOG(Countess_Log, Warning, TEXT("From %s. Up key was pressed."), TEXT(__FUNCTION__));
	if(this->IsInputKeyDown(FKey(FName("Down"))))
		UE_LOG(Countess_Log, Warning, TEXT("From %s. Down key was pressed."), TEXT(__FUNCTION__));

}