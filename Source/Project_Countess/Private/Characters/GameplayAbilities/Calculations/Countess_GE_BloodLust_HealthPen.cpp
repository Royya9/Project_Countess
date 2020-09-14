// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Calculations/Countess_GE_BloodLust_HealthPen.h"
#include "Engine/CurveTable.h"

UCountess_GE_BloodLust_HealthPen::UCountess_GE_BloodLust_HealthPen()
{
	//Get our AbilityDetails Curve Table
	static ConstructorHelpers::FObjectFinder<UCurveTable> AbilityDetailsObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails'"));
	if (AbilityDetailsObject.Succeeded())
	{
		AbilityDetailsTable = AbilityDetailsObject.Object;
	}
	else
	{
		UE_LOG(Countess_Log, Error, TEXT("Cannot find AbilityDetails curve table in location : /Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails. Check if it is moved. From %s"), TEXT(__FUNCTION__));
	}
}

float UCountess_GE_BloodLust_HealthPen::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FString ContextString;
	FSimpleCurve* HealthPenaltyRowHanle = AbilityDetailsTable->FindSimpleCurve(FName("BloodLustAbilityHealthRegenPenaltyPercentage"), ContextString);

	const float PenaltyPercentage = HealthPenaltyRowHanle->Eval(Spec.GetLevel());

	// We are dividing our current HealthRegenRate by this number
	// For eg., if our Penalty Percentage is 100, then our HealthRegenRate has to reduce by a factor of 2 (divide by 2)
	// if our Penalty Percentage is 50, then our HealthRegenRate has to reduce by a factor of 1.5 (divide by 1.5)

	return (1 + PenaltyPercentage/100);

}
