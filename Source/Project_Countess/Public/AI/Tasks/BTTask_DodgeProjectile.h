// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DodgeProjectile.generated.h"

class AActor;
class UAISense;

/**
 *  Task that senses an incoming projectile (eg Fireball) and tries to dodge it by jumping 
 */
UCLASS()
class PROJECT_COUNTESS_API UBTTask_DodgeProjectile : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	/*Dodge Chance = 1 enables AI to always dodge.*/
	UPROPERTY(EditAnywhere, Category = "Parameters", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float DodgeChance = 1.f;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	TSubclassOf<AActor> ProjectileToDodge_Class;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	TSubclassOf<UAISense> SenseToDetectIncomingProjectile;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
