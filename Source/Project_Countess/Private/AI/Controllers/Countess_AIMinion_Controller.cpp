// Free to play. By SoNa Games.


#include "AI/Controllers/Countess_AIMinion_Controller.h"
#include "AI/Characters/Countess_Character_AI_Minion.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

ACountess_AIMinion_Controller::ACountess_AIMinion_Controller()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AI Perception Component"));

	UAISenseConfig_Sight* AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("AI Sense Config Sight"));
	UAISenseConfig_Hearing* AISenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(FName("AI Sense Config Hearing"));

	AISenseConfig_Sight->SightRadius = 600.f;
	AISenseConfig_Sight->LoseSightRadius = 700.f;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;

	AISenseConfig_Hearing->HearingRange = 500.f;

	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfig_Hearing);
	AIPerceptionComponent->SetDominantSense(AISenseConfig_Sight->GetSenseImplementation());
	
}

void ACountess_AIMinion_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACountess_Character_AI_Minion* Minion_Character = Cast<ACountess_Character_AI_Minion>(InPawn);
	if (Minion_Character)
	{
		UBehaviorTree* BehaviorTree = Minion_Character->MinionBehaviorTree;
		if (BehaviorTree)
			RunBehaviorTree(BehaviorTree);

	}

}

void ACountess_AIMinion_Controller::BeginPlay()
{
	Super::BeginPlay();
	
/*
	FAIMoveRequest Request;
	Request.SetGoalLocation(FVector(0, 0, 293.f));
	Request.SetCanStrafe(true);
	this->MoveTo(Request);*/


	this->SetGenericTeamId(FGenericTeamId(1));
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACountess_AIMinion_Controller::OnPerceptionUpdated);
}

void ACountess_AIMinion_Controller::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(Countess_Log, Warning, TEXT("From %s. Perception Updated by %s and received %s Stimulus."), TEXT(__FUNCTION__), *Actor->GetFName().ToString(), *Stimulus.Tag.ToString());
	TArray<AActor*> Perceived_Actors;
	AIPerceptionComponent->GetCurrentlyPerceivedActors(TSubclassOf<UAISense_Sight>(), Perceived_Actors);
	bool bHasEntered = Perceived_Actors.Contains(Actor);
	int NumActorsSeen = Perceived_Actors.Num();
	UE_LOG(Countess_Log, Warning, TEXT("%s has just %s the view now."), *Actor->GetFName().ToString(), bHasEntered ? TEXT("entered") : TEXT("left"));
}
