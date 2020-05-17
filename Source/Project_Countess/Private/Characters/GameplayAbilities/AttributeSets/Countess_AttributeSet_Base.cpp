// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Engine/DataTable.h"


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
	Super::PreAttributeChange(Attribute,NewValue);
}

void UCountess_AttributeSet_Base::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UCountess_AttributeSet_Base::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{

}
