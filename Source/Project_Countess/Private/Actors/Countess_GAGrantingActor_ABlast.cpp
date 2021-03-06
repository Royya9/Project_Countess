// Free to play. By SoNa Games.


#include "Actors/Countess_GAGrantingActor_ABlast.h"
#include "Components/BoxComponent.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_ArcBlas.h"
#include "Interfaces/Countess_Interface_Actor.h"

ACountess_GAGrantingActor_ABlast::ACountess_GAGrantingActor_ABlast()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent_HardRef = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
	BoxComponent_HardRef->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	RootComponent = BoxComponent_HardRef;
	BoxComponent_HardRef->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	BoxComponent = BoxComponent_HardRef;
	//BoxComponent->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Overlap);
	//BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

	AbilityToGrant = UCountess_GameplayAbility_ArcBlas::StaticClass();

	bAcquired_Ability = false;
}

void ACountess_GAGrantingActor_ABlast::AbilityAcquired(TSubclassOf<UCountess_GameplayAbility_Base> AcquiredAbility)
{
	Super::AbilityAcquired(AcquiredAbility);
	if (AcquiredAbility.GetDefaultObject() != AbilityToGrant.GetDefaultObject())
		return;
	// 	UE_LOG(LogTemp, Warning, TEXT("Ability Acquired!! From %s"), TEXT(__FUNCTION__));
		//native implementation: set bool to true so that when player overlaps again, this actor does nothing.
	bAcquired_Ability = true;
	// pass to blueprint implementation which destorys actor componets like skill book mesh, lights etc and does some VFX may be?
	AbilityAcquiredEvent_Implementation();
}

void ACountess_GAGrantingActor_ABlast::AbilityAcquiredEvent_Implementation()
{
	AbilityAcquiredEvent();
}

bool ACountess_GAGrantingActor_ABlast::IsAbilityAcquired() const
{
	return bAcquired_Ability;
}

UBoxComponent* ACountess_GAGrantingActor_ABlast::GetBoxComponent() const
{
	return BoxComponent_HardRef;
}

TSubclassOf<UCountess_GameplayAbility_Base> ACountess_GAGrantingActor_ABlast::GetAbilityToGrant() const
{
	return AbilityToGrant;
}

void ACountess_GAGrantingActor_ABlast::BeginPlay()
{
	Super::BeginPlay();
}
