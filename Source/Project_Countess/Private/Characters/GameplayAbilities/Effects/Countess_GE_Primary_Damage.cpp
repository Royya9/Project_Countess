// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_Primary_Damage.h"
#include "Characters/GameplayAbilities/Calculations/Countess_GE_Primary_Execution.h"

UCountess_GE_Primary_Damage::UCountess_GE_Primary_Damage()
{
    DurationPolicy = EGameplayEffectDurationType::Instant;

    FGameplayEffectExecutionDefinition Definition;
    Definition.CalculationClass = UCountess_GE_Primary_Execution::StaticClass();
    this->Executions.Add(Definition);

    InheritableOwnedTagsContainer.AddTag(CountessTags::FDamageTags::PrimaryAbilityDamageTag);
}
