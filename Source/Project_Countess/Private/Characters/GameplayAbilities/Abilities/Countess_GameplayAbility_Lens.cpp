// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Lens.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Actors/Countess_LensOfTruth_Platform.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Lens_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Lens_CoolDown.h"
#include "Player/Countess_PlayerController.h"
#include "Player/Countess_PlayerState.h"
#include "TimerManager.h"
#include "Camera/Countess_CameraManager.h"
#include "Components/Countess_Timer_Component.h"

UCountess_GameplayAbility_Lens::UCountess_GameplayAbility_Lens()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    AbilityType = EAbilityType::Active;
    ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);
    ActivationBlockedTags.AddTag(CountessTags::FStatusTags::LensAbilityOnTag); // We cannot active this ability again while it is active.
    AbilityTags.AddTag(CountessTags::FAbilityTags::LensOfTruthAbilityTag);

    static ConstructorHelpers::FObjectFinder<USoundWave> SoundToPlayObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/WMagic_LensActive.WMagic_LensActive'"));
    if(SoundToPlayObject.Succeeded())
        SoundToPlay = SoundToPlayObject.Object;
    else
        UE_LOG(Countess_Log, Error, TEXT("Lens Sound is not found in %s. Check if it is moved."), TEXT(__FUNCTION__));

    /*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/
    static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/LensAbility/BP_AbilityData_Lens'"));
    if (AbilityDataObject.Succeeded())
    {
        AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
        UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
    }
    else
        UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));
    
    CostGameplayEffectClass = UCountess_GE_Lens_Cost::StaticClass();
    CooldownGameplayEffectClass = UCountess_GE_Lens_CoolDown::StaticClass();

    VignetteIntensity = 1.f;
    FieldOfView = 100.f;
}

void UCountess_GameplayAbility_Lens::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
    {
        if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
        {
            EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
            return;
        }
        //CommitAbility(Handle, ActorInfo, ActivationInfo);

        ACountess_PlayerState* Countess_PlayerState = Cast<ACountess_PlayerState>(ActorInfo->OwnerActor.Get());
        ACountess_PlayerController* Countess_PlayerController = Cast<ACountess_PlayerController>(ActorInfo->PlayerController.Get());
        if(!Countess_PlayerState || !Countess_PlayerController)
        {
            EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
            return;
        }

        UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
        PlayerASC->AddLooseGameplayTag(CountessTags::FStatusTags::LensAbilityOnTag); // Add Status GameplayTag which blocks this ability from activating again while being active 
        
        // Play Ability Sound
        if(SoundToPlay.Get())
            UGameplayStatics::PlaySound2D(this, SoundToPlay.Get(), 3.f);
        
        // Make all LensPlatforms visible
        TogglePlatforms(true, PlatformActors);
        
        // Show timer Widget on Player
        if(AbilityData.Get())
        {
            const FText& AbilityText = AbilityData.Get()->Title;
            const FString ContextString;
            const float Duration = AbilityData.Get()->AbilityDurationHandle.Eval(Countess_PlayerState->GetPlayerLevel(), ContextString);
            //Countess_PlayerController->ShowTimerBarWidget(AbilityText, Duration);
            ACountess_CameraManager* Countess_CameraManager = Cast<ACountess_CameraManager>(Countess_PlayerController->PlayerCameraManager);
            if(Countess_CameraManager)
            {
                Countess_CameraManager->SetVignetteAndFOV(VignetteIntensity, FieldOfView);
            }
            
            //FTimerDelegate LensAbilityTimerDelegate = FTimerDelegate::CreateUObject(this, &UCountess_GameplayAbility_Lens::OnLensAbilityDurationCompleted); 
            //GetWorld()->GetTimerManager().SetTimer(LensAbilityTimerHandle, LensAbilityTimerDelegate, Duration * UGameplayStatics::GetGlobalTimeDilation(this), false);

            UCountess_Timer_Component* TimerComponent_Duration = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
            TimerComponent_Duration->RegisterComponent();
            TimerComponent_Duration->CountessTimerCompletedDelegate.AddDynamic(this, &UCountess_GameplayAbility_Lens::OnLensAbilityDurationCompleted);
            TimerComponent_Duration->StartLerp(0, Duration);

            Countess_PlayerController->ShowTimerBarWidget(AbilityText, Duration, false, TimerComponent_Duration);

            if(!PlayerASC->CountessTimeSlowActivated.IsAlreadyBound(this, &UCountess_GameplayAbility_Lens::HandleDurationAndCooldownEffectsOnTimeSlow))
                PlayerASC->CountessTimeSlowActivated.AddDynamic(this, &UCountess_GameplayAbility_Lens::HandleDurationAndCooldownEffectsOnTimeSlow);
        }
    }
}

void UCountess_GameplayAbility_Lens::CancelAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

    TogglePlatforms(false, PlatformActors); // make platforms invisible again with no collision

    //Reset Camera zoom and vignette
    ACountess_PlayerController* Countess_PlayerController = Cast<ACountess_PlayerController>(ActorInfo->PlayerController.Get());
    if (Countess_PlayerController)
    {
        ACountess_CameraManager* Countess_CameraManager = Cast<ACountess_CameraManager>(Countess_PlayerController->PlayerCameraManager);
		if (Countess_CameraManager)
			Countess_CameraManager->ResetVignetteAndFOV();
    }

    UAbilitySystemComponent* PlayerASC = Cast<UAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
    if (PlayerASC)
        PlayerASC->RemoveLooseGameplayTag(CountessTags::FStatusTags::LensAbilityOnTag); // Remove the status tag on ability cancel

}

void UCountess_GameplayAbility_Lens::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility, bool bWasCancelled)
{
    TogglePlatforms(false, PlatformActors); // make platforms invisible again with no collision

	//Reset Camera zoom and vignette
	ACountess_PlayerController* Countess_PlayerController = Cast<ACountess_PlayerController>(ActorInfo->PlayerController.Get());
	if (Countess_PlayerController)
	{
		ACountess_CameraManager* Countess_CameraManager = Cast<ACountess_CameraManager>(Countess_PlayerController->PlayerCameraManager);
		if (Countess_CameraManager)
			Countess_CameraManager->ResetVignetteAndFOV();
	}

    UAbilitySystemComponent* PlayerASC = Cast<UAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
    if (PlayerASC)
        PlayerASC->RemoveLooseGameplayTag(CountessTags::FStatusTags::LensAbilityOnTag); // Remove the status tag on ability end
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCountess_GameplayAbility_Lens::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
    //Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
    UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
    if (CooldownGE)
    {
        CooldownHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, CooldownGE, GetAbilityLevel(Handle, ActorInfo));

        UE_LOG(Countess_Log, Warning, TEXT("From %s. Applied Custom Cooldown with handle %s"), TEXT(__FUNCTION__), *CooldownHandle.ToString());

    }
}

bool UCountess_GameplayAbility_Lens::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
    const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

    if (ASC)
    {
        if (ASC->HasMatchingGameplayTag(CountessTags::FCooldownTags::LensAbilityCooldownTag)) // Our ASC has cooldown tag. so Ability is still in CD
            OptionalRelevantTags->AddTag(CountessTags::FCooldownTags::LensAbilityCooldownTag);
		
        if (!this->CheckCost(Handle, ActorInfo, OptionalRelevantTags)) // Check if we have enough mana. 
            OptionalRelevantTags->AddTag(CountessTags::FCostTags::LensAbilityCostTag);
    }
   return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCountess_GameplayAbility_Lens::TogglePlatforms(const bool bAbilityActive, TArray<AActor*>& OutActors) const
{
    UGameplayStatics::GetAllActorsOfClass(this, ACountess_LensOfTruth_Platform::StaticClass(), OutActors);
    for(auto & Actor: OutActors)
    {
        ACountess_LensOfTruth_Platform* Platform = Cast<ACountess_LensOfTruth_Platform>(Actor);
        if(Platform)
        {
            Platform->ToggleStaticMesh(bAbilityActive);
        }
    }

    if(!bAbilityActive && SoundToPlay.Get()) // Stop playing Ability Sound  
        SoundToPlay.Get()->Volume = 0;
    else if(bAbilityActive && SoundToPlay.Get())
        SoundToPlay.Get()->Volume = 1.f;
}

void UCountess_GameplayAbility_Lens::OnLensAbilityDurationCompleted()
{
    //GetWorld()->GetTimerManager().ClearTimer(LensAbilityTimerHandle);
    EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), true, false);
}

void UCountess_GameplayAbility_Lens::HandleDurationAndCooldownEffectsOnTimeSlow(const float TimeDilationAmount,
    const float TimeRemaining, const float ActualDurationTime)
{
    Super::HandleAbilityDurationAndCooldownOnTimeSlowActivate(TimeDilationAmount, TimeRemaining, ActualDurationTime,
    this->GetCooldownTimeRemaining(), FActiveGameplayEffectHandle(), CountessTags::FStatusTags::LensAbilityOnTag);
	
    UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
    if(PlayerASC)
        PlayerASC->CountessTimeSlowActivated.RemoveDynamic(this, &UCountess_GameplayAbility_Lens::HandleDurationAndCooldownEffectsOnTimeSlow);
}
