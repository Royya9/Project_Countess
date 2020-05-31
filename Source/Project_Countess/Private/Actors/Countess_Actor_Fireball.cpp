// Free to play. By SoNa Games.


#include "Actors/Countess_Actor_Fireball.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/Countess_Interface_Actor.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Camera//Countess_Fireball_CameraShake.h"

// Sets default values
ACountess_Actor_Fireball::ACountess_Actor_Fireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FireballCollision = CreateDefaultSubobject<USphereComponent>(FName("Collision Component"));
	FireballCollision->SetSphereRadius(20.f);
	FireballCollision->SetCollisionObjectType(ECC_GameTraceChannel1);
	FireballCollision->SetCollisionResponseToAllChannels(ECR_Block);
	FireballCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	RootComponent = FireballCollision;

	FireballMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh Component"));
	FireballMeshComponent->SetupAttachment(FireballCollision);
	FireballMeshComponent->SetRelativeLocation(FVector(35.f, 0, 0));
	FireballMeshComponent->SetRelativeScale3D(FVector(3.f));
	FireballMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FireballVFXComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("Fireball VFX Component"));
	FireballVFXComponent->SetupAttachment(FireballCollision);
	FireballVFXComponent->SetRelativeLocation(FVector(5.f, 0, 0));
	FireballVFXComponent->SetRelativeScale3D(FVector(2.5f));

	FireballProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Fireball Projectile Movement Component"));
	FireballProjectileMovementComponent->InitialSpeed = 1500.f;
	FireballProjectileMovementComponent->MaxSpeed = 1500.f;
	FireballProjectileMovementComponent->ProjectileGravityScale = 0.f;

	Range = 3000.f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireballVFXObject(TEXT("ParticleSystem'/Game/MyProjectMain/Particles/P_FireBall_Strong.P_FireBall_Strong'"));
	if (FireballVFXObject.Succeeded())
	{
		FireballVFX = FireballVFXObject.Object;
		FireballVFXComponent->SetTemplate(FireballVFX);
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("Fireball VFX not found for ActorFireball. Check if it is moved. from %s"), TEXT(__FUNCTION__));

	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FireballMeshObject(TEXT("StaticMesh'/Game/MyProjectMain/Meshes/SM_FireBall.SM_FireBall'"));
	if (FireballMeshObject.Succeeded())
	{
		FireballMesh = FireballMeshObject.Object;
		FireballMeshComponent->SetStaticMesh(FireballMesh);
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("Fireball Mesh not found for ActorFireball. Check if it is moved. from %s"), TEXT(__FUNCTION__));


	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FireballMaterialObject(TEXT("Material'/Game/MyProjectMain/Materials/M_FireBlast_Add_FireBall_MeshEmit.M_FireBlast_Add_FireBall_MeshEmit'"));
	if (FireballMaterialObject.Succeeded())
	{
		FireballMaterial = FireballMaterialObject.Object;
		FireballMesh->SetMaterial(0, FireballMaterial);
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("Fireball Material not found for ActorFireball. Check if it is moved. from %s"), TEXT(__FUNCTION__));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireballImpactVFXObject(TEXT("ParticleSystem'/Game/MyProjectMain/Particles/P_FireBall_Strong_MuzzFlash_01.P_FireBall_Strong_MuzzFlash_01'"));
	if (FireballImpactVFXObject.Succeeded())
	{
		FireballImpactVFX = FireballImpactVFXObject.Object;
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("Fireball Impact VFX not found for ActorFireball. Check if it is moved. from %s"), TEXT(__FUNCTION__));

	static ConstructorHelpers::FObjectFinder<USoundCue> FireballImpactSoundCueObject(TEXT("SoundCue'/Game/MyProjectMain/Audio/Explosion_Cue.Explosion_Cue'"));
	if (FireballImpactSoundCueObject.Succeeded())
	{
		FireballImpactSoundCue = FireballImpactSoundCueObject.Object;
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("Fireball Impact SoundCue not found for ActorFireball. Check if it is moved. from %s"), TEXT(__FUNCTION__));
}

void ACountess_Actor_Fireball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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
				ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
				if (FireballImpactVFX)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireballImpactVFX, Hit.Location);
				if (FireballImpactSoundCue)
					UGameplayStatics::PlaySoundAtLocation(this, FireballImpactSoundCue, Hit.Location);

				UGameplayStatics::PlayWorldCameraShake(this, UCountess_Fireball_CameraShake::StaticClass(), Hit.Location, 0, 1000.f);
				Destroy();
			}
		}
	}
}

// Called when the game starts or when spawned
void ACountess_Actor_Fireball::BeginPlay()
{
	Super::BeginPlay();
	FireballCollision->OnComponentHit.AddDynamic(this, &ACountess_Actor_Fireball::OnHit);
	SetLifeSpan(Range / FireballProjectileMovementComponent->InitialSpeed);
}



