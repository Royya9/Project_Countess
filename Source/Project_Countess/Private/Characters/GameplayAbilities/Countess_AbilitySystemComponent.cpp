// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"

void UCountess_AbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

void UCountess_AbilitySystemComponent::OnDamageReceived(UCountess_AbilitySystemComponent* SourceASC, float UnMitigatedDamage, float MitigatedDamage)
{
	OnDamageReceivedDelegate.Broadcast(SourceASC, UnMitigatedDamage, MitigatedDamage);
}
