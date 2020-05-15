// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Jump_CoolDown.h"

UCountess_GE_Jump_CoolDown::UCountess_GE_Jump_CoolDown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	// #TODO Work on this Hardcoded value in Duration Magnitude
	DurationMagnitude = FGameplayEffectModifierMagnitude(3.1f);
	//DurationMagnitude = FGameplayEffectModifierMagnitude(FSetByCallerFloat)

	//FInheritedTagContainer InheritableOwnedTagsContainer
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Jumping")));
}
