// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "Countess_Actor_ArcBlast.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UParticleSystem;
class UParticleSystemComponent;
class UStaticMesh;
class UMaterialInterface;
class UProjectileMovementComponent;
class USoundCue;
class UAIPerceptionStimuliSourceComponent;

/* Our ArcticBlast Actor*/
UCLASS()
class PROJECT_COUNTESS_API ACountess_Actor_ArcBlast : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACountess_Actor_ArcBlast();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* ArcticBlastCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ArcticBlastMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ArcticBlastVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* ArcticBlastVFXComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY()
	UStaticMesh* ArcticBlastMesh;

	UPROPERTY()
	UMaterialInterface* ArcticBlastMaterial;

	UPROPERTY()
	UProjectileMovementComponent* ArcticBlastProjectileMovementComponent;

	UPROPERTY()
	UParticleSystem* ArcticBlastImpactVFX;

	UPROPERTY()
	USoundCue* ArcticBlastImpactSoundCue;

	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnArcticBlastImpact(const FVector& ImpactLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};
