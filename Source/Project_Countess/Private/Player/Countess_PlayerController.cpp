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


	/*Delegates to handle Player attribute changes from PlayerState which inturn receives changes from ASC*/

	PlayerState->Countess_Health_Changed_Delegate.AddDynamic(this, &ACountess_PlayerController::OnHealthChanged);

	// #TODO: handle UI after Player loses Abilities too..
	/*Delegates to handle UI after Player acquires Abilities */
	PlayerState->Countess_Ability_Acquired_Delegate.AddDynamic(this, &ACountess_PlayerController::OnAbilityAcquired);

	BMagicSlotted = E_BMagic::None;
	WMagicSlotted = E_WMagic::None;
	SkillAcquired = E_Skills::None;
}

void ACountess_PlayerController::OnHealthChanged(float NewHealthValue)
{
	UE_LOG(Countess_Log, Warning, TEXT("Here from %s"), TEXT(__FUNCTION__));
	Countess_HUD_Widget->SetCurrentHealth(NewHealthValue);
	
}

void ACountess_PlayerController::OnAbilityAcquired(FSlateBrush AbilityIcon, float Cooldown)
{
	UE_LOG(Countess_Log, Warning, TEXT("Success! Handling UI Ability Icons now. From %s. Coolddown for this abillity is %f"), TEXT(__FUNCTION__), Cooldown);
	Countess_HUD_Widget->SetWMagicAbilityIcon(AbilityIcon);
}
