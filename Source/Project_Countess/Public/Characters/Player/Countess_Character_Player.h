// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Characters/Countess_Character_Base.h"
#include "Components/TimelineComponent.h"
#include "Globals/Project_Countess.h"
#include "Countess_Character_Player.generated.h"

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
class UCurveFloat;
class UFloatingPawnMovement;
class UAnimMontage;
class UCapsuleComponent;
class USoundCue;

UCLASS(config=Game)
class ACountess_Character_Player : public ACountess_Character_Base
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

	/*Location of Feet to spawn emitters (preferrably dust)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* FireballSpawnLocationArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	USoundWave* SoundToPlayOnLanding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ParticleSystemOnLanding;

	/*Floating Pawn Movement enabled during ElectroSparkVFX*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleSwordCollisionLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleSwordCollisionRight;
	/*scaling for size of particle system to played on landing*/
	float JumpScale;

	ACountess_PlayerController* Countess_PlayerController;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability | BackDash", meta = (AllowPrivateAccess = "true"))
	FVector BackDashLeftVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability | BackDash", meta = (AllowPrivateAccess = "true"))
	FVector BackDashRightVector;

	FVector StartBackDashLoc;
	FVector EndBackDashLoc;

	FTimeline BackDashTimeLine;

	UCurveFloat* CurveFloat;

	USoundCue* BeginPlaySoundCue;

	USoundCue* CooldownSoundCue;

	USoundCue* CostSoundCue;

	FGenericTeamId PlayerTeamId;

	UFUNCTION()
	void TimeLineProgress(float Value);

public:
	/*Ability */

	/*Handled by BackDash Ability*/
	void BeginBackDash();
	
	/*Handled by ElectroSpark Ability*/
	void ElectroSparkOn();

	/*Handled by ElectroSpark Ability*/
	void ElectroSparkOff();
	/*Reference to our Countesss PlayerState*/

	ACountess_PlayerState* Countess_PlayerState;

public:
	/*Implementation of ICountess_Interface_Actor*/

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool GiveAbilityOnOverlap(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGive);
	virtual bool GiveAbilityOnOverlap_Implementation(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGive) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool GiveAbilityEndOverlap();
	virtual bool GiveAbilityEndOverlap_Implementation() override;

	UFUNCTION()
	void AbilityAcquiredInfoToGAGrantingActor(TSubclassOf<UCountess_GameplayAbility_Base> AbilityAcquiredClass, FSlateBrush AbilityIcon, float Cooldown);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* MontageToPlayOnGameBegin;

	UFUNCTION() // Our begin play montage ended. so enable input and clear this timer
	void MontageToPlayOnGameBeginEnded(FTimerHandle& TimerHandle) const;

	UFUNCTION()
	void OnLeftSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:
	/**/
	bool bIsDoubleJumping;

	bool bIsSoundPlaying;

public:
	/* Ability functions*/
	UFUNCTION(BlueprintCallable)
	bool GetIsDoubleJumping() const;

	void SetIsDoubleJumping(bool bNewState);

	UFUNCTION()
	void AbilityFailedCallback(const UGameplayAbility* FailedAbility, const FGameplayTagContainer& TagContainer);

	UFUNCTION()
	void ClearTimer(FTimerHandle& TimerHandle, bool bSetIsSoundPlaying = true);

public:
	/*VFX*/

	/*Play VFX on Landing*/
	UFUNCTION()
	virtual void Landed(const FHitResult& Hit) override;

public:
	ACountess_Character_Player();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/* Returns FeetLocationArrowComponent subobject*/
	FORCEINLINE UArrowComponent* GetFeetLocationArrowComponent() const { return FeetLocationArrowComponent; }
	/* Returns FeetLocationArrowComponent subobject*/
	FORCEINLINE UArrowComponent* GetFireballSpawnLocationArrowComponent() const { return FireballSpawnLocationArrowComponent; }

	UFUNCTION(BlueprintCallable, Category = "Countess | Components")
	FORCEINLINE UCapsuleComponent* GetCollisionSwordLeftComponent() { return CapsuleSwordCollisionLeft; }

	UFUNCTION(BlueprintCallable, Category = "Countess | Components")
	FORCEINLINE UCapsuleComponent* GetCollisionSwordRightComponent() { return CapsuleSwordCollisionRight; }

public:
	/*Overrides*/
	
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual bool IsAlive() const override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual int32 GetCharacterLevel() const override;
};
