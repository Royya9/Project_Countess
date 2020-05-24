// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Countess_GAGrantingActor_Base.h"
#include "Countess_GAGrantingActor_Jump.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_GAGrantingActor_Jump : public ACountess_GAGrantingActor_Base
{
	GENERATED_BODY()
	
public:

	ACountess_GAGrantingActor_Jump();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent_HardRef;

	USoundBase* SoundToPlayOnInteract_HardRef;

};
