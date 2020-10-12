// Free to play. By SoNa Games.


#include "Actors/Countess_Actor_ArcBlast.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/Countess_Interface_Actor.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Camera/Countess_Fireball_CameraShake.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ACountess_Actor_ArcBlast::ACountess_Actor_ArcBlast()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArcticBlastCollision = CreateDefaultSubobject<USphereComponent>(FName("Collision Component"));
	ArcticBlastCollision->SetSphereRadius(20.f);
	ArcticBlastCollision->SetCollisionObjectType(ECC_GameTraceChannel1);
	ArcticBlastCollision->SetCollisionResponseToAllChannels(ECR_Block);
	ArcticBlastCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	RootComponent = ArcticBlastCollision;

	ArcticBlastMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh Component"));
	ArcticBlastMeshComponent->SetupAttachment(ArcticBlastCollision);
	ArcticBlastMeshComponent->SetRelativeLocation(FVector(35.f, 0, 0));
	ArcticBlastMeshComponent->SetRelativeScale3D(FVector(3.f));
	ArcticBlastMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArcticBlastVFXComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("ArcticBlast VFX Component"));
	ArcticBlastVFXComponent->SetupAttachment(ArcticBlastCollision);
	ArcticBlastVFXComponent->SetRelativeLocation(FVector(5.f, 0, 0));
	ArcticBlastVFXComponent->SetRelativeScale3D(FVector(2.5f));

	ArcticBlastProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ArcticBlast Projectile Movement Component"));
	ArcticBlastProjectileMovementComponent->InitialSpeed = 2000.f;
	ArcticBlastProjectileMovementComponent->MaxSpeed = 2000.f;
	ArcticBlastProjectileMovementComponent->ProjectileGravityScale = 0.f;

	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("AI Stimuli Scource Component"));
	StimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());

	Range = 4000.f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ArcticBlastVFXObject(TEXT("ParticleSystem'/Game/MyProjectMain/Particles/P_ArcticBlast_Strong.P_ArcticBlast_Strong'"));
	if (ArcticBlastVFXObject.Succeeded())
	{
		ArcticBlastVFX = ArcticBlastVFXObject.Object;
		ArcticBlastVFXComponent->SetTemplate(ArcticBlastVFX);
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("ArcticBlast VFX not found for Actor ArcticBlast. Check if it is moved. from %s"), TEXT(__FUNCTION__));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArcticBlastMeshObject(TEXT("StaticMesh'/Game/MyProjectMain/Meshes/SM_ArcBlast.SM_ArcBlast'"));
	if (ArcticBlastMeshObject.Succeeded())
	{
		ArcticBlastMesh = ArcticBlastMeshObject.Object;
		ArcticBlastMeshComponent->SetStaticMesh(ArcticBlastMesh);
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("ArcticBlast Mesh not found for Actor ArcticBlast. Check if it is moved. from %s"), TEXT(__FUNCTION__));


	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ArcticBlastMaterialObject(TEXT("Material'/Game/MyProjectMain/Materials/M_ArcBlast_Add_ArcBall_MeshEmit.M_ArcBlast_Add_ArcBall_MeshEmit'"));
	if (ArcticBlastMaterialObject.Succeeded())
	{
		ArcticBlastMaterial = ArcticBlastMaterialObject.Object;
		ArcticBlastMesh->SetMaterial(0, ArcticBlastMaterial);
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("ArcticBlast Material not found for Actor ArcticBlast. Check if it is moved. from %s"), TEXT(__FUNCTION__));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ArcticBlastImpactVFXObject(TEXT("ParticleSystem'/Game/MyProjectMain/Particles/P_ArcticBlast_Strong_MuzzFlash_01.P_ArcticBlast_Strong_MuzzFlash_01'"));
	if (ArcticBlastImpactVFXObject.Succeeded())
	{
		ArcticBlastImpactVFX = ArcticBlastImpactVFXObject.Object;
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("ArcticBlast Impact VFX not found for Actor ArcticBlast. Check if it is moved. from %s"), TEXT(__FUNCTION__));

	static ConstructorHelpers::FObjectFinder<USoundCue> ArcticBlastImpactSoundCueObject(TEXT("SoundCue'/Game/MyProjectMain/Audio/Freeze_Sound_Effect_Cue.Freeze_Sound_Effect_Cue'"));
	if (ArcticBlastImpactSoundCueObject.Succeeded())
	{
		ArcticBlastImpactSoundCue = ArcticBlastImpactSoundCueObject.Object;
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("ArcticBlast Impact SoundCue not found for Actor ArcticBlast. Check if it is moved. from %s"), TEXT(__FUNCTION__));
}

void ACountess_Actor_ArcBlast::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ICountess_Interface_Actor* PlayerCharacter_Interface = Cast<ICountess_Interface_Actor>(OtherActor);
		if (PlayerCharacter_Interface)
		{
			//UE_LOG(Countess_Log, Warning, TEXT("From %s. Our Projectile Hit %s."), TEXT(__FUNCTION__), *GetDebugName(OtherActor));
			UCountess_AbilitySystemComponent* ASC = Cast<UCountess_AbilitySystemComponent>(PlayerCharacter_Interface->GetASC());
			if (ASC)
			{
				//UE_LOG(Countess_Log, Warning, TEXT("From %s. Hit Actor ASC is %s"), TEXT(__FUNCTION__), *ASC->GetFName().ToString());
				DamageEffectSpecHandle.Data->Period *= UGameplayStatics::GetGlobalTimeDilation(this);
				ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
				OnArcticBlastImpact(SweepResult.Location);
			}
		}
	}
}

void ACountess_Actor_ArcBlast::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
		OnArcticBlastImpact(Hit.Location);
}

void ACountess_Actor_ArcBlast::OnArcticBlastImpact(const FVector& ImpactLocation)
{
	if (ArcticBlastImpactVFX)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ArcticBlastImpactVFX, ImpactLocation);
	if (ArcticBlastImpactSoundCue)
		UGameplayStatics::PlaySoundAtLocation(this, ArcticBlastImpactSoundCue, ImpactLocation);

	UGameplayStatics::PlayWorldCameraShake(this, UCountess_Fireball_CameraShake::StaticClass(), ImpactLocation, 0, 1000.f);
	Destroy();
}

// Called when the game starts or when spawned
void ACountess_Actor_ArcBlast::BeginPlay()
{
	Super::BeginPlay();
	ArcticBlastCollision->OnComponentBeginOverlap.AddDynamic(this, &ACountess_Actor_ArcBlast::OnOverlap);
	ArcticBlastCollision->OnComponentHit.AddDynamic(this, &ACountess_Actor_ArcBlast::OnHit);
	SetLifeSpan(Range/ArcticBlastProjectileMovementComponent->InitialSpeed);
}

void ACountess_Actor_ArcBlast::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	this->CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(this);
}


