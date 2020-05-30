// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Countess_Character_Base.generated.h"

class UCountess_AbilitySystemComponent;
class UCountess_AttributeSet_Base;

UCLASS()
class PROJECT_COUNTESS_API ACountess_Character_Base : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACountess_Character_Base();


	TWeakObjectPtr<UCountess_AbilitySystemComponent> AbilitySystemComponent;

	TWeakObjectPtr<UCountess_AttributeSet_Base> AttributeSet;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
