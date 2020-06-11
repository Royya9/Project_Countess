// Free to play. By SoNa Games.


#include "AI/Tasks/BTTask_DodgeProjectile.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Actor.h"
#include "Perception/AISense.h"
#include "Math/Vector.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

EBTNodeResult::Type UBTTask_DodgeProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MinionAIController = OwnerComp.GetAIOwner();
	UAIPerceptionComponent* AIPerceptionComponent = MinionAIController->GetAIPerceptionComponent();
	ACharacter* MinionCharacter = MinionAIController->GetCharacter();
	AActor* ProjectileToDodgeCDO = ProjectileToDodge_Class.GetDefaultObject();

	if (!MinionAIController || !AIPerceptionComponent || !MinionCharacter)
		return EBTNodeResult::Failed;

	TArray<AActor*> IncomingProjectiles;
	AIPerceptionComponent->GetCurrentlyPerceivedActors(SenseToDetectIncomingProjectile, IncomingProjectiles);

	/*Try to Dodge the first projectile.*/
	if (IncomingProjectiles.Contains(ProjectileToDodgeCDO))
	{
		//calculate velocity and jump at precise time to dodge
		//float ProjectileDodge_Velocity = ProjectileToDodgeCDO->GetVelocity().X;
		FVector ProjectileToDodge_Velocity = ProjectileToDodgeCDO->GetVelocity().GetSafeNormal();
		FVector MinionCharacter_ForwardVector = MinionCharacter->GetActorForwardVector();
		
		float DotProduct_value = FMath::Abs(FVector::DotProduct(ProjectileToDodge_Velocity, MinionCharacter_ForwardVector));
		float random_num = FMath::RandRange(0, 1);
		if (DotProduct_value < 0.85 || random_num > DodgeChance || MinionCharacter->GetMovementComponent()->IsFalling())
			return EBTNodeResult::Failed;
		
		MinionCharacter->Jump();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
