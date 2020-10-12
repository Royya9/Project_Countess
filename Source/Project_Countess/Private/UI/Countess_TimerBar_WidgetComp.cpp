// Free to play. By SoNa Games.


#include "UI/Countess_TimerBar_WidgetComp.h"
#include "UI/Countess_TimerBar_Widget.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Countess_Timer_Component.h"

UCountess_TimerBar_WidgetComp::UCountess_TimerBar_WidgetComp()
{
    this->SetWidgetSpace(EWidgetSpace::Screen);

    static ConstructorHelpers::FClassFinder<UCountess_TimerBar_Widget> TimerBarWidgetClass_Helper(TEXT("'/Game/MyProjectMain/Widgets/BP_TimerBar'"));
    if (TimerBarWidgetClass_Helper.Succeeded())
    {
        TimerBarWidgetClass = TimerBarWidgetClass_Helper.Class;
        this->SetWidgetClass(TimerBarWidgetClass);
    }
    else
        UE_LOG(Countess_Log, Warning, TEXT("Can't find DamageNumberWidgetBP in %s. Check if it is moved."), TEXT(__FUNCTION__));

    ComponentId = -1;
    bIsAbilityTimeSlow = false;
}

void UCountess_TimerBar_WidgetComp::SetAbilityTextAndDuration_Implementation(const FText& AbilityText,
    const float Duration, UCountess_Timer_Component* Timer_Component)
{
    UCountess_TimerBar_Widget* TimerBar_Widget = Cast<UCountess_TimerBar_Widget>(this->GetUserWidgetObject());
    if(TimerBar_Widget)
    {
        TimerBar_Widget->bIsAbilityTimeSlow = bIsAbilityTimeSlow;
        TimerBar_Widget->SetTimerBarWidgetText(AbilityText);
        TimerBar_Widget->SetTimerBarDuration(Duration);
    }
    //FTimerDelegate TimerBarWidgetDelegate = FTimerDelegate::CreateUObject(this, &UCountess_TimerBar_WidgetComp::DestroyTimerBarWidgetComponent);
    //GetWorld()->GetTimerManager().SetTimer(TimerBarWidgetHandle, TimerBarWidgetDelegate, Duration * (UGameplayStatics::GetGlobalTimeDilation(this)), false);

    if(!Timer_Component)
    {
        UCountess_Timer_Component* TimerComponent_Duration = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
        TimerComponent_Duration->RegisterComponent();
        TimerComponent_Duration->bIsAbilityTimeSlow = bIsAbilityTimeSlow;
        TimerComponent_Duration->CountessTimerCompletedDelegate.AddDynamic(this, &UCountess_TimerBar_WidgetComp::DestroyTimerBarWidgetComponent);
        TimerComponent_Duration->CountessTimerDelegate.AddUObject(TimerBar_Widget, &UCountess_TimerBar_Widget::SetTimerBarPercentage);
        TimerComponent_Duration->StartLerp(0, Duration);
        
    }
    else
    {
        Timer_Component->CountessTimerDelegate.AddUObject(TimerBar_Widget, &UCountess_TimerBar_Widget::SetTimerBarPercentage);
        Timer_Component->CountessTimerCompletedDelegate.AddDynamic(this, &UCountess_TimerBar_WidgetComp::DestroyTimerBarWidgetComponent);
    }
}
/*
void UCountess_TimerBar_WidgetComp::SetAbilityTextAndDuration_Implementation(const FText& AbilityText, const float Duration, UCountess_Timer_Component* Timer_Component)
{
    UCountess_TimerBar_Widget* TimerBar_Widget = Cast<UCountess_TimerBar_Widget>(this->GetUserWidgetObject());
    if(TimerBar_Widget)
    {
        TimerBar_Widget->bIsAbilityTimeSlow = bIsAbilityTimeSlow;
        TimerBar_Widget->SetTimerBarWidgetText(AbilityText);
        TimerBar_Widget->SetTimerBarDuration(Duration);
    }
    //FTimerDelegate TimerBarWidgetDelegate = FTimerDelegate::CreateUObject(this, &UCountess_TimerBar_WidgetComp::DestroyTimerBarWidgetComponent);
    //GetWorld()->GetTimerManager().SetTimer(TimerBarWidgetHandle, TimerBarWidgetDelegate, Duration * (UGameplayStatics::GetGlobalTimeDilation(this)), false);

    if(!Timer_Component)
    {
        UCountess_Timer_Component* TimerComponent_Duration = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
        TimerComponent_Duration->RegisterComponent();
        TimerComponent_Duration->bIsAbilityTimeSlow = bIsAbilityTimeSlow;
        TimerComponent_Duration->CountessTimerCompletedDelegate.AddDynamic(this, &UCountess_TimerBar_WidgetComp::DestroyTimerBarWidgetComponent);
        TimerComponent_Duration->CountessTimerDelegate.AddUObject(TimerBar_Widget, &UCountess_TimerBar_Widget::SetTimerBarPercentage);
        TimerComponent_Duration->StartLerp(0, Duration);
        
    }
    else
    {
        Timer_Component->CountessTimerDelegate.AddUObject(TimerBar_Widget, &UCountess_TimerBar_Widget::SetTimerBarPercentage);
        Timer_Component->CountessTimerCompletedDelegate.AddDynamic(this, &UCountess_TimerBar_WidgetComp::DestroyTimerBarWidgetComponent);
    }
}
*/
void UCountess_TimerBar_WidgetComp::DestroyTimerBarWidgetComponent()
{
    //GetWorld()->GetTimerManager().ClearTimer(TimerBarWidgetHandle);
    TimerBarComponentDestroyedDelegate.Broadcast(ComponentId);
    this->DestroyComponent();
}
