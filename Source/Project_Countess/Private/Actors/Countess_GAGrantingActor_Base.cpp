// Free to play. By SoNa Games.


#include "Actors/Countess_GAGrantingActor_Base.h"
#include "Components/BoxComponent.h"
#include "Interfaces/Countess_Interface_Actor.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"

// Sets default values
ACountess_GAGrantingActor_Base::ACountess_GAGrantingActor_Base()
{
	PrimaryActorTick.bCanEverTick = false;
	bAcquired_Ability = false;
}

void ACountess_GAGrantingActor_Base::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlapped. From %s"), TEXT(__FUNCTION__));
	if (bAcquired_Ability) //Ability already Acquired. Do Nothing
		return;

	if (!AbilityToGrant)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilityToGrant class is not FOUND in Countess_GAGrantingActor"));
	}
	ICountess_Interface_Actor* PlayerCharacter_Interface = Cast<ICountess_Interface_Actor>(OtherActor);
	if (PlayerCharacter_Interface)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor is %s"), *PlayerCharacter->GetName());
		// Give Ability Class to the actor implementing the Interface. in this case our player character
		PlayerCharacter_Interface->Execute_GiveAbilityOnOverlap(OtherActor, AbilityToGrant);

		//bind to the delegate (declared in interface) which tells us ability is acquired by our player
		if (!PlayerCharacter_Interface->CountessAbilityAcquired_Interface_Delegate.IsBound())
		{
			PlayerCharacter_Interface->CountessAbilityAcquired_Interface_Delegate.AddDynamic(this, &ACountess_GAGrantingActor_Base::AbilityAcquired_Implementation);
		}
	}
}


void ACountess_GAGrantingActor_Base::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bAcquired_Ability) //Ability already Acquired. Do Nothing
		return;

	ICountess_Interface_Actor* PlayerCharacter_Interface = Cast<ICountess_Interface_Actor>(OtherActor);
	if (PlayerCharacter_Interface)
	{
		PlayerCharacter_Interface->Execute_GiveAbilityEndOverlap(OtherActor);
	}
}



void ACountess_GAGrantingActor_Base::AbilityAcquired_Implementation()
{
// 	UE_LOG(LogTemp, Warning, TEXT("Ability Acquired!! From %s"), TEXT(__FUNCTION__));
	//native implementation: set bool to true so that when player overlaps again, this actor does nothing.
	bAcquired_Ability = true;
	// pass to blueprint implementation which destorys actor componets like skill book mesh, lights etc and does some VFX may be?
	AbilityAcquired();
}

// Called when the game starts or when spawned
void ACountess_GAGrantingActor_Base::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACountess_GAGrantingActor_Base::BeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACountess_GAGrantingActor_Base::EndOverlap);
}



