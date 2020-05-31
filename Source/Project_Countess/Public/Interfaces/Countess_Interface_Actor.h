// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "UObject/Interface.h"
#include "Countess_Interface_Actor.generated.h"

class UCountess_GameplayAbility_Base;
class UAbilitySystemComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCountess_Interface_Actor : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface b/w Player Character and GA Granting Actor
 */
class PROJECT_COUNTESS_API ICountess_Interface_Actor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool GiveAbilityOnOverlap(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGive);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool GiveAbilityEndOverlap();

	virtual UAbilitySystemComponent* GetASC() const = 0;

	FCountessAbilityAcquired_Interface_Delegate CountessAbilityAcquired_Interface_Delegate;
};
