// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Engine/DataTable.h"
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
	Armor(5.f)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> AttributeData(TEXT("/Game/MyProjectMain/Data/StartupData"));
	if(AttributeData.Succeeded())
		StartupData = AttributeData.Object;
	if (StartupData)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found Data Table %s"), *StartupData->GetName());
		
		FString ContextString;
		FAttributeMetaData* MaxHealthRow = StartupData->FindRow<FAttributeMetaData>(FName(TEXT("MaxHealth")), ContextString);
		FAttributeMetaData* CurHealthRow = StartupData->FindRow<FAttributeMetaData>(FName(TEXT("CurrentHealth")), ContextString);
		FAttributeMetaData* MaxManaRow = StartupData->FindRow<FAttributeMetaData>(FName(TEXT("MaxMana")), ContextString);
		FAttributeMetaData* CurManaRow = StartupData->FindRow<FAttributeMetaData>(FName(TEXT("CurrentMana")), ContextString);
		FAttributeMetaData* HealthRegenRow = StartupData->FindRow<FAttributeMetaData>(FName(TEXT("HealthRegenRate")), ContextString);
		FAttributeMetaData* ManaRegenRow = StartupData->FindRow<FAttributeMetaData>(FName(TEXT("ManaRegenRate")), ContextString);
		FAttributeMetaData* ArmorRow = StartupData->FindRow<FAttributeMetaData>(FName(TEXT("Armor")), ContextString);
		if (MaxHealthRow)
			MaxHealth = MaxHealthRow->BaseValue;
		if (CurHealthRow)
			Health = CurHealthRow->BaseValue;
		if (MaxManaRow)
			MaxMana = MaxManaRow->BaseValue;
		if (CurManaRow)
			Mana = CurManaRow->BaseValue;
		if (HealthRegenRow)
			HealthRegenRate = HealthRegenRow->BaseValue;
		if (ManaRegenRow)
			ManaRegenRate = ManaRegenRow->BaseValue;
		if (ArmorRow)
			Armor = ArmorRow->BaseValue;
			
	}
}

void UCountess_AttributeSet_Base::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	
	//UE_LOG(Countess_Log, Warning, TEXT("Attribute %s going to be changed. From %s\n Attribute value is %f"), *Attribute.GetName(), TEXT(__FUNCTION__), NewValue);
	

	
}

void UCountess_AttributeSet_Base::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Countess_Context = Data.EffectSpec.GetContext();
	
	UAbilitySystemComponent* AbilitySystemComponent = Countess_Context.GetOriginalInstigatorAbilitySystemComponent();

	

	
	// Apply FullStamina Tag to our AbilitySystemComponent to prevent Regen Effect being applied. Remove the tag if not so.
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
// 		float val = Data.EvaluatedData.Magnitude;
// 		FGameplayModifierEvaluatedData Countess_Data = Data.EvaluatedData;
// 		UE_LOG(Countess_Log, Warning, TEXT("%f"), Countess_Data.Magnitude);

		

		
		//UE_LOG(Countess_Log, Warning, TEXT("stamina value is %f. From %s"), GetStamina(), TEXT(__FUNCTION__));
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
		if ( GetStamina() > GetMaxStamina() ||  FMath::IsNearlyEqual(GetStamina(), GetMaxStamina()))
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stamina.NotFull")));
// 			if (!AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Buff.FullStamina"))))
// 			{
// 				AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Buff.FullStamina")));
// 				UE_LOG(Countess_Log, Warning, TEXT("Add Full Stamina Tag in %s"), TEXT(__FUNCTION__));
// 			}
		}
		else
		{
			AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Stamina.NotFull")));
// 			if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Buff.FullStamina"))))
// 			{
// 				AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Buff.FullStamina")));
// 				UE_LOG(Countess_Log, Warning, TEXT("Removed Full Stamina Tag in %s"), TEXT(__FUNCTION__));
// 			}
		}
		
	}
	
	//Handle Stamina Change
	/*
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		//FGameplayEffectSpec Countess_Spec = Data.;
		FGameplayEffectContextHandle Countess_Context = Data.EffectSpec.GetContext();
		//Countess_Context.
		FGameplayAttribute Attribute = Data.EvaluatedData.Attribute;
		float NewValue = Data.EvaluatedData.Attribute.GetNumericValue(this);
		UE_LOG(Countess_Log, Warning, TEXT("Attribute %s going to be changed. From %s\n Attribute value is %f"), *Attribute.GetName(), TEXT(__FUNCTION__), NewValue);
		CountessAttributeChangedDelegate.Broadcast(Attribute);
	}
	*/

}

void UCountess_AttributeSet_Base::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{

}
