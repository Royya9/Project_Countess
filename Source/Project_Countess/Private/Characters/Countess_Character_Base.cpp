// Free to play. By SoNa Games.


#include "Characters/Countess_Character_Base.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
ACountess_Character_Base::ACountess_Character_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ACountess_Character_Base::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UAbilitySystemComponent* ACountess_Character_Base::GetASC() const
{
	return AbilitySystemComponent.Get();
}


bool ACountess_Character_Base::IsAlive() const
{
	return AttributeSet.Get()->GetHealth() > 0.f;
}

// Called when the game starts or when spawned
void ACountess_Character_Base::BeginPlay()
{
	Super::BeginPlay();	
}



