// Free to play. By SoNa Games.


#include "UI/Countess_TimerBar_WidgetComp.h"
#include "UI/Countess_TimerBar_Widget.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

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

}

void UCountess_TimerBar_WidgetComp::SetAbilityTextAndDuration_Implementation(const FText& AbilityText,
    const float Duration)
{
    UCountess_TimerBar_Widget* TimerBar_Widget = Cast<UCountess_TimerBar_Widget>(this->GetUserWidgetObject());
    if(TimerBar_Widget)
    {
        TimerBar_Widget->SetTimerBarWidgetText(AbilityText);
        TimerBar_Widget->SetTimerBarDuration(Duration);
    }
    FTimerDelegate TimerBarWidgetDelegate = FTimerDelegate::CreateUObject(this, &UCountess_TimerBar_WidgetComp::DestroyTimerBarWidgetComponent);
    GetWorld()->GetTimerManager().SetTimer(TimerBarWidgetHandle, TimerBarWidgetDelegate, Duration * (UGameplayStatics::GetGlobalTimeDilation(this)), false);
}

void UCountess_TimerBar_WidgetComp::DestroyTimerBarWidgetComponent()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerBarWidgetHandle);
    this->DestroyComponent();
}
