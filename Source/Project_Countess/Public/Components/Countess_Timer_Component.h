// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Components/ActorComponent.h"
#include "Countess_Timer_Component.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCountessTimerDelegate, float, StartValue, float, EndValue, float, LerpedValue);
DECLARE_MULTICAST_DELEGATE_OneParam(FCountessTimerDelegate, float);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountessTimerRemainingAbsValueDelegate, float, TimeRemaining);
// Delegate fired when the timer duration is over
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCountessTimerCompletedDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_COUNTESS_API UCountess_Timer_Component : public UActorComponent
{
    GENERATED_BODY()

    public:	
    // Sets default values for this component's properties
    UCountess_Timer_Component();

    FCountessTimerDelegate CountessTimerDelegate;

    // Delegate fired when the timer duration is over
    FCountessTimerCompletedDelegate CountessTimerCompletedDelegate;

	// Delegate which fires continuously giving the cooldown time remaining in absolute values (like 5.4 sec remaining instead of %)
	FCountessTimerRemainingAbsValueDelegate CountessTimerRemainingAbsValueDelegate;

    void StartLerp(const float StartValue, const float EndValue);

    void DestroyCountessTimerComponent();

    protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    public:	
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    bool bIsAbilityTimeSlow;

    private:

    FTimerHandle CountessTimerComponentHandle;
    FTimerDelegate CountessTimerComponentDestroyDelegate;

    float LerpStartValue;
    float LerpEndValue;
    float LerpedValue;

    float ComponentDestroyDelay;

    bool bStartLerp;
		
};