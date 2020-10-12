// Free to play. By SoNa Games.


#include "Actors/Pickups/Countess_Pickup_Potion_Health.h"
#include "Items/Countess_Item_Potion_Health.h"
#include "Engine/StaticMesh.h"

ACountess_Pickup_Potion_Health::ACountess_Pickup_Potion_Health()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshHelper(TEXT("/Game/ParagonMinions/FX/Meshes/Shapes/SM_Sphere"));
    if(MeshHelper.Succeeded())
    {
        StaticMesh = MeshHelper.Object;
        StaticMeshComponent->SetStaticMesh(StaticMesh);
    }
    else
        UE_LOG(Countess_Log, Warning, TEXT("From %s. Static Mesh not found "), TEXT(__FUNCTION__));

    
    //Material'/Engine/EngineMaterials/WorldGridMaterial.WorldGridMaterial'
    PickupItem = UCountess_Item_Potion_Health::StaticClass();
    Count = 50;
}

void ACountess_Pickup_Potion_Health::OnItemPickedUp_Implementation(bool bItemPickedUp, int32& NumberOfItemsLeft)
{
    Super::OnItemPickedUp_Implementation(bItemPickedUp, NumberOfItemsLeft);
    // Grant Item Ability/Effect to user and reduce count (and destroy it if needed)
}
