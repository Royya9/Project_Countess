// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Countess_Interface_Actor.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCountess_Interface_Actor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_COUNTESS_API ICountess_Interface_Actor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool GiveAbilityOnOverlap( TSubclassOf<class UCountess_GameplayAbility_Base> AbilityToGive);
};
