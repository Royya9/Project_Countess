// Free to play. By SoNa Games.


#include "Player/Countess_PlayerState.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Characters/GameplayAbilities/Effects/Countess_GameplayEffect_Base.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Regen.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_PlayerStats.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Primary.h"


ACountess_PlayerState::ACountess_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCountess_AbilitySystemComponent>(FName("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UCountess_AttributeSet_Base>(FName("AttributeSet"));

	StartupAbilities.Add(UCountess_GameplayAbility_Regen::StaticClass());
	StartupAbilities.Add(UCountess_GameplayAbility_Primary::StaticClass());
	PlayerLevel = 1;
}

void ACountess_PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ACountess_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	AttributeSet->Countess_Level_Changed_Delegate.AddDynamic(this, &ACountess_PlayerState::PlayerLevelIncreased);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ACountess_PlayerState::OnHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &ACountess_PlayerState::OnStaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &ACountess_PlayerState::OnManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetExpAttribute()).AddUObject(this, &ACountess_PlayerState::OnExpChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ACountess_PlayerState::OnMaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &ACountess_PlayerState::OnMaxStaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute()).AddUObject(this, &ACountess_PlayerState::OnMaxManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxExpAttribute()).AddUObject(this, &ACountess_PlayerState::OnMaxExpChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthRegenRateAttribute()).AddUObject(this, &ACountess_PlayerState::OnHealthRegenRateChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaRegenRateAttribute()).AddUObject(this, &ACountess_PlayerState::OnManaRegenRateChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaRegenRateAttribute()).AddUObject(this, &ACountess_PlayerState::OnStaminaRegenRateChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetArmorAttribute()).AddUObject(this, &ACountess_PlayerState::OnArmorChanged);

	GiveStartupAbilities();

	AbilitySystemComponent->AddLooseGameplayTag(CountessTags::FStatusTags::ExpNotFullTag);

}

bool ACountess_PlayerState::AcquireAbilitiy(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToAcquire)
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(Countess_Log, Error, TEXT("Ability System Component is nullptr from %s"), TEXT(__FUNCTION__));
		return false;
	}
	
	// Acquire only those Abilities that which are not already acquired.
	if (AcquiredAbilities.Contains(AbilityToAcquire))
		return false;

	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
	//We are giving ability to our ASC
	FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAcquire, GetPlayerLevel(), INDEX_NONE, this));
	//stating that the ownerActor of this ability is the this (PlayerState) and AvatarActor which physically acquires the ability is our PlayerCharacter (GetPawn())
	
	
	//Adding Acquired ability to our list
	AcquiredAbilities.Add(AbilityToAcquire);
	
	
	//FGameplayAbilityActorInfo* myinfo = AbilitySystemComponent->AbilityActorInfo.Get();

	//Process of getting the duration of the cooldown duration
	UCountess_GameplayAbility_Base* AbilityToAcquireCDO = Cast<UCountess_GameplayAbility_Base>(AbilityToAcquire->GetDefaultObject(false));
	float Duration = -1.f;
	UCountess_GameplayEffect_Base* GameplayEffectCDO = Cast<UCountess_GameplayEffect_Base>(AbilityToAcquireCDO->GetCooldownGameplayEffect());
	if (GameplayEffectCDO)
		GameplayEffectCDO->DurationMagnitude.GetStaticMagnitudeIfPossible(GetPlayerLevel(), Duration);
	
	//Below comment is how to get cooldown duration remaining when this ability is active 	
 	//AbilityToAcquireCDO->GetCooldownTimeRemainingAndDuration(AbilitySpecHandle, myinfo, Cd, Duration);


	//UE_LOG(Countess_Log, Warning, TEXT("Cooldown duration read on PS in %s is %f "), TEXT(__FUNCTION__), Duration);

	
	if(AbilityToAcquireCDO->AbilityData->IsValidLowLevel())
	{
		//Broadcasting our acquired ability details to listeners.
		// #TODO Combine all these details to decrease the number of params getting broadcasted. Turn them to a global struct maybe? 
		Countess_Ability_Acquired_Delegate.Broadcast(AbilityToAcquire, AbilityToAcquireCDO->AbilityData->AbilityIcon, Duration);
		//UE_LOG(Countess_Log, Warning, TEXT("Acquired Ability is %s and Description is %s"), *AbilityToAcquire->GetName(), *AbilityToAcquireCDO->AbilityData->Description.ToString());
	}
	else
	{
		UE_LOG(Countess_Log, Error, TEXT("Acquired Ability is %s and has no associated AbilityData with it."), *AbilityToAcquire->GetName());
	}
	
	return true;
}

bool ACountess_PlayerState::Countess_Interface_AcquireAbilitiy_Implementation(TSubclassOf<UCountess_GameplayAbility_Base>& AbilityToAcquire)
{
	return AcquireAbilitiy(AbilityToAcquire);
}

void ACountess_PlayerState::SetStartupAbilities(TSubclassOf<UCountess_GameplayAbility_Base>& StartupAbility)
{
	StartupAbilities.Add(StartupAbility);
}

void ACountess_PlayerState::GiveStartupAbilities()
{
	check(AbilitySystemComponent);

	if (!bAbilitiesInitialized)
	{
		for (auto& StartupAbility : StartupAbilities)
		{
			if (AcquireAbilitiy(StartupAbility))
			{
				UE_LOG(Countess_Log, Warning, TEXT("Activating Startup Ability From %s"), TEXT(__FUNCTION__));
				AbilitySystemComponent->TryActivateAbilityByClass(StartupAbility);
			}
		}

		//Apply Passive Effects too.
		FGameplayEffectContextHandle GameplayEffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		GameplayEffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(UCountess_GE_PlayerStats::StaticClass(), GetPlayerLevel(), GameplayEffectContextHandle);
		if (GameplayEffectSpecHandle.IsValid())
		{
			//FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), AbilitySystemComponent);
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
		}
		bAbilitiesInitialized = true;
	}
}

void ACountess_PlayerState::RemoveStartupAbilities()
{
	check(AbilitySystemComponent);

	if (bAbilitiesInitialized)
	{
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if (Spec.SourceObject == this && StartupAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
				AcquiredAbilities.RemoveSingle(Spec.Ability->GetClass());
			}
		}

		// Do in two passes so the removal happens after we have the full list
		for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
		{
			AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
		}
		// Remove all of the passive gameplay effects that were applied by this character
		//This is handled now by the ability that is getting removed.
// 		FGameplayTagContainer TagContainer;
// 		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Stamina.Regen")));
// 		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Health.Regen")));
// 		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(TagContainer);
// 		AbilitySystemComponent->RemoveActiveEffects(Query);
 		bAbilitiesInitialized = false;
	}
}

void ACountess_PlayerState::RefreshStartupAbilities()
{
	RemoveStartupAbilities();
	GiveStartupAbilities();
}


void ACountess_PlayerState::PlayerLevelIncreased(int32 NewLevel)
{
	if (NewLevel == AttributeSet->GetPlayerMaxLevel())
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(CountessTags::FStatusTags::ExpNotFullTag);
	}
	else
	{
		RemoveStartupAbilities();
		PlayerLevel = NewLevel;
		GiveStartupAbilities();
	}
}

UAbilitySystemComponent* ACountess_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGameplayAbility* ACountess_PlayerState::GetAbilityObjectFromClass(TSubclassOf<UGameplayAbility>& Ability)
{
	return Cast<UGameplayAbility>(Ability->GetDefaultObject(false));
}

bool ACountess_PlayerState::CanDoubleJump() const
{
	FGameplayTagContainer DoubleJumpContainer;

	DoubleJumpContainer.AddTag(CountessTags::FStatusTags::JumpingTag);
	
	return AbilitySystemComponent->HasAllMatchingGameplayTags(DoubleJumpContainer);
}

bool ACountess_PlayerState::IsAlive() const
{
	return AttributeSet->GetHealth() > 0.f;
}

bool ACountess_PlayerState::CanJump(TSubclassOf<UGameplayAbility>& OUTJumpAbility) const
{
	TSubclassOf<UGameplayAbility> JumpAbility, DoubleJumpAbility;

	for (auto Ability : AcquiredAbilities)
	{
		if (Ability.GetDefaultObject()->AbilityTags.HasTagExact(CountessTags::FAbilityTags::JumpAbilityTag))
		{
			JumpAbility = Ability;
			OUTJumpAbility = JumpAbility;
		}
		if (Ability.GetDefaultObject()->AbilityTags.HasTagExact(CountessTags::FAbilityTags::DoubleJumpAbilityTag))
		{
			DoubleJumpAbility = Ability;
		}
	}
	if (JumpAbility.GetDefaultObject()->IsValidLowLevel())
	{
		if (DoubleJumpAbility.GetDefaultObject()->IsValidLowLevel())
		{
			if (CanDoubleJump())
			{
				UE_LOG(Countess_Log, Warning, TEXT("Player Can Double Jump now. from %s"), TEXT(__FUNCTION__));
				AbilitySystemComponent->CancelAbility(JumpAbility.GetDefaultObject());
				OUTJumpAbility = DoubleJumpAbility;
			}
			return true;
		}
		OUTJumpAbility = JumpAbility;
		return true;
	}
	return false;
}

bool ACountess_PlayerState::CanBackDash(TSubclassOf<UGameplayAbility>& OUTBackDashAbility) const
{
	for (auto Ability : AcquiredAbilities)
	{
		if (Ability.GetDefaultObject()->AbilityTags.HasTagExact(CountessTags::FAbilityTags::BackDashAbilityTag))
		{
			OUTBackDashAbility = Ability;
			return true;
		}
	}
	return false;
}


bool ACountess_PlayerState::CanFireball(TSubclassOf<UGameplayAbility>& OUTBackDashAbility) const
{
	for (auto Ability : AcquiredAbilities)
	{
		if (Ability.GetDefaultObject()->AbilityTags.HasTagExact(CountessTags::FAbilityTags::FireballAbilityTag))
		{
			OUTBackDashAbility = Ability;
			return true;
		}
	}
	return false;
}


bool ACountess_PlayerState::CanESpark(TSubclassOf<UGameplayAbility>& OUTBackDashAbility) const
{
	for (auto Ability : AcquiredAbilities)
	{
		if (Ability.GetDefaultObject()->AbilityTags.HasTagExact(CountessTags::FAbilityTags::ElectroSparkAbilityTag))
		{
			OUTBackDashAbility = Ability;
			return true;
		}
	}
	return false;
}

bool ACountess_PlayerState::CanPrimary(TSubclassOf<UGameplayAbility>& OUTPrimaryAbility) const
{
	for (auto Ability : AcquiredAbilities)
	{
		if (Ability.GetDefaultObject()->AbilityTags.HasTagExact(CountessTags::FAbilityTags::PrimaryAbilityTag))
		{
			OUTPrimaryAbility = Ability;
			return true;
		}
	}
	return false;
}

int32 ACountess_PlayerState::GetPlayerLevel() const
{
	return PlayerLevel;
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

float ACountess_PlayerState::GetCurrentExp() const
{
	return AttributeSet->GetExp();
}

float ACountess_PlayerState::GetMaxExp() const
{
	return AttributeSet->GetMaxExp();
}

bool ACountess_PlayerState::Countess_Interface_TryActivateAbilityByClass_Implementation(TSubclassOf<UGameplayAbility>& AbilityToActivate)
{
	return AbilitySystemComponent->TryActivateAbilityByClass(AbilityToActivate);
}

void ACountess_PlayerState::Countess_Interface_CancelAbility_Implementation(TSubclassOf<UGameplayAbility>& AbilityToCancel)
{
	AbilitySystemComponent->CancelAbility(GetAbilityObjectFromClass(AbilityToCancel));
}

FCountessLevelChangedDelegate& ACountess_PlayerState::GetCountessLevelChangedDelegate()
{
	return AttributeSet->Countess_Level_Changed_Delegate;
}

void ACountess_PlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	Countess_Health_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	Countess_Stamina_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnManaChanged(const FOnAttributeChangeData& Data)
{
	Countess_Mana_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	Countess_MaxHealth_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnMaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	Countess_MaxStamina_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnMaxManaChanged(const FOnAttributeChangeData& Data)
{
	Countess_MaxMana_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnHealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
	Countess_HealthRegenRate_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnManaRegenRateChanged(const FOnAttributeChangeData& Data)
{
	Countess_ManaRegenRate_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnStaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{
	Countess_StaminaRegenRate_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnArmorChanged(const FOnAttributeChangeData& Data)
{
	Countess_Armor_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnExpChanged(const FOnAttributeChangeData& Data)
{
	Countess_Exp_Changed_Delegate.Broadcast(Data.NewValue);
}

void ACountess_PlayerState::OnMaxExpChanged(const FOnAttributeChangeData& Data)
{
	Countess_MaxExp_Changed_Delegate.Broadcast(Data.NewValue);
}
