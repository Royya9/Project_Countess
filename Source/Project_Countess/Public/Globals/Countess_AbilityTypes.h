// Free to play. By SoNa Games.

#pragma once
#include "GameplayTagContainer.h"
#include "Countess_AbilityTypes.generated.h"

UENUM(BlueprintType)
enum class E_BMagic : uint8
{
	None			UMETA(DisplayName = "None"),
	FireBall		UMETA(DisplayName = "FireBall"),
	ElectroSpark	UMETA(DisplayName = "ElectroSpark"),
	BloodLust		UMETA(DisplayName = "BloodLust"),
	ArcticBlast		UMETA(DisplayName = "ArcticBlast")
};

UENUM(BlueprintType)
enum class E_WMagic : uint8
{
	None			UMETA(DisplayName = "None"),
	LensOfTruth		UMETA(DisplayName = "LensOfTruth"),
	Mist			UMETA(DisplayName = "Mist"),
	Shield			UMETA(DisplayName = "Shield"),
	TimeSlow		UMETA(DisplayName = "TimeSlow")
};

UENUM(BlueprintType)
enum class E_Skill : uint8
{
	None			UMETA(DisplayName = "None"),
	Jump			UMETA(DisplayName = "Jump"),
	DoubleJump		UMETA(DisplayName = "DoubleJump"),
	BackDash		UMETA(DisplayName = "BackDash"),
};


/*Delegates*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessAbilityAcquired_Interface_Delegate, TSubclassOf<UCountess_GameplayAbility_Base>, AcquiredGameplayAbilityClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessLevelChangedDelegate, int32, PlayerLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCountessAbilityAcquiredDelegate, TSubclassOf<UCountess_GameplayAbility_Base>, AcquiredGameplayAbilityClass, FSlateBrush, AbilityIcon, float, Cooldown); //Add float Cooldown, Ability Type (White Magic/Black Magic) etc..

namespace CountessTags
{

	inline static const FGameplayTag InvalidTag = FGameplayTag::RequestGameplayTag(FName("Invalid"));

	struct FAbilityTags
	{
		inline static const FGameplayTag PrimaryAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Primary"));
		inline static const FGameplayTag RegenAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Regen"));
		inline static const FGameplayTag JumpAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump"));
		inline static const FGameplayTag DoubleJumpAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.DJump"));
		inline static const FGameplayTag BackDashAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.BDash"));
		inline static const FGameplayTag FireballAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Fireball"));
		inline static const FGameplayTag ElectroSparkAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.ESpark"));
		
	};

	struct FStatusTags
	{
		inline static const FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
		inline static const FGameplayTag StunTag = FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"));
		inline static const FGameplayTag JumpingTag = FGameplayTag::RequestGameplayTag(FName("State.Jumping"));
		inline static const FGameplayTag ExpNotFullTag = FGameplayTag::RequestGameplayTag(FName("State.Exp.NotFull"));
		inline static const FGameplayTag HealthNotFullTag = FGameplayTag::RequestGameplayTag(FName("State.Health.NotFull"));
		inline static const FGameplayTag ManaNotFullTag = FGameplayTag::RequestGameplayTag(FName("State.Mana.NotFull"));
		inline static const FGameplayTag StaminaNotFullTag = FGameplayTag::RequestGameplayTag(FName("State.Stamina.NotFull"));
	};

	struct FCooldownTags
	{   
		inline static const FGameplayTag PrimaryAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.Primary.Cooldown"));
		inline static const FGameplayTag BackDashAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.BDash.Cooldown"));
		inline static const FGameplayTag JumpAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump.Cooldown"));
		inline static const FGameplayTag DoubleJumpAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.DJump.Cooldown"));
		inline static const FGameplayTag FireballAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.Fireball.Cooldown"));
		inline static const FGameplayTag ElectroSparkAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.ESpark.Cooldown"));
	};  

	struct FCostTags
	{
		inline static const FGameplayTag BackDashAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.BDash.Cost"));
		inline static const FGameplayTag JumpAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump.Cost"));
		inline static const FGameplayTag DoubleJumpAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.DJump.Cost"));
		inline static const FGameplayTag FireballAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.Fireball.Cost"));
		inline static const FGameplayTag ElectroSparkAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.ESpark.Cost"));
	};

	struct FEffectTags
	{
		inline static const FGameplayTag HealthRegenTag = FGameplayTag::RequestGameplayTag(FName("Effect.Health.Regen"));
		inline static const FGameplayTag ManaRegenTag = FGameplayTag::RequestGameplayTag(FName("Effect.Mana.Regen"));
		inline static const FGameplayTag StaminaRegenTag = FGameplayTag::RequestGameplayTag(FName("Effect.Stamina.Regen"));
	};

	struct FEventTags
	{
		inline static const FGameplayTag FireballCastEventTag = FGameplayTag::RequestGameplayTag(FName("Event.Ability.Fireball.Cast"));
		inline static const FGameplayTag ElectroSparkCastEventTag = FGameplayTag::RequestGameplayTag(FName("Event.Ability.ESpark.Cast"));
	};

	struct FDamageTags
	{
		inline static const FGameplayTag FireballAbilityDamageTag = FGameplayTag::RequestGameplayTag(FName("Ability.Fireball.Damage"));
		inline static const FGameplayTag ElectroSparkAbilityDamageTag = FGameplayTag::RequestGameplayTag(FName("Ability.ESpark.Damage"));
		inline static const FGameplayTag PrimaryAbilityDamageTag = FGameplayTag::RequestGameplayTag(FName("Ability.Primary.Damage"));
	};

	inline static const TMap<E_BMagic, FGameplayTag> BMagicTag = {
		{E_BMagic::None, InvalidTag},
		{E_BMagic::FireBall, FAbilityTags::FireballAbilityTag},
		{E_BMagic::ElectroSpark, FAbilityTags::ElectroSparkAbilityTag}
	};

	inline static const TMap<E_Skill, FGameplayTag> SkillTag = {
		{E_Skill::None, InvalidTag},
		{E_Skill::Jump, FAbilityTags::JumpAbilityTag},
		{E_Skill::DoubleJump, FAbilityTags::DoubleJumpAbilityTag},
		{E_Skill::BackDash, FAbilityTags::BackDashAbilityTag}
	};
}
