// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Countess_AIMinion_Controller.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UBehaviorTree;

/**
 *  Our AI MinionController
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_AIMinion_Controller : public AAIController
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	TWeakObjectPtr<UBehaviorTree> BehaviorTree_WeakPtr;

public:

	ACountess_AIMinion_Controller();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
