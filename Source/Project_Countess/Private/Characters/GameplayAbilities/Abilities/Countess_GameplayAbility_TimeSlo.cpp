// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_TimeSlo.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_TimeSlow_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_TimeSlow_CoolDown.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "TimerManager.h"
#include "Player/Countess_PlayerController.h"
#include "Player/Countess_PlayerState.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "Camera/Countess_CameraManager.h"
#include "Components/Countess_Timer_Component.h"

UCountess_GameplayAbility_TimeSlo::UCountess_GameplayAbility_TimeSlo()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::TimeSlowAbilityOnTag);
	ActivationBlockedTags.AddTag(CountessTags::FCooldownTags::TimeSlowAbilityCooldownTag);
	AbilityTags.AddTag(CountessTags::FAbilityTags::TimeSlowAbilityTag);
	AbilityType = EAbilityType::Active;
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundToPlayObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/SFX_TimeSlowBegin.SFX_TimeSlowBegin'"));
	if (SoundToPlayObject.Succeeded())
		SoundToPlay = SoundToPlayObject.Object;
	else
		UE_LOG(Countess_Log, Error, TEXT("TimeSlow Sound is not found in %s. Check if it is moved."), TEXT(__FUNCTION__));

	static ConstructorHelpers::FObjectFinder<USoundWave> SoundToPlayOnTimeSlowEndObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/SFX_TimeSlowEnd.SFX_TimeSlowEnd'"));
	if (SoundToPlayOnTimeSlowEndObject.Succeeded())
		SoundToPlayOnTimeSlowEnd = SoundToPlayOnTimeSlowEndObject.Object;
	else
		UE_LOG(Countess_Log, Error, TEXT("TimeSlow Sound is not found in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/
	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/TimeSlowAbility/BP_AbilityData_TimeSlow'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	CostGameplayEffectClass = UCountess_GE_TimeSlow_Cost::StaticClass();
	CooldownGameplayEffectClass = UCountess_GE_TimeSlow_CoolDown::StaticClass();

	TimeDilationAmount = 4.f;
}

void UCountess_GameplayAbility_TimeSlo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{

		//CooldownGameplayEffectClass.GetDefaultObject()->DurationMagnitude.; //Scale cooldown period accordingly
		//CommitAbility(Handle, ActorInfo, ActivationInfo);

		//UE_LOG(Countess_Log, Warning, TEXT("From %s. Cooldown remaining is %f"), TEXT(__FUNCTION__), cooldownduration);

		ACountess_PlayerController* Countess_PlayerController = Cast<ACountess_PlayerController>(ActorInfo->PlayerController.Get());
		ACountess_Character_Player* Countess_PlayerCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
		ACountess_PlayerState* Countess_PlayerState = Cast<ACountess_PlayerState>(ActorInfo->OwnerActor.Get());
		if (!Countess_PlayerCharacter || !Countess_PlayerController || !Countess_PlayerState)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
		PlayerASC->AddLooseGameplayTag(CountessTags::FStatusTags::TimeSlowAbilityOnTag); // Add Status GameplayTag which blocks this ability from activating again while being active
		//PlayerASC->AddLooseGameplayTag(CountessTags::FCooldownTags::TimeSlowAbilityCooldownTag);

		// Play Ability Sound
		if (SoundToPlay.Get())
			UGameplayStatics::PlaySound2D(this, SoundToPlay.Get(), 3.f);

		// Show timer Widget on Player
		if (AbilityData.Get())
		{
			// #TODO Set Time Dilation gradually like in 0.2 seconds instead of instantly for smoothness.
			Countess_PlayerCharacter->CustomTimeDilation = TimeDilationAmount;
			UGameplayStatics::SetGlobalTimeDilation(this, 1 / TimeDilationAmount);

			const FText& AbilityText = AbilityData.Get()->Title;
			const FString ContextString;
			ActualDurationTime = AbilityData.Get()->AbilityDurationHandle.Eval(Countess_PlayerState->GetPlayerLevel(), ContextString);
			ActualCooldownTime = AbilityData.Get()->CoolDownRowHandle.Eval(Countess_PlayerState->GetPlayerLevel(), ContextString);

			ACountess_CameraManager* Countess_CameraManager = Cast<ACountess_CameraManager>(Countess_PlayerController->PlayerCameraManager);
			if (Countess_CameraManager)
			{
				Countess_CameraManager->SetVignetteAndBlur(VignetteIntensity, BlurAmount, 0, 0.15f);
			}

			//FTimerDelegate CooldownCompletedTimerDelegate = FTimerDelegate::CreateUObject(this, &UCountess_GameplayAbility_TimeSlo::RemoveCooldownTagOnCooldownCompleted);
			//Don't forget to SetTimer Duration according to the TimeDilation. Otherwise it will last longer than intended!
			//GetWorld()->GetTimerManager().SetTimer(CooldownCompletedTimerHandle, CooldownCompletedTimerDelegate, cooldownduration / TimeDilationAmount, false);

			//FTimerDelegate TimeSlowAbilityTimerDelegate = FTimerDelegate::CreateUObject(this, &UCountess_GameplayAbility_TimeSlo::OnTimeSlowAbilityDurationCompleted);
			//Don't forget to SetTimer Duration according to the TimeDilation. Otherwise it will last longer than intended!
			//GetWorld()->GetTimerManager().SetTimer(TimeSlowAbilityTimerHandle, TimeSlowAbilityTimerDelegate, Duration / TimeDilationAmount, false);


			////////// TESTING REGION
			UCountess_Timer_Component* TimerComponent_Duration = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
			TimerComponent_Duration->RegisterComponent();
			TimerComponent_Duration->bIsAbilityTimeSlow = true;
			TimerComponent_Duration->CountessTimerCompletedDelegate.AddDynamic(this, &UCountess_GameplayAbility_TimeSlo::OnTimeSlowAbilityDurationCompleted);
			TimerComponent_Duration->CountessTimerRemainingAbsValueDelegate.AddDynamic(this, &UCountess_GameplayAbility_TimeSlo::CurrentDurationRemaining);
			TimerComponent_Duration->StartLerp(0, ActualDurationTime);

			Countess_PlayerController->ShowTimerBarWidget(AbilityText, ActualDurationTime, true, TimerComponent_Duration);

			PlayerASC->CountessTimeSlowActivated.Broadcast(TimeDilationAmount, ActualDurationTime, ActualDurationTime); // #TODO Refactor this

			if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			}
		}
	}
}

void UCountess_GameplayAbility_TimeSlo::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	// Play Ability Sound
	if (SoundToPlayOnTimeSlowEnd.Get())
		UGameplayStatics::PlaySound2D(this, SoundToPlayOnTimeSlowEnd.Get(), 3.f);

	ACountess_PlayerController* Countess_PlayerController = Cast<ACountess_PlayerController>(ActorInfo->PlayerController.Get());
	if (Countess_PlayerController)
	{
		ACountess_CameraManager* Countess_CameraManager = Cast<ACountess_CameraManager>(Countess_PlayerController->PlayerCameraManager);
		if (Countess_CameraManager)
		{
			Countess_CameraManager->ResetVignetteAndBlur(0, 0.15f);
		}
	}

	ACountess_Character_Player* Countess_PlayerCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
	if (Countess_PlayerCharacter)
	{
		// #TODO ReSet Time Dilation gradually like in 0.2 seconds instead of instantly for smoothness.
		Countess_PlayerCharacter->CustomTimeDilation = 1.f; // Reset TimeDilation
		UGameplayStatics::SetGlobalTimeDilation(this, 1.f); // Reset TimeDilation
	}

	UAbilitySystemComponent* PlayerASC = Cast<UAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (PlayerASC)
		PlayerASC->RemoveLooseGameplayTag(CountessTags::FStatusTags::TimeSlowAbilityOnTag); // Remove the status tag on ability cancel
}

void UCountess_GameplayAbility_TimeSlo::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Play Ability Sound
	if (SoundToPlayOnTimeSlowEnd.Get())
		UGameplayStatics::PlaySound2D(this, SoundToPlayOnTimeSlowEnd.Get(), 3.f);

	ACountess_PlayerController* Countess_PlayerController = Cast<ACountess_PlayerController>(ActorInfo->PlayerController.Get());
	if (Countess_PlayerController)
	{
		ACountess_CameraManager* Countess_CameraManager = Cast<ACountess_CameraManager>(Countess_PlayerController->PlayerCameraManager);
		if (Countess_CameraManager)
		{
			Countess_CameraManager->ResetVignetteAndBlur(0, 0.15f);
		}
	}

	ACountess_Character_Player* Countess_PlayerCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
	if (Countess_PlayerCharacter)
	{
		// #TODO ReSet Time Dilation gradually like in 0.2 seconds instead of instantly for smoothness.
		Countess_PlayerCharacter->CustomTimeDilation = 1.f; // Reset TimeDilation
		UGameplayStatics::SetGlobalTimeDilation(this, 1.f); // Reset TimeDilation
	}

	UAbilitySystemComponent* PlayerASC = Cast<UAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (PlayerASC)
		PlayerASC->RemoveLooseGameplayTag(CountessTags::FStatusTags::TimeSlowAbilityOnTag); // Remove the status tag on ability End

// 	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
// 	UE_LOG(Countess_Log, Warning, TEXT("From %s. ASC is %s"), TEXT(__FUNCTION__), *ASC->GetFName().ToString());

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//	ASC->RemoveLooseGameplayTag(CountessTags::FCooldownTags::TimeSlowAbilityCooldownTag);
	UE_LOG(Countess_Log, Warning, TEXT("From %s. TimeSlowAbility Ended."), TEXT(__FUNCTION__));
}

void UCountess_GameplayAbility_TimeSlo::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	//This is giving interesting numbers. 
	//Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);



	
	float finalCD = 0.f;
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
//	FActiveGameplayEffectHandle CooldownHandle;
	if (CooldownGE)
	{
		CooldownHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, CooldownGE, GetAbilityLevel(Handle, ActorInfo));
		float cooldownDurationFromGE = this->GetCooldownTimeRemaining();
		float DurationActive = ActualDurationTime/TimeDilationAmount;
		float cooldownAfterTimeSlowIsRemoved = cooldownDurationFromGE - DurationActive;
		finalCD = DurationActive + cooldownAfterTimeSlowIsRemoved * TimeDilationAmount;
	}

	UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(PlayerASC)
		PlayerASC->SetGameplayEffectDurationHandle(CooldownHandle, finalCD);
	
	UE_LOG(Countess_Log, Warning, TEXT("From %s. New Cooldown remaining is %f"), TEXT(__FUNCTION__), finalCD);

}

bool UCountess_GameplayAbility_TimeSlo::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (ASC)
	{
		if (ASC->HasMatchingGameplayTag(CountessTags::FCooldownTags::TimeSlowAbilityCooldownTag)) // Our ASC has cooldown tag. so Ability is still in CD
			OptionalRelevantTags->AddTag(CountessTags::FCooldownTags::TimeSlowAbilityCooldownTag);

		if (!this->CheckCost(Handle, ActorInfo, OptionalRelevantTags)) // Check if we have enough mana. 
			OptionalRelevantTags->AddTag(CountessTags::FCostTags::TimeSlowAbilityCostTag);
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCountess_GameplayAbility_TimeSlo::OnTimeSlowAbilityDurationCompleted()
{
	//GetWorld()->GetTimerManager().ClearTimer(TimeSlowAbilityTimerHandle);
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(CountessTags::FStatusTags::TimeSlowAbilityOnTag);
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), true, false);
}

void UCountess_GameplayAbility_TimeSlo::RemoveCooldownTagOnCooldownCompleted()
{
	UE_LOG(Countess_Log, Warning, TEXT("From %s. CooldownTag is now removed. Can cast ability again"), TEXT(__FUNCTION__));
	//GetWorld()->GetTimerManager().ClearTimer(CooldownCompletedTimerHandle);
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(CountessTags::FCooldownTags::TimeSlowAbilityCooldownTag);
	//EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), true, false);
}

void UCountess_GameplayAbility_TimeSlo::CurrentDurationRemaining(const float DurationRemaining)
{
	DurationTimeRemaining = DurationRemaining;
	UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());

	PlayerASC->CountessTimeSlowActivated.Broadcast(TimeDilationAmount, DurationRemaining, ActualDurationTime);
}
