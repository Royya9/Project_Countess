// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/Countess_Interface_Actor.h"
#include "Project_CountessCharacter.generated.h"

/*Forward Declarations*/
class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;
class USoundWave;
class UParticleSystem;
class ACountess_PlayerState;
class ACountess_PlayerController;
class UCountess_AbilitySystemComponent;
class UCountess_AttributeSet_Base;
class UCountess_GameplayAbility_Base;

UCLASS(config=Game)
class AProject_CountessCharacter : public ACharacter, public IAbilitySystemInterface, public ICountess_Interface_Actor
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/*Location of Feet to spawn emitters (preferrably dust)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* FeetLocationArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	USoundWave* SoundToPlayOnLanding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ParticleSystemOnLanding;

	/*scaling for size of particle system to played on landing*/
	float JumpScale;

	/*Attribute Set for our Countess*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	UCountess_AttributeSet_Base* AttributeSet;

	/*Reference to our Countesss PlayerState*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerState, meta = (AllowPrivateAccess = "true"))
	ACountess_PlayerState* Countess_PlayerState;

	/*Reference to our Countesss PlayerState*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerState, meta = (AllowPrivateAccess = "true"))
	ACountess_PlayerController* Countess_PlayerController;


public:
	/*Ability System*/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComp;

	//Implementation of IAbilitySystemInterface
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


public:
	/*Implementation of ICountess_Interface_Actor*/

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool GiveAbilityOnOverlap(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGive);
	virtual bool GiveAbilityOnOverlap_Implementation(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGive) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool GiveAbilityEndOverlap();
	virtual bool GiveAbilityEndOverlap_Implementation() override;

	UFUNCTION()
	void AbilityAcquiredInfoToGAGrantingActor(FSlateBrush AbilityIcon, float Cooldown);

private:
	/**/
	//TArray<TSubclassOf<UCountess_GameplayAbility_Base>> AcquiredAbilities;

	
public:
	/* Ability functions*/


public:
	/*VFX*/

	/*Play VFX on Landing*/
	virtual void Landed(const FHitResult& Hit) override;

public:
	AProject_CountessCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/* Returns FeetLocationArrowComponent subobject*/
	FORCEINLINE UArrowComponent* GetFeetLocationArrowComponent() const { return FeetLocationArrowComponent; }

	UCountess_AttributeSet_Base* GetAttributeSet() const { return AttributeSet; }

public:
	/*Overrides*/
	
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

};
