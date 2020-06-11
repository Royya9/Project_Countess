// Free to play. By SoNa Games.


#include "AI/Characters/Countess_Character_AI_Minion.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "UI/Countess_AI_Widget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"
#include "AI/Effects/Countess_GE_MinionStats.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_PlayerStats.h"
#include "AI/Controllers/Countess_AIMinion_Controller.h"

// Sets default values
ACountess_Character_AI_Minion::ACountess_Character_AI_Minion()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MinionAbilitySystemComponent = CreateDefaultSubobject<UCountess_AbilitySystemComponent>(FName("Minion Ability System Component"));
	AbilitySystemComponent = MinionAbilitySystemComponent; // set our base class ASC ref to MinionASC
	
	MinionAttributeSet = CreateDefaultSubobject<UCountess_AttributeSet_Base>(FName("Minion Attribute Set"));
	//AttributeSet = MinionAttributeSet;

	Minion_Widget_Component = CreateDefaultSubobject<UWidgetComponent>(FName("Health Widget"));
	Minion_Widget_Component->SetupAttachment(RootComponent);
	Minion_Widget_Component->SetRelativeLocation(FVector(0, 0, 150.f));
	Minion_Widget_Component->SetWidgetSpace(EWidgetSpace::Screen);
	Minion_Widget_Component->SetDrawSize(FVector2D(2000.f, 1000.f));

	static ConstructorHelpers::FClassFinder<UCountess_AI_Widget> Countess_AI_Widget_Class_Helper(TEXT("/Game/MyProjectMain/Widgets/BP_Countess_AI_Widget"));
	if (Countess_AI_Widget_Class_Helper.Succeeded())
		Minion_Widget_Class = Countess_AI_Widget_Class_Helper.Class;
	else
		UE_LOG(Countess_Log, Warning, TEXT("Countess AI Widget not FOUND!!! Check if it is moved. from %s"), TEXT(__FUNCTION__));

	//Setting our AI Controller
	AIControllerClass = ACountess_AIMinion_Controller::StaticClass();

	MinionLevel = 1;
}

int32 ACountess_Character_AI_Minion::GetMinionLevel() const
{
	return MinionLevel;
}

// Called when the game starts or when spawned
void ACountess_Character_AI_Minion::BeginPlay()
{
	Super::BeginPlay();
	if (MinionAbilitySystemComponent->IsValidLowLevel())
	{
		MinionAbilitySystemComponent->InitAbilityActorInfo(this, this);

		GiveStartupEffects();

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			if (Minion_Widget_Class)
			{
				Minion_Widget = CreateWidget<UCountess_AI_Widget>(PC, Minion_Widget_Class, FName("Minion Widget"));
				if (Minion_Widget)
				{
					Minion_Widget_Component->SetWidget(Minion_Widget);
					//UE_LOG(Countess_Log, Warning, TEXT("Minion Health is %f from %s"), MinionAttributeSet->GetHealth(), TEXT(__FUNCTION__));
					Minion_Widget->SetHealthPercentage(MinionAttributeSet->GetHealth() / MinionAttributeSet->GetMaxHealth());
				}
			}
		}

		MinionAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MinionAttributeSet->GetHealthAttribute()).AddUObject(this, &ACountess_Character_AI_Minion::OnHealthChanged);
	}
	
	MinionTeamId = FGenericTeamId(1);
	this->SetGenericTeamId(MinionTeamId);
}

void ACountess_Character_AI_Minion::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ACountess_Character_AI_Minion::GiveStartupEffects()
{
	//Apply Passive Effects too.
	FGameplayEffectContextHandle GameplayEffectContextHandle = MinionAbilitySystemComponent->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = MinionAbilitySystemComponent->MakeOutgoingSpec(UCountess_GE_MinionStats::StaticClass(), GetMinionLevel(), GameplayEffectContextHandle);
	
	if (GameplayEffectSpecHandle.IsValid())
	{
		MinionAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
		//Below applying SpecToTarget is not working. Though it worked in PlayerState. Don't know why.
		//FActiveGameplayEffectHandle ActiveGameplayEffectHandle = MinionAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), MinionAbilitySystemComponent);
	}
}

bool ACountess_Character_AI_Minion::IsAlive() const
{
	return MinionAttributeSet->GetHealth() > 0.f;
}

FGenericTeamId ACountess_Character_AI_Minion::GetGenericTeamId() const
{
	return MinionTeamId;
}

int32 ACountess_Character_AI_Minion::GetCharacterLevel() const
{
	return MinionLevel;
}

void ACountess_Character_AI_Minion::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	//UE_LOG(Countess_Log, Warning, TEXT("From %s. Minion Health Changed to %f"), TEXT(__FUNCTION__), Data.NewValue);
	if (Minion_Widget)
		Minion_Widget->SetHealthPercentage(Data.NewValue / MinionAttributeSet->GetMaxHealth());	
}

UCountess_AbilitySystemComponent* ACountess_Character_AI_Minion::GetMinionAbilitySystemComponent()
{
	return MinionAbilitySystemComponent;
}

