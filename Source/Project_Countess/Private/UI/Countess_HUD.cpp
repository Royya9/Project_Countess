// Free to play. By SoNa Games.


#include "UI/Countess_HUD.h"
#include "UI/Countess_HUD_Widget.h"

ACountess_HUD::ACountess_HUD()
{
	static ConstructorHelpers::FClassFinder<UCountess_HUD_Widget> Countess_HUD_Widget_Class_Getter(TEXT("/Game/MyProjectMain/Widgets/BP_Countess_HUD_Widget"));
	if (Countess_HUD_Widget_Class_Getter.Succeeded())
	{
		Countess_HUD_Widget_Class = Countess_HUD_Widget_Class_Getter.Class;
		//UE_LOG(LogTemp, Warning, TEXT("HUD Widget Class is %s"), *Countess_HUD_Widget_Class->GetName());
	}


	
}

void ACountess_HUD::BeginPlay()
{
	Super::BeginPlay();
}

void ACountess_HUD::CreateWidgets(class APlayerController* PlayerController)
{
	if (HUD_Widget)
		return;

	HUD_Widget = CreateWidget<UCountess_HUD_Widget>(PlayerController, Countess_HUD_Widget_Class);
	HUD_Widget->AddToViewport();
}
