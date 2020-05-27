// Free to play. By SoNa Games.

#pragma once

#include "Engine/CurveTable.h"
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
enum class E_Skills : uint8
{
	None			UMETA(DisplayName = "None"),
	DoubleJump		UMETA(DisplayName = "DoubleJump"),
	BackDash		UMETA(DisplayName = "BackDash"),
	FireBall		UMETA(DisplayName = "FireBall"),
	ElectroSpark	UMETA(DisplayName = "ElectroSpark"),
	LensOfTruth		UMETA(DisplayName = "LensOfTruth"),
	Mist			UMETA(DisplayName = "Mist")
};


/*Delegates*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessAbilityAcquired_Interface_Delegate, TSubclassOf<UCountess_GameplayAbility_Base>, AcquiredGameplayAbilityClass);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessAttributeChangedDelegate, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessLevelChangedDelegate, int32, PlayerLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCountessAbilityAcquiredDelegate, TSubclassOf<UCountess_GameplayAbility_Base>, AcquiredGameplayAbilityClass, FSlateBrush, AbilityIcon, float, Cooldown); //Add float Cooldown, Ability Type (White Magic/Black Magic) etc..

/*
USTRUCT(BlueprintType)
struct FAbilityDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MasterData", meta = (DisplayName = "Cool Down"))
	float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MasterData", meta = (DisplayName = "Mana Cost"))
	float ManaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MasterData", meta = (DisplayName = "Stamina Cost"))
	float StaminaCost;
};
*/

/*Holds the Data required all classes*/
/*
USTRUCT(BlueprintType, Blueprintable)
struct PROJECT_COUNTESS_API FMaster_Data
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MasterData", meta = (DisplayName = "Ability Data"))
	//TArray<TMap<TSubclassOf<class UCountess_GameplayAbility_Base>, float>> AbilityData;
	TMap<TSubclassOf<class UCountess_GameplayAbility_Base>, FAbilityDetails> AbilityData;

};
*/
/*
UCLASS(BlueprintType, Blueprintable)
class PROJECT_COUNTESS_API UMaster_Data_Table : public UCurveTable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MasterData", meta = (DisplayName = "Master Data"))
	FMaster_Data MasterData;
};
*/