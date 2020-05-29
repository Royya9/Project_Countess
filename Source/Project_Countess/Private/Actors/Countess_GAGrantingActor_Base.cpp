// Free to play. By SoNa Games.


#include "Actors/Countess_GAGrantingActor_Base.h"
#include "Components/BoxComponent.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Base.h"
#include "Interfaces/Countess_Interface_Actor.h"


ACountess_GAGrantingActor_Base::ACountess_GAGrantingActor_Base()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ACountess_GAGrantingActor_Base::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsAbilityAcquired()) //Ability already Acquired. Do Nothing
		return;

	ICountess_Interface_Actor* PlayerCharacter_Interface = Cast<ICountess_Interface_Actor>(OtherActor);
	if (PlayerCharacter_Interface)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor is %s"), *PlayerCharacter->GetName());
		// Give Ability Class to the actor implementing the Interface. in this case our player character
		PlayerCharacter_Interface->Execute_GiveAbilityOnOverlap(OtherActor, GetAbilityToGrant());

		//bind to the delegate (declared in interface) which tells us ability is acquired by our player
		//if (!PlayerCharacter_Interface->CountessAbilityAcquired_Interface_Delegate.IsBound())
		//AbilityAcquiredDelegateFromBaseClass = PlayerCharacter_Interface->CountessAbilityAcquired_Interface_Delegate;
		PlayerCharacter_Interface->CountessAbilityAcquired_Interface_Delegate.AddDynamic(this, &ACountess_GAGrantingActor_Base::AbilityAcquired);

	}
}


void ACountess_GAGrantingActor_Base::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsAbilityAcquired()) //Ability already Acquired. Do Nothing
		return;

	ICountess_Interface_Actor* PlayerCharacter_Interface = Cast<ICountess_Interface_Actor>(OtherActor);
	if (PlayerCharacter_Interface)
	{
		PlayerCharacter_Interface->CountessAbilityAcquired_Interface_Delegate.Remove(this, FName("AbilityAcquired"));
		PlayerCharacter_Interface->Execute_GiveAbilityEndOverlap(OtherActor);
	}
}

void ACountess_GAGrantingActor_Base::AbilityAcquired(TSubclassOf<UCountess_GameplayAbility_Base> AcquiredAbility)
{

}

bool ACountess_GAGrantingActor_Base::IsAbilityAcquired() const
{
	return bAcquired_Ability_Base;
}

UBoxComponent* ACountess_GAGrantingActor_Base::GetBoxComponent() const
{
	return BoxComponent.Get(false);
}

TSubclassOf<UCountess_GameplayAbility_Base> ACountess_GAGrantingActor_Base::GetAbilityToGrant() const
{
	return AbilityToGrant_Base;
}

// Called when the game starts or when spawned
void ACountess_GAGrantingActor_Base::BeginPlay()
{
	Super::BeginPlay();
	GetBoxComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACountess_GAGrantingActor_Base::BeginOverlap);
	GetBoxComponent()->OnComponentEndOverlap.AddDynamic(this, &ACountess_GAGrantingActor_Base::EndOverlap);
}