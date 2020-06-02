// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Fireball_Damage.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Characters/GameplayAbilities/Calculations/Countess_GE_Damage_Execution.h"

UCountess_GE_Fireball_Damage::UCountess_GE_Fireball_Damage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayEffectExecutionDefinition Definition;
	Definition.CalculationClass = UCountess_GE_Damage_Execution::StaticClass();
	this->Executions.Add(Definition);

	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Fireball.Damage")));
}
