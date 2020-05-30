// Free to play. By SoNa Games.


#include "Actors/Countess_Actor_Fireball.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACountess_Actor_Fireball::ACountess_Actor_Fireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FireballCollision = CreateDefaultSubobject<USphereComponent>(FName("Collision Component"));
	FireballCollision->SetSphereRadius(20.f);
	FireballCollision->SetCollisionObjectType(ECC_GameTraceChannel1);
	FireballCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
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
}

// Called when the game starts or when spawned
void ACountess_Actor_Fireball::BeginPlay()
{
	Super::BeginPlay();
	
}



