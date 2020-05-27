// Free to play. By SoNa Games.


#include "Actors/Countess_GAGrantingActor_DJump.h"
#include "Components/BoxComponent.h"
#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_DJump.h"
#include "Interfaces/Countess_Interface_Actor.h"


ACountess_GAGrantingActor_DJump::ACountess_GAGrantingActor_DJump()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent_HardRef_DJump = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
	RootComponent = BoxComponent_HardRef_DJump;
	BoxComponent_HardRef_DJump->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	BoxComponent = BoxComponent_HardRef_DJump;
	//BoxComponent->SetCollisionResponseToChannel(ECC_Camera, ECollisionResponse::ECR_Overlap);
	//BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

	AbilityToGrant = UCountess_GameplayAbility_DJump::StaticClass();

	bAcquired_Ability = false;
}

void ACountess_GAGrantingActor_DJump::AbilityAcquired(TSubclassOf<UCountess_GameplayAbility_Base> AcquiredAbility)
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

void ACountess_GAGrantingActor_DJump::AbilityAcquiredEvent_Implementation()
{
	AbilityAcquiredEvent();
}

bool ACountess_GAGrantingActor_DJump::IsAbilityAcquired() const
{
	return bAcquired_Ability;
}

UBoxComponent* ACountess_GAGrantingActor_DJump::GetBoxComponent() const
{
	return BoxComponent_HardRef_DJump;
}

TSubclassOf<UCountess_GameplayAbility_Base> ACountess_GAGrantingActor_DJump::GetAbilityToGrant() const
{
	return AbilityToGrant;
}

// Called when the game starts or when spawned
void ACountess_GAGrantingActor_DJump::BeginPlay()
{
	Super::BeginPlay();
}