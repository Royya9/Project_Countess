// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GameplayEffect_Base.h"
#include "Engine/CurveTable.h"

UCountess_GameplayEffect_Base::UCountess_GameplayEffect_Base()
{
	static ConstructorHelpers::FObjectFinder<UCurveTable> AbilityDetailsObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails'"));
	if (AbilityDetailsObject.Succeeded())
	{
		AbilityDetailsTable = AbilityDetailsObject.Object;
	}
	else
	{
		UE_LOG(Countess_Log, Error, TEXT("Cannot find AbilityDetails curve table in location : /Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails. Check if it is moved. From %s"), TEXT(__FUNCTION__));
	}
}
