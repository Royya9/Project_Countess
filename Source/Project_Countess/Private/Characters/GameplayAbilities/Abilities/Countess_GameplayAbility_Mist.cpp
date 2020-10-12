// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Mist.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Mist_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Mist_CoolDown.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "TimerManager.h"
#include "Player/Countess_PlayerController.h"
#include "Player/Countess_PlayerState.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "Camera/Countess_CameraManager.h"
#include "Components/Countess_Timer_Component.h"

UCountess_GameplayAbility_Mist::UCountess_GameplayAbility_Mist()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);
	//ActivationBlockedTags.AddTag(CountessTags::FStatusTags::MistAbilityOnTag);
	//ActivationBlockedTags.AddTag(CountessTags::FCooldownTags::MistAbilityCooldownTag);
	AbilityTags.AddTag(CountessTags::FAbilityTags::MistAbilityTag);
	AbilityType = EAbilityType::Active;
	static ConstructorHelpers::FObjectFinder<USoundWave> SoundToPlayObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/WMagic_MistTransform.WMagic_MistTransform'"));
	if (SoundToPlayObject.Succeeded())
		SoundToPlay = SoundToPlayObject.Object;
	else
		UE_LOG(Countess_Log, Error, TEXT("Mist Sound is not found in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/
	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/MistAbility/BP_AbilityData_Mist'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	CostGameplayEffectClass = UCountess_GE_Mist_Cost::StaticClass();
	CooldownGameplayEffectClass = UCountess_GE_Mist_CoolDown::StaticClass();

	VignetteIntensity = 1.f;
	BlurAmount = 30.f;
}

void UCountess_GameplayAbility_Mist::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}
		//CommitAbility(Handle, ActorInfo, ActivationInfo);

		ACountess_PlayerController* Countess_PlayerController = Cast<ACountess_PlayerController>(ActorInfo->PlayerController.Get());
		ACountess_Character_Player* Countess_PlayerCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
		ACountess_PlayerState* Countess_PlayerState = Cast<ACountess_PlayerState>(ActorInfo->OwnerActor.Get());
		if (!Countess_PlayerCharacter || !Countess_PlayerController)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
		PlayerASC->AddLooseGameplayTag(CountessTags::FStatusTags::MistAbilityOnTag); // Add Status GameplayTag which blocks this ability from activating again while being active 
		//PlayerASC->AddLooseGameplayTag(CountessTags::FCooldownTags::MistAbilityCooldownTag);
		
		// Play Ability Sound
		if (SoundToPlay.Get())
			UGameplayStatics::PlaySound2D(this, SoundToPlay.Get(), 3.f);

		// Show timer Widget on Player
		if (AbilityData.Get())
		{
			Countess_PlayerCharacter->MistAbilityOn();

			const FText& AbilityText = AbilityData.Get()->Title;
			const FString ContextString;
			const float Duration = AbilityData.Get()->AbilityDurationHandle.Eval(Countess_PlayerState->GetPlayerLevel(), ContextString);
			const float CooldownDuration = AbilityData.Get()->CoolDownRowHandle.Eval(Countess_PlayerState->GetPlayerLevel(), ContextString);

			ACountess_CameraManager* Countess_CameraManager = Cast<ACountess_CameraManager>(Countess_PlayerController->PlayerCameraManager);
			if (Countess_CameraManager)
			{
				Countess_CameraManager->SetVignetteAndBlur(VignetteIntensity, BlurAmount, 0, 0.15f);
			}

			//FTimerDelegate MistAbilityTimerDelegate = FTimerDelegate::CreateUObject(this, &UCountess_GameplayAbility_Mist::OnMistAbilityDurationCompleted);
			//GetWorld()->GetTimerManager().SetTimer(MistAbilityTimerHandle, MistAbilityTimerDelegate, Duration * UGameplayStatics::GetGlobalTimeDilation(this), false);

			UCountess_Timer_Component* TimerComponent_Duration = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
			TimerComponent_Duration->RegisterComponent();
			TimerComponent_Duration->CountessTimerCompletedDelegate.AddDynamic(this, &UCountess_GameplayAbility_Mist::OnMistAbilityDurationCompleted);
			TimerComponent_Duration->StartLerp(0, Duration);
			
			Countess_PlayerController->ShowTimerBarWidget(AbilityText, Duration, false, TimerComponent_Duration);

			/*
			CooldownTimer = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
			CooldownTimer->RegisterComponent();
			CooldownTimer->CountessTimerCompletedDelegate.AddDynamic(this, &UCountess_GameplayAbility_Mist::RemoveCooldownTagOnCooldownCompleted);
			CooldownTimer->CountessTimerRemainingAbsValueDelegate.AddDynamic(this, &UCountess_GameplayAbility_Mist::CurrentCooldown);
			CooldownTimer->StartLerp(0, CooldownDuration);
			*/
			
			if(!PlayerASC->CountessTimeSlowActivated.IsAlreadyBound(this, &UCountess_GameplayAbility_Mist::HandleDurationAndCooldownEffectsOnTimeSlow))
				PlayerASC->CountessTimeSlowActivated.AddDynamic(this, &UCountess_GameplayAbility_Mist::HandleDurationAndCooldownEffectsOnTimeSlow);

		}
	}
}
/*
void UCountess_GameplayAbility_Mist::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	// Play Ability Sound
	if (SoundToPlay.Get())
		UGameplayStatics::PlaySound2D(this, SoundToPlay.Get(), 3.f);

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
		Countess_PlayerCharacter->MistAbilityOff();

	UAbilitySystemComponent* PlayerASC = Cast<UAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (PlayerASC)
		PlayerASC->RemoveLooseGameplayTag(CountessTags::FStatusTags::MistAbilityOnTag); // Remove the status tag on ability cancel
}
*/
void UCountess_GameplayAbility_Mist::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// Play Ability Sound
	if (SoundToPlay.Get())
		UGameplayStatics::PlaySound2D(this, SoundToPlay.Get(), 3.f);

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
		Countess_PlayerCharacter->MistAbilityOff();

	UAbilitySystemComponent* PlayerASC = Cast<UAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (PlayerASC)
		PlayerASC->RemoveLooseGameplayTag(CountessTags::FStatusTags::MistAbilityOnTag); // Remove the status tag on ability cancel
}

void UCountess_GameplayAbility_Mist::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	//Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		CooldownHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, CooldownGE, GetAbilityLevel(Handle, ActorInfo));

		UE_LOG(Countess_Log, Warning, TEXT("From %s. Applied Custom Cooldown with handle %s"), TEXT(__FUNCTION__), *CooldownHandle.ToString());

	}
}

bool UCountess_GameplayAbility_Mist::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (ASC)
	{
		if (ASC->HasMatchingGameplayTag(CountessTags::FCooldownTags::MistAbilityCooldownTag)) // Our ASC has cooldown tag. so Ability is still in CD
			OptionalRelevantTags->AddTag(CountessTags::FCooldownTags::MistAbilityCooldownTag);

		if (!this->CheckCost(Handle, ActorInfo, OptionalRelevantTags)) // Check if we have enough mana. 
			OptionalRelevantTags->AddTag(CountessTags::FCostTags::MistAbilityCostTag);
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCountess_GameplayAbility_Mist::OnMistAbilityDurationCompleted()
{
	//GetWorld()->GetTimerManager().ClearTimer(MistAbilityTimerHandle);
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), true, false);
}

void UCountess_GameplayAbility_Mist::HandleDurationAndCooldownEffectsOnTimeSlow(const float TimeDilationAmount,
	const float TimeRemaining, const float ActualDurationTime)
{
	Super::HandleAbilityDurationAndCooldownOnTimeSlowActivate(TimeDilationAmount, TimeRemaining, ActualDurationTime,
    this->GetCooldownTimeRemaining(), FActiveGameplayEffectHandle(), CountessTags::FStatusTags::MistAbilityOnTag);
	
	UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(PlayerASC)
		PlayerASC->CountessTimeSlowActivated.RemoveDynamic(this, &UCountess_GameplayAbility_Mist::HandleDurationAndCooldownEffectsOnTimeSlow);
}
