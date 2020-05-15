// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Characters/Project_CountessCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Countess_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Particles/ParticleSystem.h"


UAbilitySystemComponent* AProject_CountessCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

bool AProject_CountessCharacter::GiveAbilityOnOverlap_Implementation(TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGive)
{
	return Countess_PlayerState->AcquireAbilitiy(AbilityToGive);
}

void AProject_CountessCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (SoundToPlayOnLanding)
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlayOnLanding, GetCapsuleComponent()->GetComponentLocation(), 0.5f);

	
	if (ParticleSystemOnLanding)
	{
		FVector Scale = FVector(FMath::Clamp(GetVelocity().Z * JumpScale, 0.5f, 3.f) / 2);
		UGameplayStatics::SpawnEmitterAttached(ParticleSystemOnLanding, FeetLocationArrowComponent, FName(NAME_None), FVector(ForceInit), FRotator::ZeroRotator, Scale);
	}
	
}

AProject_CountessCharacter::AProject_CountessCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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

	/*Load VFX to be played on Player Landing*/
	//Load Soundwave
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundToPlayOnLandingObject(TEXT("SoundWave'/Game/ParagonCountess/Characters/Heroes/Vamp/Sounds/SoundWaves/Countess_Effort_Land_03.Countess_Effort_Land_03'"));
	if (SoundToPlayOnLandingObject.Succeeded())
		SoundToPlayOnLanding = SoundToPlayOnLandingObject.Object;

	//Load ParticleSystem
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemOnLandingObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/Master/P_MasterGrunt_Drag_Dust.P_MasterGrunt_Drag_Dust'"));
	if (ParticleSystemOnLandingObject.Succeeded())
		ParticleSystemOnLanding = ParticleSystemOnLandingObject.Object;

	//set emitter scaling value on landing
	JumpScale = -0.001f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

void AProject_CountessCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProject_CountessCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	Countess_PlayerState = GetPlayerState<ACountess_PlayerState>();
	if (Countess_PlayerState)
	{
		/*Configure our AbiltiySystemComponent and AttributeSet*/
		AbilitySystemComp = Countess_PlayerState->GetAbilitySystemComponent();
		AttributeSet = Countess_PlayerState->GetAttributeSet();
	}
}


