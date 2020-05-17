// Free to play. By SoNa Games.


#include "Player/Countess_PlayerState.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Characters/GameplayAbilities/Effects/Countess_GameplayEffect_Base.h"




ACountess_PlayerState::ACountess_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCountess_AbilitySystemComponent>(FName("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UCountess_AttributeSet_Base>(FName("AttributeSet"));
	
}

void ACountess_PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ACountess_PlayerState::OnHealthChanged);
}

bool ACountess_PlayerState::AcquireAbilitiy(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToAcquire)
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(Countess_Log, Error, TEXT("Ability System Component is nullptr from %s"), TEXT(__FUNCTION__));
		return false;
	}
	
	// Acquire only those Abilities that which are not already acquired.
	for (auto Ability : AcquiredAbilities)
	{
		if (Ability == AbilityToAcquire)
			return false;
	}

	//We are giving ability to our ASC
	FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAcquire, 1, INDEX_NONE, this));
	//stating that the ownerActor of this ability is the this (PlayerState) and AvatarActor which physically acquires the ability is our PlayerCharacter (GetPawn())
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
	
	//Adding Acquired ability to our list
	AcquiredAbilities.Add(AbilityToAcquire);
	
	UCountess_GameplayAbility_Base* AbilityToAcquireCDO = Cast<UCountess_GameplayAbility_Base>(AbilityToAcquire->GetDefaultObject(false));
	//FGameplayAbilityActorInfo* myinfo = AbilitySystemComponent->AbilityActorInfo.Get();

	//Process of getting the duration of the cooldown duration
	float Duration = -1.f;
	UCountess_GameplayEffect_Base* GameplayEffectCDO = Cast<UCountess_GameplayEffect_Base>(AbilityToAcquireCDO->GetCooldownGameplayEffect());
	if (GameplayEffectCDO)
		GameplayEffectCDO->DurationMagnitude.GetStaticMagnitudeIfPossible(1, Duration);
	
	//Below comment is how to get cooldown duration remaining when this ability is active 	
 	//AbilityToAcquireCDO->GetCooldownTimeRemainingAndDuration(AbilitySpecHandle, myinfo, Cd, Duration);


	UE_LOG(Countess_Log, Warning, TEXT("Cooldown duration read on PS in %s is %f "), TEXT(__FUNCTION__), Duration);
	if(AbilityToAcquireCDO->AbilityData->IsValidLowLevel())
	{
		//Broadcasting our acquired ability details to listeners.
		// #TODO Combine all these details to decrease the number of params getting broadcasted. Turn them to a global struct maybe? 
		Countess_Ability_Acquired_Delegate.Broadcast(AbilityToAcquireCDO->AbilityData->AbilityIcon, Duration);
		UE_LOG(Countess_Log, Warning, TEXT("Acquired Ability is %s and Description is %s"), *AbilityToAcquire->GetName(), *AbilityToAcquireCDO->AbilityData->Description.ToString());
	}
	else
	{
		UE_LOG(Countess_Log, Error, TEXT("Acquired Ability is %s and has no associated AbilityData with it."), *AbilityToAcquire->GetName());
	}
	
	return true;
}

bool ACountess_PlayerState::Countess_TryActivateAbilityByClass(TSubclassOf<UGameplayAbility>& AbilityToActivate)
{
	return AbilitySystemComponent->TryActivateAbilityByClass(AbilityToActivate);
}

void ACountess_PlayerState::Countess_CancelAbility(TSubclassOf<UGameplayAbility>& AbilityToCancel)
{
	AbilitySystemComponent->CancelAbility(GetAbilityObjectFromClass(AbilityToCancel));
}

UAbilitySystemComponent* ACountess_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGameplayAbility* ACountess_PlayerState::GetAbilityObjectFromClass(TSubclassOf<UGameplayAbility>& Ability)
{
	return Cast<UGameplayAbility>(Ability->GetDefaultObject(false));
}

bool ACountess_PlayerState::IsAlive() const
{
	return AttributeSet->GetHealth() > 0.f;
}

bool ACountess_PlayerState::CanJump(TSubclassOf<UGameplayAbility>& JumpAbility) const
{
	for (auto Ability : AcquiredAbilities)
	{
		if (Ability.GetDefaultObject()->AbilityTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Ability.Jump"))))
		{
			JumpAbility = Ability;
			return true;
		}

	}
	return false;
}

float ACountess_PlayerState::GetCurrentHealth() const
{
	return AttributeSet->GetHealth();
}

float ACountess_PlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ACountess_PlayerState::GetHealthRegenRate() const
{
	return AttributeSet->GetHealthRegenRate();
}

float ACountess_PlayerState::GetArmor() const
{
	return AttributeSet->GetArmor();
}

float ACountess_PlayerState::GetCurrentMana() const
{
	return AttributeSet->GetMana();
}

float ACountess_PlayerState::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float ACountess_PlayerState::GetManaRegenRate() const
{
	return AttributeSet->GetManaRegenRate();
}

float ACountess_PlayerState::GetCurrentStamina() const
{
	return AttributeSet->GetStamina();
}

float ACountess_PlayerState::GetMaxStamina() const
{
	return AttributeSet->GetMaxStamina();
}

float ACountess_PlayerState::GetStaminaRegenRate() const
{
	return AttributeSet->GetStaminaRegenRate();
}

void ACountess_PlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	Countess_Health_Changed_Delegate.Broadcast(Data.NewValue);
}
