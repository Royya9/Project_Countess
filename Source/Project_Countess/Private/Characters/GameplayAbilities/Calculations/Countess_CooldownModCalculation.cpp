// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Calculations/Countess_CooldownModCalculation.h"
#include "Kismet/GameplayStatics.h"

float UCountess_CooldownModCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Just retun time dilation which neutralizes the cooldown
	return UGameplayStatics::GetGlobalTimeDilation(Spec.GetEffectContext().GetInstigator());
}
