// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/CurveTable.h"
#include "Countess_GameplayAbility_Base.generated.h"

class USoundWave;
class UParticleSystem;
class UAbilityData;
class USoundCue;
class UCurveTable;

/**
 *  Base class for all our Abilities. Holds important details for each ability such as title, description, icon, cost etc.
 */
UCLASS(Abstract)
class PROJECT_COUNTESS_API UCountess_GameplayAbility_Base : public UGameplayAbility
{
	
	GENERATED_BODY()

public:

	UCountess_GameplayAbility_Base();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	//virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	//virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	/*sound to play when ability is activated*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = VFX, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<USoundWave> SoundToPlay;

	/*sound cue to play when ability is activated*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = VFX, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<USoundCue> SoundCueToPlay;

	/*particle emitter to spawn when ability is activated*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = VFX, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UParticleSystem> EmitterToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityDetails, meta = (DisplayName = "Ability Data"))
	TWeakObjectPtr<UAbilityData> AbilityData;
};

UENUM(BlueprintType)
enum class EAbilityCostType : uint8
{
	None  		UMETA(DisplayName = "None"),
    Mana		UMETA(DisplayName = "Mana"),
    Stamina		UMETA(DisplayName = "Stamina"),
    Health		UMETA(DisplayName = "Health")
};

/*Container for our Ability Details. All Countess_GA_Base subclasses must have this*/
UCLASS(BlueprintType, Blueprintable)
class PROJECT_COUNTESS_API UAbilityData : public UObject
{
	GENERATED_BODY()
public:

	inline static const TMap<EAbilityCostType, FName> AbilityCostString = {
		{EAbilityCostType::Mana, FName("Mana Cost")},
		{EAbilityCostType::Stamina, FName("Stamina Cost")},
		{EAbilityCostType::Health, FName("Health Cost")},
		{EAbilityCostType::None, FName("None Cost")}
	};

	/* UI Related Data*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Title"))
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Description"))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Interactive Message"))
	FText InteractiveMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Input Button"))
	FSlateBrush InputButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Ability Icon"))
	FSlateBrush AbilityIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Ability Image"))
	FSlateBrush AbilityImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Ability Menu Image"))
	FSlateBrush AbilityMenuImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Ability Border Color"))
	FSlateBrush AbilityBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Ability Font Color"))
	FSlateColor AbilityFontColor;

	/* Data Related to Ability such as Cost, Damage, Cooldown, RegenRate etc*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | Data", meta = (DisplayName = "Is Magic Ability"))
	bool bIsMagicAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | Data", meta = (DisplayName = "Ability Cost Data Table"))
	FCurveTableRowHandle CostRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | Data", meta = (DisplayName = "Ability Cost Type"))
	EAbilityCostType AbilityCostType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | Data", meta = (DisplayName = "Ability Cooldown Data Table"))
	FCurveTableRowHandle CoolDownRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | Data", meta = (DisplayName = "Ability Damage Data Table"))
	FCurveTableRowHandle DamageRowHandle;
};