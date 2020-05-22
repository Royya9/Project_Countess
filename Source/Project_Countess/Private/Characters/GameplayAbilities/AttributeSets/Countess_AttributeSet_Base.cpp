// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Engine/CurveTable.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"


UCountess_AttributeSet_Base::UCountess_AttributeSet_Base()
	:Health(20.f),
	MaxHealth(20.f),
	HealthRegenRate(1.f),
	Mana(40.f),
	MaxMana(40.f),
	ManaRegenRate(2.f),
	Stamina(10.f),
	MaxStamina(10.f),
	StaminaRegenRate(1.f),
	Armor(5.f),
	Exp(0.f),
	MaxExp(100.f),
	CurrentPlayerLevel(1),
	MAX_PLAYER_LEVEL(1),
	bMaxPlayerLevelReached(false)
{
	
	static ConstructorHelpers::FObjectFinder<UCurveTable> _AbilityDetailsObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails'"));
	if (_AbilityDetailsObject.Succeeded())
	{
		AbiiltyDetailsTable = _AbilityDetailsObject.Object;
	}
	else
	{
		UE_LOG(Countess_Log, Error, TEXT("Cannot find AbilityDetails curve table in location : /Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails. Check if it is moved. From %s"), TEXT(__FUNCTION__));
	}
	if (AbiiltyDetailsTable)
	{
		//Create Attributes at Level 1 from our AbilityDetails Table
		SetAttributes();
	}
}


void UCountess_AttributeSet_Base::SetAttributes()
{
	if (AbiiltyDetailsTable)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found Data Table %s"), *StartupData->GetName());
		FString ContextString;
		FSimpleCurve* HealthHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("Health"), ContextString);
		FSimpleCurve* MaxHealthHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("MaxHealth"), ContextString);
		FSimpleCurve* ManaHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("Mana"), ContextString);
		FSimpleCurve* MaxManaHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("MaxMana"), ContextString);
		FSimpleCurve* StaminaHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("Stamina"), ContextString);
		FSimpleCurve* MaxStaminaHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("MaxStamina"), ContextString);
		FSimpleCurve* HealthRegenHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("HealthRegenRate"), ContextString);
		FSimpleCurve* StaminaRegenHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("StaminaRegenRate"), ContextString);
		FSimpleCurve* ManaRegenHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("ManaRegenRate"), ContextString);
		FSimpleCurve* ArmorHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("Armor"), ContextString);
		FSimpleCurve* ExpHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("Exp"), ContextString);
		FSimpleCurve* MaxExpHandle = AbiiltyDetailsTable->FindSimpleCurve(FName("MaxExp"), ContextString);

		if (HealthHandle)
			Health = HealthHandle->Eval(1.f);
		if (MaxHealthHandle)
			MaxHealth = MaxHealthHandle->Eval(1.f);
		if (ManaHandle)
			Mana = ManaHandle->Eval(1.f);
		if (MaxManaHandle)
			MaxMana = MaxManaHandle->Eval(1.f);
		if (StaminaHandle)
			Stamina = StaminaHandle->Eval(1.f);
		if (MaxStaminaHandle)
			MaxStamina = MaxStaminaHandle->Eval(1.f);
		if (HealthRegenHandle)
			HealthRegenRate = HealthRegenHandle->Eval(1.f);
		if (StaminaRegenHandle)
			StaminaRegenRate = StaminaRegenHandle->Eval(1.f);
		if (ManaRegenHandle)
			ManaRegenRate = ManaRegenHandle->Eval(1.f);
		if (ArmorHandle)
			Armor = ArmorHandle->Eval(1.f);
		if (ExpHandle)
		{
			Exp = ExpHandle->Eval(1.f);
			MAX_PLAYER_LEVEL = ExpHandle->GetNumKeys();
		}
		if (MaxExpHandle)
			MaxExp = MaxExpHandle->Eval(1.f);
	}
}

void UCountess_AttributeSet_Base::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	
	//UE_LOG(Countess_Log, Warning, TEXT("Attribute %s going to be changed. From %s\n Attribute value is %f"), *Attribute.GetName(), TEXT(__FUNCTION__), NewValue);
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
	else if (Attribute == GetExpAttribute() && !bMaxPlayerLevelReached)
	{
			
		//UE_LOG(Countess_Log, Warning, TEXT("Gaining Exp: %f. From %s"), NewValue, TEXT(__FUNCTION__));
		if(NewValue > (MaxExp.GetCurrentValue() ))
		{
			CurrentPlayerLevel++;
			if (CurrentPlayerLevel == MAX_PLAYER_LEVEL+1)
			{
				//UE_LOG(Countess_Log, Warning, TEXT("Max Player Level reached. Max Exp value is %f. From %s"), GetMaxExp(), TEXT(__FUNCTION__));
				bMaxPlayerLevelReached = true;

			}
			else
			{
				NewValue -= MaxExp.GetCurrentValue();
				// We broadcast that player level changed first instead of SetExp(NewValue) to handle the cases where Player level increases by more than 1 when he gains too much exp.
				Countess_Level_Changed_Delegate.Broadcast();

				SetExp(NewValue);
			}

		}
	}

}

void UCountess_AttributeSet_Base::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Countess_Context = Data.EffectSpec.GetContext();
	
	UAbilitySystemComponent* AbilitySystemComponent = Countess_Context.GetOriginalInstigatorAbilitySystemComponent();
	
	// Apply FullStamina Tag to our AbilitySystemComponent to prevent Regen Effect being applied. Remove the tag if not so.
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{		
		UE_LOG(Countess_Log, Warning, TEXT("stamina value is %f and Stamina_Regen_Rate is %f. From %s"), GetStamina(), GetStaminaRegenRate(), TEXT(__FUNCTION__));
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
		if ( GetStamina() > GetMaxStamina() ||  FMath::IsNearlyEqual(GetStamina(), GetMaxStamina()))
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stamina.NotFull")));
		}
		else
		{
			AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stamina.NotFull")));
		}
	}
	
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		if (FMath::IsNearlyEqual(GetHealth(), GetMaxHealth()))
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Health.NotFull")));
		}
		else
		{
			AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Health.NotFull")));
		}
	}
	
	else if (Data.EvaluatedData.Attribute == GetMaxExpAttribute())
	{
		if (!AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Exp.NotFull"))))
		{
			UE_LOG(Countess_Log, Warning, TEXT("Max Player Level Reached?? From %s"), TEXT(__FUNCTION__));
		}
	}
	else if (Data.EvaluatedData.Attribute == GetExpAttribute())
	{
		if (bMaxPlayerLevelReached)
		{
			SetExp(FMath::Clamp(GetExp(), 0.f, GetMaxExp()));
		}
	}

}

void UCountess_AttributeSet_Base::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

