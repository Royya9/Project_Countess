// Free to play. By SoNa Games.


#include "Actors/Countess_LensOfTruth_Platform.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACountess_LensOfTruth_Platform::ACountess_LensOfTruth_Platform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));

	//The mesh has no collision and is not visible by default. This is toggled when player activates LensOfTruth Ability
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetVisibility(false);
}

void ACountess_LensOfTruth_Platform::ToggleStaticMesh(const bool bLensOfTruthActive) const
{
	if(bLensOfTruthActive) // LensOfTruth is Active
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StaticMeshComponent->SetVisibility(true);
	}
	else // LensOfTruth is inActive
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMeshComponent->SetVisibility(false);
	}
}


