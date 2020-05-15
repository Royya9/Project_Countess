// Free to play. By SoNa Games.


#include "Actors/Countess_GAGrantingActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/Countess_Interface_Actor.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"

// Sets default values
ACountess_GAGrantingActor::ACountess_GAGrantingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	RootComponent = StaticMesh;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	AbilityToGrant = UCountess_GameplayAbility_Base::StaticClass();
}

void ACountess_GAGrantingActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!AbilityToGrant)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilityToGrant class is not FOUND in Countess_GAGrantingActor"));
	}
	ICountess_Interface_Actor* PlayerCharacter_Interface = Cast<ICountess_Interface_Actor>(OtherActor);
	if (PlayerCharacter_Interface)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor is %s"), *PlayerCharacter->GetName());
		PlayerCharacter_Interface->Execute_GiveAbilityOnOverlap(OtherActor, AbilityToGrant);
	}
	
}

// Called when the game starts or when spawned
void ACountess_GAGrantingActor::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACountess_GAGrantingActor::BeginOverlap);
}



