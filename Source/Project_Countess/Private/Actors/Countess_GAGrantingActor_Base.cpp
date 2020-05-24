// Free to play. By SoNa Games.


#include "Actors/Countess_GAGrantingActor_Base.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/Countess_Interface_Actor.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"

// Sets default values
ACountess_GAGrantingActor_Base::ACountess_GAGrantingActor_Base()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACountess_GAGrantingActor_Base::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlapped. From %s"), TEXT(__FUNCTION__));
	if (!AbilityToGrant)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilityToGrant class is not FOUND in Countess_GAGrantingActor"));
	}
	ICountess_Interface_Actor* PlayerCharacter_Interface = Cast<ICountess_Interface_Actor>(OtherActor);
	if (PlayerCharacter_Interface)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor is %s"), *PlayerCharacter->GetName());
		bool bAcquired_Ability = PlayerCharacter_Interface->Execute_GiveAbilityOnOverlap(OtherActor, AbilityToGrant);
		if (SoundToPlayOnInteract.Get(false) && bAcquired_Ability)
		{
			UGameplayStatics::PlaySound2D(this, SoundToPlayOnInteract.Get(false));
		}
		
	}
	
}

// Called when the game starts or when spawned
void ACountess_GAGrantingActor_Base::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACountess_GAGrantingActor_Base::BeginOverlap);
}



