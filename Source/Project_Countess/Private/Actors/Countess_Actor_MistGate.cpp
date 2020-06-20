// Free to play. By SoNa Games.


#include "Actors/Countess_Actor_MistGate.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACountess_Actor_MistGate::ACountess_Actor_MistGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
	RootComponent = BoxComponent;
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore BlackMagic (eg., Fireball)
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap); // Overlap Mist

	ArchComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Arch Component"));
	ArchComponent->SetupAttachment(RootComponent);

	GateComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Gate Component"));
	GateComponent->SetupAttachment(RootComponent);
	GateComponent->SetCollisionProfileName(FName("IgnoreMist"));
}

// Called when the game starts or when spawned
void ACountess_Actor_MistGate::BeginPlay()
{
	Super::BeginPlay();	
}

