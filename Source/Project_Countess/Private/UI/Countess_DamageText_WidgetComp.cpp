// Free to play. By SoNa Games.


#include "UI/Countess_DamageText_WidgetComp.h"
#include "UI/Countess_DamageNumber_Widget.h"
#include "TimerManager.h"

UCountess_DamageText_WidgetComp::UCountess_DamageText_WidgetComp()
{
	this->SetWidgetSpace(EWidgetSpace::Screen);
	this->SetDrawSize(FVector2D(200.f, 100.f));
	this->SetPivot(FVector2D(0.5f, 2.f));

	static ConstructorHelpers::FClassFinder<UCountess_DamageNumber_Widget> DamageNumberWidgetClass_Helper(TEXT("'/Game/MyProjectMain/Widgets/BP_Damage_Number'"));
	if (DamageNumberWidgetClass_Helper.Succeeded())
	{
		DamageNumberWidgetClass = DamageNumberWidgetClass_Helper.Class;
		this->SetWidgetClass(DamageNumberWidgetClass);
	}
	else
		UE_LOG(Countess_Log, Warning, TEXT("Can't find DamageNumberWidgetBP in %s. Check if it is moved."), TEXT(__FUNCTION__));


}

void UCountess_DamageText_WidgetComp::SetDamageText_Implementation(float Damage)
{
	DamageNumberWidget = Cast<UCountess_DamageNumber_Widget>(this->GetUserWidgetObject());
	if (DamageNumberWidget)
		DamageNumberWidget->SetDamageText(Damage);

	FTimerHandle DamageTextWidgetTimerHandle;
	FTimerDelegate DamageTextWidgetTimerDelegate = FTimerDelegate::CreateUObject(this, &UCountess_DamageText_WidgetComp::DestroyDamageTextWidgetComponent);
	GetWorld()->GetTimerManager().SetTimer(DamageTextWidgetTimerHandle, DamageTextWidgetTimerDelegate, 1.f, false);
}

void UCountess_DamageText_WidgetComp::DestroyDamageTextWidgetComponent()
{
	//UE_LOG(Countess_Log, Warning, TEXT("Destroyed %s"), *this->GetFName().ToString());
	this->DestroyComponent();
}
