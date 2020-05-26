// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/Actor.h"
#include "Countess_GAGrantingActor_Base.generated.h"

class UBoxComponent;
class UCountess_GameplayAbility_Base;
class USoundBase;
class UUserWidget;

UCLASS(Abstract)
class PROJECT_COUNTESS_API ACountess_GAGrantingActor_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACountess_GAGrantingActor_Base();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<UBoxComponent> BoxComponent;

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbility, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGrant;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AbilityAcquired();
	UFUNCTION()
	virtual void AbilityAcquired_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	bool bAcquired_Ability;
};
