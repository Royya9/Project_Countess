// Free to play. By SoNa Games.


#include "Characters/Countess_Character_Base.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"

// Sets default values
ACountess_Character_Base::ACountess_Character_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* ACountess_Character_Base::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

// Called when the game starts or when spawned
void ACountess_Character_Base::BeginPlay()
{
	Super::BeginPlay();
	
}



