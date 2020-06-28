// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "Countess_Actor_Fireball.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UParticleSystem;
class UParticleSystemComponent;
class UStaticMesh;
class UMaterialInterface;
class UProjectileMovementComponent;
class USoundCue;
class UAIPerceptionStimuliSourceComponent;

/*Our Fireball Actor*/
UCLASS()
class PROJECT_COUNTESS_API ACountess_Actor_Fireball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACountess_Actor_Fireball();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent* FireballCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* FireballMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* FireballVFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* FireballVFXComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY()
	UStaticMesh* FireballMesh;

	UPROPERTY()
	UMaterialInterface* FireballMaterial;

	UPROPERTY()
	UProjectileMovementComponent* FireballProjectileMovementComponent;

	UPROPERTY()
	UParticleSystem* FireballImpactVFX;

	UPROPERTY()
	USoundCue* FireballImpactSoundCue;

	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnFireballImpact(const FVector& ImpactLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
