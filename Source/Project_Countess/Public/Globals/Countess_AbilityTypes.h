// Free to play. By SoNa Games.

#pragma once

//#include "UObject/Object.h"
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


