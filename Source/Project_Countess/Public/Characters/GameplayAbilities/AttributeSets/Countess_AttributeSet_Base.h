// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Countess_AttributeSet_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCountessLevelChangedDelegate);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_AttributeSet_Base : public UAttributeSet
{
	GENERATED_BODY()
	
	UCurveTable* AbiiltyDetailsTable;
public:

	UCountess_AttributeSet_Base();

	virtual void SetAttributes();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, HealthRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData ManaRegenRate;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, ManaRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, StaminaRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Armor")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, Armor)

	//Temporary Attribute to calculate health
	UPROPERTY(BlueprintReadOnly, Category = "Damage", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Exp")
	FGameplayAttributeData Exp;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, Exp)

	UPROPERTY(BlueprintReadOnly, Category = "MaxExp")
	FGameplayAttributeData MaxExp;
	ATTRIBUTE_ACCESSORS(UCountess_AttributeSet_Base, MaxExp)

	/*Delegates to broadcast that our Player Experience Crossed Max Experience for that level ie., his level is increased.*/
	FCountessLevelChangedDelegate Countess_Level_Changed_Delegate;

protected:
	

	//Function to calculate proportional attribute value change due to its max value change
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

private:

	int32 CurrentPlayerLevel;
	int32 MAX_PLAYER_LEVEL;
	bool bMaxPlayerLevelReached;
};
