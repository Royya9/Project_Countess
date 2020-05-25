// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Abilities/GameplayAbility.h"
#include "Countess_GameplayAbility_Base.generated.h"

class USoundWave;
class UParticleSystem;
class UAbilityData;

/**
 * 
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

	/*particle emitter to spawn when ability is activated*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = VFX, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UParticleSystem> EmitterToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AbilityDetails, meta = (DisplayName = "Ability Data"))
	TWeakObjectPtr<UAbilityData> AbilityData;
};

/*Container for our Ability Details. All Countess_GA_Base subclasses must have this*/
UCLASS(BlueprintType, Blueprintable)
class PROJECT_COUNTESS_API UAbilityData : public UObject
{
	GENERATED_BODY()
public:

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Ability Border Color"))
	FSlateBrush AbilityBorderColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "Ability Font Color"))
	FSlateColor AbilityFontColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityDetails | UI", meta = (DisplayName = "IsMagicAbility"))
	bool bIsMagicAbility;

};