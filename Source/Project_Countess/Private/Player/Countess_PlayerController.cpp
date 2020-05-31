// Free to play. By SoNa Games.


#include "Player/Countess_PlayerController.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "UI/Countess_HUD.h"
#include "UI/Countess_HUD_Widget.h"
#include "UI/Countess_Notify_Widget.h"
#include "UI/Countess_SkillAcquired_Widget.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Interfaces/Countess_Interface_AbilityDetail.h"
#include "Camera/Countess_CameraManager.h"

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

	PlayerCameraManagerClass = ACountess_CameraManager::StaticClass();
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
			InputComponent->BindAction("Interact", IE_Pressed, this, &ACountess_PlayerController::Interact);
			/*InputComponent->BindAction("EndInteract", IE_Pressed, this, &ACountess_PlayerController::EndInteract).bExecuteWhenPaused = true;*/
			FInputActionBinding& EndInteractBinding = InputComponent->BindAction("EndInteract", IE_Pressed, this, &ACountess_PlayerController::EndInteract);
			EndInteractBinding.bConsumeInput = false;
			EndInteractBinding.bExecuteWhenPaused = true;
			InputComponent->BindAction("BackDash", IE_Pressed, this, &ACountess_PlayerController::Ability_BackDash);
			InputComponent->BindAction("Fireball", IE_Pressed, this, &ACountess_PlayerController::Ability_Fireball);
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

/*
void ACountess_PlayerController::Ability_StopJumping(const FHitResult& Hit)
{
	//PlayerState->Countess_CancelAbility(JumpAbility);
	//PlayerStateInterface->Execute_Countess_Interface_CancelAbility(GetPlayerState<APlayerState>(), JumpAbility);

}*/

void ACountess_PlayerController::Ability_BackDash()
{
	/*Check with PlayerState whether we have the ability to BackDash*/
	if (PlayerStateInterface->CanBackDash(BackDashAbility)) /* Ref sent to CanBackDash. This is OUT Param */
	{
		//UE_LOG(Countess_Log, Warning, TEXT("Can Back Dash. from %s"), TEXT(__FUNCTION__));
		PlayerStateInterface->Execute_Countess_Interface_TryActivateAbilityByClass(GetPlayerState<APlayerState>(), BackDashAbility);
	}
	//else
	//	UE_LOG(Countess_Log, Warning, TEXT("Can't Back Dash yet. Check if it is acquired. From %s"), TEXT(__FUNCTION__));
}


void ACountess_PlayerController::Ability_Fireball()
{
	/*Check with PlayerState whether we have the ability to Fireball*/
	// #TODO /*Actually check with PlayerState which blackmagic ability is slotted right and return that ability. For testing we are asking about fireball ability*/
	if (PlayerStateInterface->CanFireball(BlackMagicAbility))
	{
		UE_LOG(Countess_Log, Warning, TEXT("Can Fireball. from %s"), TEXT(__FUNCTION__));
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
	SkillAcquired = E_Skills::None;

	bHandlingAbilityAcquire = false;

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
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetWidgetTitle(AbilityToAcquireCDO->AbilityData.Get(false)->Title);
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetWidgetDescription(AbilityToAcquireCDO->AbilityData.Get(false)->Description);
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetWidgetAbilityIcon(AbilityToAcquireCDO->AbilityData.Get(false)->AbilityIcon);
				Countess_HUD->Get_Countess_Skill_Acquired_Widget()->SetWidgetScreenShotImage(AbilityToAcquireCDO->AbilityData.Get(false)->AbilityImage);
			}
		}
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
	Countess_HUD_Widget->SetWMagicAbilityIcon(AbilityIcon);
	if (SkillAcquiredSound)
		UGameplayStatics::PlaySound2D(this, SkillAcquiredSound, 3.f);
	this->SetPause(true);
	FInputModeGameAndUI GameAndUI;
	this->SetInputMode(GameAndUI);
	Countess_HUD->CreateSkillAcquiredWidget(this);
	Populate_Skill_Acquired_Widget(AbilityAcquiredClass);
	Countess_HUD->Get_Countess_Skill_Acquired_Widget()->AddToViewport();

}

void ACountess_PlayerController::OnPlayerLevelIncreased(int32 NewPlayerLevel)
{
	Countess_HUD_Widget->SetPlayerLevel(NewPlayerLevel);
}
