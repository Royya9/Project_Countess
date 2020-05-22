// Free to play. By SoNa Games.


#include "Player/Countess_PlayerController.h"
#include "Characters/Project_CountessCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Countess_PlayerState.h"
#include "UI/Countess_HUD.h"
#include "UI/Countess_HUD_Widget.h"

void ACountess_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	if (aPawn)
	{
		PlayerCharacter = Cast<AProject_CountessCharacter>(aPawn);
		if (PlayerCharacter)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s Possessed %s"), *this->GetName(), *PlayerCharacter->GetName());
			SetViewTarget(PlayerCharacter);

			InputComponent->BindAction("Jump", IE_Pressed, this, &ACountess_PlayerController::Ability_Jump);
			InputComponent->BindAction("Jump", IE_Released, this, &ACountess_PlayerController::Ability_StopJumping);
			InputComponent->BindAxis("MoveRight", this, &ACountess_PlayerController::MoveRight);
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
	
	TSubclassOf<UGameplayAbility> JumpAbility;
	/*Check with PlayerState whether we have the ability to Jump*/
	if (PlayerState->CanJump(JumpAbility) && !PlayerCharacter->GetCharacterMovement()->IsFalling())
	{
		PlayerState->Countess_TryActivateAbilityByClass(JumpAbility);
	}
}

void ACountess_PlayerController::Ability_StopJumping()
{
	TSubclassOf<UGameplayAbility> JumpAbility;
	/*Check with PlayerState whether we have the ability to Jump*/
	if (PlayerState->CanJump(JumpAbility))
	{
		PlayerState->Countess_CancelAbility(JumpAbility);
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

	PlayerState = GetPlayerState<ACountess_PlayerState>();
	if (!PlayerState)
		return;

	/*Load Player Character Attribute values in our HUD Widget*/
	Countess_HUD_Widget->SetCurrentHealth(PlayerState->GetCurrentHealth());
	Countess_HUD_Widget->SetMaxHealth(PlayerState->GetMaxHealth());
	Countess_HUD_Widget->SetHealthPercentage(PlayerState->GetCurrentHealth()/ PlayerState->GetMaxHealth());
	Countess_HUD_Widget->SetHealthRegenRate(PlayerState->GetHealthRegenRate());
	Countess_HUD_Widget->SetCurrentMana(PlayerState->GetCurrentMana());
	Countess_HUD_Widget->SetMaxMana(PlayerState->GetMaxMana());
	Countess_HUD_Widget->SetManaPercentage(PlayerState->GetCurrentMana()/ PlayerState->GetMaxMana());
	Countess_HUD_Widget->SetManaRegenRate(PlayerState->GetManaRegenRate());
	Countess_HUD_Widget->SetArmor(PlayerState->GetArmor());
	Countess_HUD_Widget->SetCurrentStamina(PlayerState->GetCurrentStamina());
	Countess_HUD_Widget->SetMaxStamina(PlayerState->GetMaxStamina());
	Countess_HUD_Widget->SetStaminaPercentage(PlayerState->GetCurrentStamina() / PlayerState->GetMaxStamina());
	Countess_HUD_Widget->SetStaminaRegenRate(PlayerState->GetStaminaRegenRate());
	Countess_HUD_Widget->SetExp(PlayerState->GetCurrentExp());
	Countess_HUD_Widget->SetMaxExp(PlayerState->GetMaxExp());
	Countess_HUD_Widget->SetExpPercentage(PlayerState->GetCurrentExp() / PlayerState->GetMaxExp());
	Countess_HUD_Widget->SetPlayerLevel(PlayerState->GetPlayerLevel());

	/*Delegates to handle Player attribute changes from PlayerState which inturn receives changes from ASC*/

	PlayerState->Countess_Health_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnHealthChanged);
	PlayerState->Countess_Stamina_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnStaminaChanged);
	PlayerState->Countess_Mana_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnManaChanged);
	PlayerState->Countess_MaxHealth_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnMaxHealthChanged);
	PlayerState->Countess_MaxStamina_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnMaxStaminaChanged);
	PlayerState->Countess_MaxMana_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnMaxManaChanged);
	PlayerState->Countess_HealthRegenRate_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnHealthRegenRateChanged);
	PlayerState->Countess_ManaRegenRate_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnManaRegenRateChanged);
	PlayerState->Countess_StaminaRegenRate_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnStaminaRegenRateChanged);
	PlayerState->Countess_Armor_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnArmorChanged);
	PlayerState->Countess_Exp_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnExpChanged);
	PlayerState->Countess_MaxExp_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnMaxExpChanged);
	PlayerState->Countess_Level_Changed_Delegate_TO_BE_REFACTORED_COZ_DECLARED_TWICE_IN_PLAYERSTATE_TOO.AddDynamic(this, &ACountess_PlayerController::OnPlayerLevelIncreased);
	// #TODO: handle UI after Player loses Abilities too..
	/*Delegates to handle UI after Player acquires Abilities */
	PlayerState->Countess_Ability_Acquired_Delegate.AddDynamic(this, &ACountess_PlayerController::OnAbilityAcquired);

	BMagicSlotted = E_BMagic::None;
	WMagicSlotted = E_WMagic::None;
	SkillAcquired = E_Skills::None;
}

void ACountess_PlayerController::OnHealthChanged(float NewHealthValue)
{
	//UE_LOG(Countess_Log, Warning, TEXT("Here from %s"), TEXT(__FUNCTION__));
	Countess_HUD_Widget->SetCurrentHealth(FMath::TruncateToHalfIfClose(NewHealthValue));
	Countess_HUD_Widget->SetHealthPercentage(NewHealthValue / PlayerState->GetMaxHealth());
}

void ACountess_PlayerController::OnStaminaChanged(float NewStaminaValue)
{
	Countess_HUD_Widget->SetCurrentStamina(FMath::TruncateToHalfIfClose(NewStaminaValue));
	Countess_HUD_Widget->SetStaminaPercentage(NewStaminaValue / PlayerState->GetMaxStamina());
}

void ACountess_PlayerController::OnManaChanged(float NewManaValue)
{
	Countess_HUD_Widget->SetCurrentMana(NewManaValue);
	Countess_HUD_Widget->SetManaPercentage(NewManaValue / PlayerState->GetMaxMana());
}

void ACountess_PlayerController::OnExpChanged(float NewExpValue)
{
	Countess_HUD_Widget->SetExp(NewExpValue);
	Countess_HUD_Widget->SetExpPercentage(NewExpValue / PlayerState->GetMaxExp());
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

void ACountess_PlayerController::OnAbilityAcquired(FSlateBrush AbilityIcon, float Cooldown)
{
	UE_LOG(Countess_Log, Warning, TEXT("Success! Handling UI Ability Icons now. From %s. Coolddown for this abillity is %f"), TEXT(__FUNCTION__), Cooldown);
	Countess_HUD_Widget->SetWMagicAbilityIcon(AbilityIcon);
}

void ACountess_PlayerController::OnPlayerLevelIncreased(int32 NewPlayerLevel)
{
	Countess_HUD_Widget->SetPlayerLevel(NewPlayerLevel);
}
