// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "Countess_Actor_ElectroSpark.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UParticleSystem;

/*Our ElectroSpark Actor*/
UCLASS()
class PROJECT_COUNTESS_API ACountess_Actor_ElectroSpark : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACountess_Actor_ElectroSpark();

	UPROPERTY()
	USphereComponent* ElectroSparkCollisionComponent;

	UPROPERTY()
	UParticleSystemComponent* ElectroSparkVFX_Component_Startup;
	UPROPERTY()
	UParticleSystemComponent* ElectroSparkVFX_Component_Right_01;
	UPROPERTY()
	UParticleSystemComponent* ElectroSparkVFX_Component_Right_02;
	UPROPERTY()
	UParticleSystemComponent* ElectroSparkVFX_Component_Left_01;
	UPROPERTY()
	UParticleSystemComponent* ElectroSparkVFX_Component_Left_02;

	UPROPERTY()
	UParticleSystem* ElectroSparkVFX_Startup;
	UPROPERTY()
	UParticleSystem* ElectroSparkVFX_Main;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
