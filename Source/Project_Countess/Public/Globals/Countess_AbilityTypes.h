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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessAbilityAcquired_Interface_Delegate, TSubclassOf<class UCountess_GameplayAbility_Base>, AcquiredGameplayAbilityClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessLevelChangedDelegate, int32, PlayerLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCountessAbilityAcquiredDelegate, TSubclassOf<class UCountess_GameplayAbility_Base>, AcquiredGameplayAbilityClass, FSlateBrush, AbilityIcon, float, Cooldown); //Add float Cooldown, Ability Type (White Magic/Black Magic) etc..

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
		inline static const FGameplayTag LensOfTruthAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Lens"));
		inline static const FGameplayTag MistAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Mist"));
		inline static const FGameplayTag TimeSlowAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.TimeSlow"));
		inline static const FGameplayTag ShieldAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Shield"));
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
		inline static const FGameplayTag LensAbilityOnTag = FGameplayTag::RequestGameplayTag(FName("State.Lens.On"));
		inline static const FGameplayTag MistAbilityOnTag = FGameplayTag::RequestGameplayTag(FName("State.Mist.On"));
		inline static const FGameplayTag TimeSlowAbilityOnTag = FGameplayTag::RequestGameplayTag(FName("State.TimeSlow.On"));
		inline static const FGameplayTag ShieldAbilityOnTag = FGameplayTag::RequestGameplayTag(FName("State.Shield.On"));

	};

	struct FCooldownTags
	{   
		inline static const FGameplayTag PrimaryAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.Primary.Cooldown"));
		inline static const FGameplayTag BackDashAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.BDash.Cooldown"));
		inline static const FGameplayTag JumpAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump.Cooldown"));
		inline static const FGameplayTag DoubleJumpAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.DJump.Cooldown"));
		inline static const FGameplayTag FireballAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.Fireball.Cooldown"));
		inline static const FGameplayTag ElectroSparkAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.ESpark.Cooldown"));
		inline static const FGameplayTag LensAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.Lens.Cooldown"));
		inline static const FGameplayTag MistAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.Mist.Cooldown"));
		inline static const FGameplayTag TimeSlowAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.TimeSlow.Cooldown"));
		inline static const FGameplayTag ShieldAbilityCooldownTag = FGameplayTag::RequestGameplayTag(FName("Ability.Shield.Cooldown"));
	};  

	struct FCostTags
	{
		inline static const FGameplayTag BackDashAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.BDash.Cost"));
		inline static const FGameplayTag JumpAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.Jump.Cost"));
		inline static const FGameplayTag DoubleJumpAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.DJump.Cost"));
		inline static const FGameplayTag FireballAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.Fireball.Cost"));
		inline static const FGameplayTag ElectroSparkAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.ESpark.Cost"));
		inline static const FGameplayTag LensAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.Lens.Cost"));
		inline static const FGameplayTag MistAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.Mist.Cost"));
		inline static const FGameplayTag TimeSlowAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.TimeSlow.Cost"));
		inline static const FGameplayTag ShieldAbilityCostTag = FGameplayTag::RequestGameplayTag(FName("Ability.Shield.Cost"));
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

	inline static const TMap<E_WMagic, FGameplayTag> WMagicTag = {
		{E_WMagic::None, InvalidTag},
		{E_WMagic::LensOfTruth, FAbilityTags::LensOfTruthAbilityTag},
		{E_WMagic::Mist, FAbilityTags::MistAbilityTag},
		{E_WMagic::TimeSlow, FAbilityTags::TimeSlowAbilityTag},
		{E_WMagic::Shield, FAbilityTags::ShieldAbilityTag}
	};

	inline static const TMap<E_Skill, FGameplayTag> SkillTag = {
		{E_Skill::None, InvalidTag},
		{E_Skill::Jump, FAbilityTags::JumpAbilityTag},
		{E_Skill::DoubleJump, FAbilityTags::DoubleJumpAbilityTag},
		{E_Skill::BackDash, FAbilityTags::BackDashAbilityTag}
	};

	inline static const TArray<FGameplayTag> AbilityTagsArray = { 
		FAbilityTags::BackDashAbilityTag ,
		FAbilityTags::DoubleJumpAbilityTag,
		FAbilityTags::ElectroSparkAbilityTag,
		FAbilityTags::FireballAbilityTag,
		FAbilityTags::JumpAbilityTag,
		FAbilityTags::PrimaryAbilityTag,
		FAbilityTags::RegenAbilityTag,
		FAbilityTags::LensOfTruthAbilityTag,
		FAbilityTags::MistAbilityTag,
		FAbilityTags::TimeSlowAbilityTag,
		FAbilityTags::ShieldAbilityTag
	};
	
	inline static const TArray<FGameplayTag> StatusTagsArray = {
		FStatusTags::DeadTag ,
		FStatusTags::ExpNotFullTag,
		FStatusTags::HealthNotFullTag,
		FStatusTags::JumpingTag,
		FStatusTags::ManaNotFullTag,
		FStatusTags::StaminaNotFullTag,
		FStatusTags::StunTag,
		FStatusTags::LensAbilityOnTag,
		FStatusTags::MistAbilityOnTag,
		FStatusTags::TimeSlowAbilityOnTag,
		FStatusTags::ShieldAbilityOnTag
	};

	inline static const TArray<FGameplayTag> CooldownTagsArray = {
		FCooldownTags::BackDashAbilityCooldownTag,
		FCooldownTags::DoubleJumpAbilityCooldownTag,
		FCooldownTags::ElectroSparkAbilityCooldownTag,
		FCooldownTags::FireballAbilityCooldownTag,
		FCooldownTags::JumpAbilityCooldownTag,
		FCooldownTags::PrimaryAbilityCooldownTag,
		FCooldownTags::LensAbilityCooldownTag,
		FCooldownTags::MistAbilityCooldownTag,
		FCooldownTags::TimeSlowAbilityCooldownTag,
		FCooldownTags::ShieldAbilityCooldownTag
	};

	inline static const TArray<FGameplayTag> CostTagsArray = {
		FCostTags::BackDashAbilityCostTag,
		FCostTags::DoubleJumpAbilityCostTag,
		FCostTags::ElectroSparkAbilityCostTag,
		FCostTags::FireballAbilityCostTag,
		FCostTags::JumpAbilityCostTag,
		FCostTags::LensAbilityCostTag,
		FCostTags::MistAbilityCostTag,
		FCostTags::TimeSlowAbilityCostTag,
		FCostTags::ShieldAbilityCostTag
	};

	inline static const TArray<FGameplayTag> EffectTagsArray = {
		FEffectTags::HealthRegenTag,
		FEffectTags::ManaRegenTag,
		FEffectTags::StaminaRegenTag
	};

	inline static const TArray<FGameplayTag> EventTagsArray = {
		FEventTags::ElectroSparkCastEventTag,
		FEventTags::FireballCastEventTag
	};

	inline static const TArray<FGameplayTag> DamageTagsArray = {
		FDamageTags::ElectroSparkAbilityDamageTag,
		FDamageTags::FireballAbilityDamageTag,
		FDamageTags::PrimaryAbilityDamageTag
	};
}
