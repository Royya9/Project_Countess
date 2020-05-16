// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Countess_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API UCountess_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
};
