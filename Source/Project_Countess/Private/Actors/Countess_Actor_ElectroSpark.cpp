// Free to play. By SoNa Games.


#include "Actors/Countess_Actor_ElectroSpark.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Interfaces/Countess_Interface_Actor.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"

// Sets default values
ACountess_Actor_ElectroSpark::ACountess_Actor_ElectroSpark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ElectroSparkCollisionComponent = CreateDefaultSubobject<USphereComponent>(FName("Collision Component"));
	RootComponent = ElectroSparkCollisionComponent;
	ElectroSparkCollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
	ElectroSparkCollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	ElectroSparkCollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Ignore);
	ElectroSparkCollisionComponent->SetSphereRadius(500.f);
	ElectroSparkCollisionComponent->bHiddenInGame = 0;

	ElectroSparkVFX_Component_Startup = CreateDefaultSubobject<UParticleSystemComponent>(FName("VFX_Startup"));
	ElectroSparkVFX_Component_Startup->SetupAttachment(ElectroSparkCollisionComponent);
	ElectroSparkVFX_Component_Startup->SetRelativeScale3D(FVector(3.f));

	ElectroSparkVFX_Component_Left_01 = CreateDefaultSubobject<UParticleSystemComponent>(FName("VFX_Left01"));
	ElectroSparkVFX_Component_Left_01->SetupAttachment(ElectroSparkCollisionComponent);
	ElectroSparkVFX_Component_Left_01->SetRelativeScale3D(FVector(2.f));
	ElectroSparkVFX_Component_Left_01->SetRelativeLocationAndRotation(FVector(0.f), FRotator(0.f, 0.f, -90.f));
	ElectroSparkVFX_Component_Left_01->CustomTimeDilation = 2.f;

	ElectroSparkVFX_Component_Left_02 = CreateDefaultSubobject<UParticleSystemComponent>(FName("VFX_Left02"));
	ElectroSparkVFX_Component_Left_02->SetupAttachment(ElectroSparkCollisionComponent);
	ElectroSparkVFX_Component_Left_02->SetRelativeScale3D(FVector(1.5f));
	ElectroSparkVFX_Component_Left_02->SetRelativeLocationAndRotation(FVector(0.f), FRotator(0.f, 0.f, -90.f));

	ElectroSparkVFX_Component_Right_01 = CreateDefaultSubobject<UParticleSystemComponent>(FName("VFX_Right01"));
	ElectroSparkVFX_Component_Right_01->SetupAttachment(ElectroSparkCollisionComponent);
	ElectroSparkVFX_Component_Right_01->SetRelativeScale3D(FVector(2.f));
	ElectroSparkVFX_Component_Right_01->SetRelativeLocationAndRotation(FVector(0.f), FRotator(0.f, 0.f, 90.f));
	ElectroSparkVFX_Component_Right_01->CustomTimeDilation = 2.f;

	ElectroSparkVFX_Component_Right_02 = CreateDefaultSubobject<UParticleSystemComponent>(FName("VFX_Right02"));
	ElectroSparkVFX_Component_Right_02->SetupAttachment(ElectroSparkCollisionComponent);
	ElectroSparkVFX_Component_Right_02->SetRelativeScale3D(FVector(1.5f));
	ElectroSparkVFX_Component_Right_02->SetRelativeLocationAndRotation(FVector(0.f), FRotator(0.f, 0.f, 90.f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> VFX_Startup_Object(TEXT("ParticleSystem'/Game/MyProjectMain/Particles/P_RBurst_Fire_Charge_01.P_RBurst_Fire_Charge_01'"));
	if (VFX_Startup_Object.Succeeded())
	{
		ElectroSparkVFX_Startup = VFX_Startup_Object.Object;
		ElectroSparkVFX_Component_Startup->SetTemplate(ElectroSparkVFX_Startup);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> VFX_Startup_Main_Object(TEXT("ParticleSystem'/Game/MyProjectMain/Particles/P_Whirlwind_Lightning_Veng_01.P_Whirlwind_Lightning_Veng_01'"));
	if (VFX_Startup_Main_Object.Succeeded())
	{
		ElectroSparkVFX_Main = VFX_Startup_Main_Object.Object;
		ElectroSparkVFX_Component_Right_01->SetTemplate(ElectroSparkVFX_Main);
		ElectroSparkVFX_Component_Right_02->SetTemplate(ElectroSparkVFX_Main);
		ElectroSparkVFX_Component_Left_01->SetTemplate(ElectroSparkVFX_Main);
		ElectroSparkVFX_Component_Left_02->SetTemplate(ElectroSparkVFX_Main);
	}
}

void ACountess_Actor_ElectroSpark::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ICountess_Interface_Actor* PlayerCharacter_Interface = Cast<ICountess_Interface_Actor>(OtherActor);
		if (PlayerCharacter_Interface)
		{
			UCountess_AbilitySystemComponent* ASC = Cast<UCountess_AbilitySystemComponent>(PlayerCharacter_Interface->GetASC());
			if (ASC)
			{				
				ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
			}
		}
	}
}


// Called when the game starts or when spawned
void ACountess_Actor_ElectroSpark::BeginPlay()
{
	Super::BeginPlay();
	ElectroSparkCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACountess_Actor_ElectroSpark::OnOverlap);
}

void ACountess_Actor_ElectroSpark::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	this->CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(this);
}



