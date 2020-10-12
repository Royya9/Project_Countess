// Free to play. By SoNa Games.


#include "Actors/Pickups/Countess_Pickup_Base.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Interfaces/Countess_Interface_Inventory.h"

// Sets default values
ACountess_Pickup_Base::ACountess_Pickup_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(FName("Sphere Collision"));
	RootComponent = SphereCollisionComponent;
	SphereCollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	SphereCollisionComponent->SetSphereRadius(40.f);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetRelativeScale3D(FVector(0.5f));

	StaticMeshComponent->SetStaticMesh(StaticMesh);

	
}

void ACountess_Pickup_Base::BeginPlay()
{
	Super::BeginPlay();
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACountess_Pickup_Base::OnBeginOverlap);
}

void ACountess_Pickup_Base::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		ICountess_Interface_Inventory* InventoryActor = Cast<ICountess_Interface_Inventory>(OtherActor);
		if(InventoryActor)
		{
			const bool bPickedUp = InventoryActor->Execute_GiveItemOnOverlap(OtherActor, PickupItem, Count);
			OnItemPickedUp(bPickedUp, Count);
		}
	}
}

void ACountess_Pickup_Base::OnItemPickedUp_Implementation(bool bItemPickedUp, int32& NumberOfItemsLeft)
{
	UE_LOG(Countess_Log, Warning, TEXT("ItemPickedUp = %s and Number of Items Left = %d"), bItemPickedUp? TEXT("Yes"): TEXT("No"), NumberOfItemsLeft);
	if(bItemPickedUp && NumberOfItemsLeft == 0)
		Destroy();
	else if(bItemPickedUp && NumberOfItemsLeft < Count)
		Count = NumberOfItemsLeft;

}
