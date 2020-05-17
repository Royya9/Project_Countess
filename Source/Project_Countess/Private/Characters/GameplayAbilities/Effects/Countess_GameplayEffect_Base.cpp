// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GameplayEffect_Base.h"
#include "Engine/CurveTable.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GameplayEffect_Base::UCountess_GameplayEffect_Base()
{
	//Get our AbilityDetails Curve Table
	static ConstructorHelpers::FObjectFinder<UCurveTable> AbilityDetailsObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails'"));
	if (AbilityDetailsObject.Succeeded())
	{
		AbilityDetailsTable = AbilityDetailsObject.Object;
	}
	else
	{
		UE_LOG(Countess_Log, Error, TEXT("Cannot find AbilityDetails curve table in location : /Game/MyProjectMain/Blueprints/Characters/Abilities/AbilityDetails. Check if it is moved. From %s"), TEXT(__FUNCTION__));
	}

	//Get our Countess AttributeSet
	static ConstructorHelpers::FClassFinder<UCountess_AttributeSet_Base> AttributeSetClass(TEXT("'/Script/Project_Countess.Countess_AttributeSet_Base'"));
	if (AttributeSetClass.Succeeded())
	{
		Countess_AttributeSet = Cast<UCountess_AttributeSet_Base>(AttributeSetClass.Class->GetDefaultObject(false));
	}
	else
	{
		UE_LOG(Countess_Log, Error, TEXT("Cannot find Countess_Attribute_Set C++ class. From %s"), TEXT(__FUNCTION__));
	}
}
