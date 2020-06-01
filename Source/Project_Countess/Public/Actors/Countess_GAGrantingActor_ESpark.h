// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Countess_GAGrantingActor_Base.h"
#include "Countess_GAGrantingActor_ESpark.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_GAGrantingActor_ESpark : public ACountess_GAGrantingActor_Base
{
	GENERATED_BODY()

public:

	ACountess_GAGrantingActor_ESpark();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComponent_HardRef;

	virtual void AbilityAcquired(TSubclassOf<UCountess_GameplayAbility_Base> AcquiredAbility) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AbilityAcquiredEvent();

	virtual bool IsAbilityAcquired() const override;

	virtual UBoxComponent* GetBoxComponent() const override;

	virtual TSubclassOf<UCountess_GameplayAbility_Base> GetAbilityToGrant() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bAcquired_Ability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbility, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGrant;
	
};
