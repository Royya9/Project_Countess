// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "GameFramework/Actor.h"
#include "Countess_LensOfTruth_Platform.generated.h"

class UStaticMeshComponent;

UCLASS()
class PROJECT_COUNTESS_API ACountess_LensOfTruth_Platform : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;
	
public:	
	// Sets default values for this actor's properties
	ACountess_LensOfTruth_Platform();

	void ToggleStaticMesh(const bool bLensOfTruthActive = false) const;

};
