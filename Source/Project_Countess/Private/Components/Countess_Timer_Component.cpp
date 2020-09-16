// Free to play. By SoNa Games.


#include "Components/Countess_Timer_Component.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCountess_Timer_Component::UCountess_Timer_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bStartLerp = false;
	LerpedValue = 0;
	ComponentDestroyDelay = 1.f;
}


void UCountess_Timer_Component::StartLerp(const float StartValue, const float EndValue)
{
	LerpStartValue = StartValue;
	LerpEndValue = EndValue;
	bStartLerp = true;
}

void UCountess_Timer_Component::DestroyCountessTimerComponent()
{
	GetWorld()->GetTimerManager().ClearTimer(CountessTimerComponentHandle);
	CountessTimerDelegate.RemoveAll(this);
	this->DestroyComponent();
	//UE_LOG(Countess_Log, Warning, TEXT("From %s: Destroyed %s."), TEXT(__FUNCTION__), *this->GetFName().ToString());
}

// Called when the game starts
void UCountess_Timer_Component::BeginPlay()
{
	Super::BeginPlay();
	CountessTimerComponentDestroyDelegate = FTimerDelegate::CreateUObject(this, &UCountess_Timer_Component::DestroyCountessTimerComponent);
}


// Called every frame
void UCountess_Timer_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bStartLerp)
	{
		LerpedValue += LerpStartValue + DeltaTime / (UGameplayStatics::GetGlobalTimeDilation(this));
		LerpedValue = FMath::Clamp<float>(LerpedValue, LerpStartValue, LerpEndValue);
		CountessTimerDelegate.Broadcast((LerpedValue - LerpStartValue) / (LerpEndValue - LerpStartValue));
		if (LerpedValue == LerpEndValue)
		{
			GetWorld()->GetTimerManager().SetTimer(CountessTimerComponentHandle, CountessTimerComponentDestroyDelegate, ComponentDestroyDelay * (UGameplayStatics::GetGlobalTimeDilation(this)), false);
			bStartLerp = false;
		}
	}
}

