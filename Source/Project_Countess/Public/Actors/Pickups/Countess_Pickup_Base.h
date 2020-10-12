// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/Actor.h"
#include "Countess_Pickup_Base.generated.h"

class UCountess_Item;
class USphereComponent;
class UStaticMeshComponent;
class UStaticMesh;

/* Base class for all our pickups. Not to be blueprinted directly*/
UCLASS(Abstract, BlueprintType)
class PROJECT_COUNTESS_API ACountess_Pickup_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACountess_Pickup_Base();

	virtual void BeginPlay() override;
	
	/* Item to be picked up*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickupItem")
	TSubclassOf<UCountess_Item> PickupItem;

	/* Number of items this actor drops */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickupItem")
	int32 Count;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "PickupItem")
	void OnItemPickedUp(bool bItemPickedUp, int32& NumberOfItemsLeft);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PickupItem")
	USphereComponent* SphereCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PickupItem")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PickupItem")
	UStaticMesh* StaticMesh;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
