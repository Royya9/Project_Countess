// Free to play. By SoNa Games.

#pragma once

#include "CoreMinimal.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayAbility, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCountess_GameplayAbility_Base> AbilityToGrant;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Broadcast)
	bool Execute_GiveAbilityOnOverlap(TSubclassOf<UCountess_GameplayAbility_Base> Ability);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Base, meta = (AllowPrivateAccess = "true"))
	FText InteractiveMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<USoundBase> SoundToPlayOnInteract;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
