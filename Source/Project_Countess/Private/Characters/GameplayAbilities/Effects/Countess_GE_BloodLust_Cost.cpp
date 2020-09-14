// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_BloodLust_Cost.h"
#include "Characters/GameplayAbilities/Calculations/Countess_GE_BloodLust_Cost_Exec.h"

UCountess_GE_BloodLust_Cost::UCountess_GE_BloodLust_Cost()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayEffectExecutionDefinition Definition;
	Definition.CalculationClass = UCountess_GE_BloodLust_Cost_Exec::StaticClass();
	this->Executions.Add(Definition);

	InheritableOwnedTagsContainer.AddTag(CountessTags::FCostTags::BloodLustAbilityCostTag);
}
