// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Interfaces/Countess_Interface_Actor.h"
#include "Countess_Character_Base.generated.h"

class UCountess_AbilitySystemComponent;
class UCountess_AttributeSet_Base;

UCLASS()
class PROJECT_COUNTESS_API ACountess_Character_Base : public ACharacter, public IAbilitySystemInterface, public ICountess_Interface_Actor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	int32 CharacterBaseLevel;
	
public:
	// Sets default values for this character's properties
	ACountess_Character_Base();


	TWeakObjectPtr<UCountess_AbilitySystemComponent> AbilitySystemComponent;

	TWeakObjectPtr<UCountess_AttributeSet_Base> AttributeSet;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // Override from IAbilitySystemInterface

	virtual UAbilitySystemComponent* GetASC() const override; // Override from ICountess_Interface_Actor

	virtual bool IsAlive() const;

	virtual int32 GetCharacterLevel() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
