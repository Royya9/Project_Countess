// Free to play. By SoNa Games.


#include "Camera/Countess_CameraManager.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/Countess_Timer_Component.h"

ACountess_CameraManager::ACountess_CameraManager()
{
    static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFloatHelper(TEXT("'/Game/MyProjectMain/Blueprints/Camera/CameraZoomOnLensAbility'"));
    if (CurveFloatHelper.Succeeded())
        CameraZoomCurveFloat = CurveFloatHelper.Object;
    else
        UE_LOG(Countess_Log, Error, TEXT("Camera Zoom CurveFloat not found in %s. Check if it is moved."), TEXT(__FUNCTION__));

}

void ACountess_CameraManager::UpdateCamera(float DeltaTime)
{
    Super::UpdateCamera(DeltaTime);
    CameraZoomTimeline.TickTimeline(DeltaTime);
}

void ACountess_CameraManager::SetVignetteAndFOV(float Vignette, float NewFOV)
{
    if (CameraZoomCurveFloat)
    {
		FOnTimelineFloatStatic TimeLineFloatStatic;
		TimeLineFloatStatic.BindUFunction(this, FName("TimeLineProgress"));
        CameraZoomTimeline.AddInterpFloat(CameraZoomCurveFloat, TimeLineFloatStatic);

        CameraOldFOV = this->GetFOVAngle();
        CameraNewFOV = NewFOV;

        // Get the current(old) vignette intensity from Player Camera Component (which is given by view target which inturn is set in PlayerController.OnPossess).
        UCameraComponent* PlayerCameraComponent = GetPlayerCameraComponentFromViewTarget();
		if (PlayerCameraComponent)
		{
			CameraOldVignette = PlayerCameraComponent->PostProcessSettings.VignetteIntensity;
			CameraNewVignette = Vignette;
		}

        CameraZoomTimeline.PlayFromStart();
    }
}


void ACountess_CameraManager::ResetVignetteAndFOV()
{
	if (CameraZoomCurveFloat)
	{
		FOnTimelineFloatStatic TimeLineFloatStatic;
		TimeLineFloatStatic.BindUFunction(this, FName("TimeLineProgress"));
		CameraZoomTimeline.AddInterpFloat(CameraZoomCurveFloat, TimeLineFloatStatic);

        CameraNewFOV = CameraOldFOV;
        CameraOldFOV = this->GetFOVAngle();

		// Get the current(old) vignette intensity from Player Camera Component (which is given by view target which inturn is set in PlayerController.OnPossess).
        UCameraComponent* PlayerCameraComponent = GetPlayerCameraComponentFromViewTarget();
		if (PlayerCameraComponent)
		{
			CameraNewVignette = CameraOldVignette;
			CameraOldVignette = PlayerCameraComponent->PostProcessSettings.VignetteIntensity;
		}

		CameraZoomTimeline.PlayFromStart();
	}
}


void ACountess_CameraManager::SetVignetteAndBlur(const float& NewVignette, const float& NewBlur, const float& StartTime, const float& EndTime)
{
	UCameraComponent* PlayerCameraComponent = GetPlayerCameraComponentFromViewTarget();
	if (PlayerCameraComponent)
	{
		CameraOldVignette = PlayerCameraComponent->PostProcessSettings.VignetteIntensity;
		CameraNewVignette = NewVignette;

		CameraOldBlur = PlayerCameraComponent->PostProcessSettings.DepthOfFieldFarBlurSize;
		CameraNewBlur = NewBlur;

		UCountess_Timer_Component* CountessTimerComponent = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
		CountessTimerComponent->RegisterComponent();
		CountessTimerComponent->CountessTimerDelegate.AddDynamic(this, &ACountess_CameraManager::CountessTimerProgress);
		CountessTimerComponent->StartLerp(StartTime, EndTime);
	}
}


void ACountess_CameraManager::ResetVignetteAndBlur(const float& StartTime, const float& EndTime)
{
	// Get the current(old) vignette intensity from Player Camera Component (which is given by view target which inturn is set in PlayerController.OnPossess).
	UCameraComponent* PlayerCameraComponent = GetPlayerCameraComponentFromViewTarget();
	if (PlayerCameraComponent)
	{
		CameraNewVignette = CameraOldVignette;
		CameraOldVignette = PlayerCameraComponent->PostProcessSettings.VignetteIntensity;

		CameraNewBlur = CameraOldBlur;
		CameraOldBlur = PlayerCameraComponent->PostProcessSettings.DepthOfFieldFarBlurSize;

		UCountess_Timer_Component* CountessTimerComponent = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
		CountessTimerComponent->RegisterComponent();
		CountessTimerComponent->CountessTimerDelegate.AddDynamic(this, &ACountess_CameraManager::CountessTimerProgress);
		CountessTimerComponent->StartLerp(StartTime, EndTime);
	}
}

UCameraComponent* ACountess_CameraManager::GetPlayerCameraComponentFromViewTarget()
{
	AActor* _ViewTarget = this->GetViewTarget();
	if (_ViewTarget)
	{
		UCameraComponent* PlayerCameraComponent = Cast<UCameraComponent>(_ViewTarget->GetComponentByClass(UCameraComponent::StaticClass()));
		if (PlayerCameraComponent)
			return PlayerCameraComponent;
	}
	return nullptr;
}

void ACountess_CameraManager::TimelineProgress(float Value)
{
    float ProgressFOV = FMath::LerpStable<float>(CameraOldFOV, CameraNewFOV, Value);
    float ProgressVignette = FMath::LerpStable<float>(CameraOldVignette, CameraNewVignette, Value);
    this->SetFOV(ProgressFOV);
    
	UCameraComponent* PlayerCameraComponent = GetPlayerCameraComponentFromViewTarget();
	if (PlayerCameraComponent)
	{
		PlayerCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
		PlayerCameraComponent->PostProcessSettings.VignetteIntensity = ProgressVignette;
	}
}

void ACountess_CameraManager::CountessTimerProgress(float StartTime, float EndTime, float LerpValue)
{
	float ProgressVignette = FMath::LerpStable<float>(CameraOldVignette, CameraNewVignette, LerpValue);
	float ProgressBlur = FMath::LerpStable<float>(CameraOldBlur, CameraNewBlur, LerpValue); 

	UCameraComponent* PlayerCameraComponent = GetPlayerCameraComponentFromViewTarget();
	if (PlayerCameraComponent)
	{
		PlayerCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
		PlayerCameraComponent->PostProcessSettings.VignetteIntensity = ProgressVignette;

		PlayerCameraComponent->PostProcessSettings.DepthOfFieldMethod_DEPRECATED = EDepthOfFieldMethod::DOFM_Gaussian;
		PlayerCameraComponent->PostProcessSettings.bOverride_DepthOfFieldNearBlurSize = true;
		PlayerCameraComponent->PostProcessSettings.bOverride_DepthOfFieldFarBlurSize = true;
		PlayerCameraComponent->PostProcessSettings.DepthOfFieldNearBlurSize = ProgressBlur;
		PlayerCameraComponent->PostProcessSettings.DepthOfFieldFarBlurSize = ProgressBlur;

	}
}
