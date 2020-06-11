// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Characters/Player/Countess_Character_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Countess_PlayerState.h"
#include "Player/Countess_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Animation/AnimInstance.h"
#include "AbilitySystemBlueprintLibrary.h"


void ACountess_Character_Player::TimeLineProgress(float Value)
{
	//UE_LOG(Countess_Log, Warning, TEXT("Time Line Progress in %s. Value is %f"), TEXT(__FUNCTION__), Value);
	FVector NewLocation = FMath::Lerp(StartBackDashLoc, EndBackDashLoc, Value);
	GetCapsuleComponent()->SetRelativeLocation(NewLocation);
}

void ACountess_Character_Player::BeginBackDash()
{
	if(CurveFloat)
	{
		FOnTimelineFloatStatic TimeLineFloatStatic;
		TimeLineFloatStatic.BindUFunction(this, FName("TimeLineProgress"));
		BackDashTimeLine.AddInterpFloat(CurveFloat, TimeLineFloatStatic);
		//BackDashTimeLine.SetFloatCurve(CurveFloat, FName("BackDashMovementCurve"));
		StartBackDashLoc = GetCapsuleComponent()->GetComponentLocation();
		if (GetCapsuleComponent()->GetComponentRotation().Yaw >= 0)
		{
			EndBackDashLoc = StartBackDashLoc + BackDashLeftVector;
		}
		else
			EndBackDashLoc = StartBackDashLoc + BackDashRightVector;

		BackDashTimeLine.PlayFromStart();
	}
}


void ACountess_Character_Player::ElectroSparkOn()
{
	if (GetCharacterMovement()->IsFalling())
	{
		FloatingPawnMovement->Activate();
		GetCharacterMovement()->Deactivate();
	}
	Countess_PlayerController->DisableInput(Countess_PlayerController);
}


void ACountess_Character_Player::ElectroSparkOff()
{
	Countess_PlayerController->EnableInput(Countess_PlayerController);
	FloatingPawnMovement->Deactivate();
	GetCharacterMovement()->Activate();
}

bool ACountess_Character_Player::GiveAbilityOnOverlap_Implementation(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGive)
{
	//return Countess_PlayerState->AcquireAbilitiy(AbilityToGive);
	return Countess_PlayerController->Handle_Acquire_Ability_OnOverlap(AbilityToGive);
}

bool ACountess_Character_Player::GiveAbilityEndOverlap_Implementation()
{
	return Countess_PlayerController->Handle_Acquire_Ability_EndOverlap();
}

void ACountess_Character_Player::AbilityAcquiredInfoToGAGrantingActor(TSubclassOf<UCountess_GameplayAbility_Base> AbilityAcquiredClass, FSlateBrush AbilityIcon, float Cooldown)
{
	CountessAbilityAcquired_Interface_Delegate.Broadcast(AbilityAcquiredClass);
}

void ACountess_Character_Player::MontageToPlayOnGameBeginEnded(FTimerHandle& TimerHandle) const
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	if(!Countess_PlayerController->InputEnabled())
		Countess_PlayerController->EnableInput(Countess_PlayerController);
}

void ACountess_Character_Player::OnLeftSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		ACountess_Character_Base* OtherCharacter = Cast<ACountess_Character_Base>(OtherActor);
		//UE_LOG(Countess_Log, Warning, TEXT("OtherCharacter team id is %d and our team id is %d"), OtherCharacter->GetGenericTeamId().GetId(), this->GetGenericTeamId().GetId());
		if(OtherCharacter && OtherCharacter->GetGenericTeamId().GetId() != this->GetGenericTeamId().GetId())
		{
			//UE_LOG(Countess_Log, Warning, TEXT("From %s. Left Sword Overlapped with %s."), TEXT(__FUNCTION__), *OtherCharacter->GetFName().ToString());
			FGameplayEventData Payload;
			Payload.Instigator = this;
			Payload.Target = OtherCharacter;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this,FGameplayTag::RequestGameplayTag(FName("Ability.Primary.Damage")),Payload);
		}
	}
}

bool ACountess_Character_Player::GetIsDoubleJumping() const
{
	return bIsDoubleJumping;
}

void ACountess_Character_Player::SetIsDoubleJumping(bool bNewState)
{
	bIsDoubleJumping = bNewState;
}

void ACountess_Character_Player::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (SoundToPlayOnLanding)
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlayOnLanding, GetCapsuleComponent()->GetComponentLocation(), 0.5f);

	
	if (ParticleSystemOnLanding)
	{
		FVector Scale = FVector(FMath::Clamp(GetVelocity().Z * JumpScale, 0.5f, 3.f) / 2);
		UGameplayStatics::SpawnEmitterAttached(ParticleSystemOnLanding, FeetLocationArrowComponent, FName(NAME_None), FVector(ForceInit), FRotator::ZeroRotator, Scale);
	}
	SetIsDoubleJumping(false);
}

ACountess_Character_Player::ACountess_Character_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 700.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,-90.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, -1.f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.7f;
	GetCharacterMovement()->AirControl = 0.75f;
	GetCharacterMovement()->JumpZVelocity = 1800.f;
	GetCharacterMovement()->GroundFriction = 10.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	GetCharacterMovement()->ConstrainDirectionToPlane(FVector(0.f, 1.f, 0.f));

	//longer press higher jump
	this->JumpMaxHoldTime = 0.2f;

	//configure arrowcomponent for location of feet
	FeetLocationArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName("Feet Location"));
	FeetLocationArrowComponent->SetupAttachment(RootComponent);
	FeetLocationArrowComponent->SetRelativeLocation(FVector(0, 0, -96.f));

	FireballSpawnLocationArrowComponent = CreateDefaultSubobject<UArrowComponent>(FName("Fireball Spawn Point"));
	FireballSpawnLocationArrowComponent->SetupAttachment(RootComponent);
	FireballSpawnLocationArrowComponent->SetRelativeLocation(FVector(70.f, 0.f, 20.f));


	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(FName("Floating Pawn Movement"));

	CapsuleSwordCollisionLeft = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule Collision Sword Left"));
	CapsuleSwordCollisionLeft->SetCapsuleHalfHeight(55.f);
	CapsuleSwordCollisionLeft->SetCapsuleRadius(10.f);
	CapsuleSwordCollisionLeft->SetupAttachment(GetMesh(), FName("hand_lSocket"));
	CapsuleSwordCollisionLeft->SetRelativeLocation(FVector(21.4f,0.f,-35.6f));
	CapsuleSwordCollisionLeft->SetRelativeRotation(FRotator(10.f,0.f,0.f));
	CapsuleSwordCollisionLeft->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleSwordCollisionLeft->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	CapsuleSwordCollisionLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	CapsuleSwordCollisionRight = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule Collision Sword Right"));
	CapsuleSwordCollisionRight->SetCapsuleHalfHeight(55.f);
	CapsuleSwordCollisionRight->SetCapsuleRadius(10.f);
	CapsuleSwordCollisionRight->SetupAttachment(GetMesh(), FName("hand_rSocket"));
	CapsuleSwordCollisionRight->SetRelativeLocation(FVector(-21.4f,0.f,35.6f));
	CapsuleSwordCollisionRight->SetRelativeRotation(FRotator(10.f,0.f,0.f));
	CapsuleSwordCollisionRight->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleSwordCollisionRight->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	CapsuleSwordCollisionRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	/*Load VFX to be played on Player Landing*/
	//Load Soundwave
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundToPlayOnLandingObject(TEXT("SoundWave'/Game/ParagonCountess/Characters/Heroes/Vamp/Sounds/SoundWaves/Countess_Effort_Land_03.Countess_Effort_Land_03'"));
	if (SoundToPlayOnLandingObject.Succeeded())
		SoundToPlayOnLanding = SoundToPlayOnLandingObject.Object;

	//Load ParticleSystem
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemOnLandingObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/Master/P_MasterGrunt_Drag_Dust.P_MasterGrunt_Drag_Dust'"));
	if (ParticleSystemOnLandingObject.Succeeded())
		ParticleSystemOnLanding = ParticleSystemOnLandingObject.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontageToPlayOnGameBeginObject(TEXT("AnimMontage'/Game/MyProjectMain/Animations/LevelStart_Montage.LevelStart_Montage'"));
	if(AnimMontageToPlayOnGameBeginObject.Succeeded())
		MontageToPlayOnGameBegin = AnimMontageToPlayOnGameBeginObject.Object;

	//set emitter scaling value on landing
	JumpScale = -0.001f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	bIsDoubleJumping = false;

	BackDashLeftVector= FVector(-350.f, 0.f, 0.f);
	BackDashRightVector = FVector(350.f, 0.f, 0.f);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> BackDashCurveFloatObject(TEXT("CurveFloat'/Game/MyProjectMain/Blueprints/Characters/Abilities/BackDashAbility/BackDashMovementCurve.BackDashMovementCurve'"));
	if (BackDashCurveFloatObject.Succeeded())
		CurveFloat = BackDashCurveFloatObject.Object;
	else
		UE_LOG(Countess_Log, Warning, TEXT("Can't find Back Dash CurveFloat in %s. Check if it is moved."), TEXT(__FUNCTION__));

	static ConstructorHelpers::FObjectFinder<USoundCue> BeginPlaySoundCueObject(TEXT("SoundCue'/Game/MyProjectMain/Audio/Countess_BeginPlay_Cue.Countess_BeginPlay_Cue'"));
	if(BeginPlaySoundCueObject.Succeeded())
		BeginPlaySoundCue = BeginPlaySoundCueObject.Object;
	else
		UE_LOG(Countess_Log, Warning, TEXT("Can't find BeingPlay Sound Cue in %s. Check if it is moved."), TEXT(__FUNCTION__));
	
}

void ACountess_Character_Player::BeginPlay()
{
	Super::BeginPlay();
	Countess_PlayerState->Countess_Ability_Acquired_Delegate.AddDynamic(this, &ACountess_Character_Player::AbilityAcquiredInfoToGAGrantingActor);
	if(MontageToPlayOnGameBegin)
	{
		const float MontageLength = GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlayOnGameBegin);
		if(Countess_PlayerController)
		{
			Countess_PlayerController->DisableInput(Countess_PlayerController);
			FTimerHandle MontagePlayHandle;
			FTimerDelegate MontagePlayDelegate;
			MontagePlayDelegate.BindUFunction(this, FName("MontageToPlayOnGameBeginEnded"), MontagePlayHandle);
			GetWorldTimerManager().SetTimer(MontagePlayHandle,MontagePlayDelegate,MontageLength,false);
			if(BeginPlaySoundCue)
				UGameplayStatics::PlaySound2D(this,Cast<USoundBase>(BeginPlaySoundCue));
		}
	}
	/*Player Team Id. Useful for identifying enemy/player since both derive from same character base class*/
	PlayerTeamId = FGenericTeamId(0);
	this->SetGenericTeamId(PlayerTeamId);

	CapsuleSwordCollisionLeft->OnComponentBeginOverlap.AddDynamic(this, &ACountess_Character_Player::OnLeftSwordBeginOverlap);

}

void ACountess_Character_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	Countess_PlayerState = GetPlayerState<ACountess_PlayerState>();
	if (Countess_PlayerState)
	{
		/*Configure our AbiltiySystemComponent and AttributeSet*/
		AbilitySystemComponent = Cast<UCountess_AbilitySystemComponent>(Countess_PlayerState->GetAbilitySystemComponent());
		AttributeSet = Cast<UCountess_AttributeSet_Base>(Countess_PlayerState->GetAttributeSet());
	}

	Countess_PlayerController = Cast<ACountess_PlayerController>(NewController);
}


bool ACountess_Character_Player::IsAlive() const
{
	return Countess_PlayerState->GetCurrentHealth() > 0.f;
}

void ACountess_Character_Player::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	BackDashTimeLine.TickTimeline(DeltaSeconds);
}

FGenericTeamId ACountess_Character_Player::GetGenericTeamId() const
{
	return PlayerTeamId;
}

int32 ACountess_Character_Player::GetCharacterLevel() const
{
	return Countess_PlayerState->GetPlayerLevel();
}

