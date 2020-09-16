// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/TimelineComponent.h"
#include "Countess_CameraManager.generated.h"

class UCurveFloat;
class UCameraComponent;

/**
 *  Camera Manager for Player
 */
UCLASS()
class PROJECT_COUNTESS_API ACountess_CameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

    ACountess_CameraManager();

    virtual void UpdateCamera(float DeltaTime) override;

    void SetVignetteAndFOV(float Vignette, float NewFOV);

    void ResetVignetteAndFOV();

    void SetVignetteAndBlur(const float& NewVignette, const float& NewBlur, const float& StartTime, const float& EndTime);

    void ResetVignetteAndBlur(const float& StartTime, const float& EndTime);

    UCameraComponent* GetPlayerCameraComponentFromViewTarget();

    UFUNCTION()
    void TimelineProgress(float Value);

    UFUNCTION()
    void CountessTimerProgress(float LerpValue);

    FTimeline CameraZoomTimeline;

    UCurveFloat* CameraZoomCurveFloat;

private:

    float CameraOldFOV;

    float CameraNewFOV;

    float CameraOldVignette;

    float CameraNewVignette;

    float CameraOldBlur;

    float CameraNewBlur;
};
