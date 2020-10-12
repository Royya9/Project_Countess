// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Shield.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Shield_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Shield_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Shield_Bonus.h"
#include "Player/Countess_PlayerController.h"
#include "Player/Countess_PlayerState.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Components/Countess_Timer_Component.h"

UCountess_GameplayAbility_Shield::UCountess_GameplayAbility_Shield()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);
	ActivationBlockedTags.AddTag(CountessTags::FCooldownTags::ShieldAbilityCooldownTag);
	AbilityTags.AddTag(CountessTags::FAbilityTags::ShieldAbilityTag);
	AbilityType = EAbilityType::Active;
	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/
	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/ShieldAbility/BP_AbilityData_Shield'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	//Particle System to Spawn
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemHelper(TEXT("NiagaraSystem'/Game/MyProjectMain/Particles/P_NS_Shield.P_NS_Shield'"));
	if (NiagaraSystemHelper.Succeeded())
	{
		NiagaraSystemToSpawn = NiagaraSystemHelper.Object;
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Niagara System to play Not found.. in %s"), TEXT(__FUNCTION__));

	CostGameplayEffectClass = UCountess_GE_Shield_Cost::StaticClass();
	CooldownGameplayEffectClass = UCountess_GE_Shield_CoolDown::StaticClass();
	ShieldOnEffectClass = UCountess_GE_Shield_Bonus::StaticClass();
}

void UCountess_GameplayAbility_Shield::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		if (!Countess_PlayerCharacter || !Countess_PlayerState)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

		//PlayerASC->AddLooseGameplayTag(CountessTags::FCooldownTags::ShieldAbilityCooldownTag);
		//UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectRemainingDuration(this, )
		
		// Show timer Widget on Player
		if (AbilityData.Get())
		{
			const FText& AbilityText = AbilityData.Get()->Title;
			const FString ContextString;
			const float Duration = AbilityData.Get()->AbilityDurationHandle.Eval(Countess_PlayerState->GetPlayerLevel(), ContextString);
			const float Cooldownduration = AbilityData.Get()->CoolDownRowHandle.Eval(Countess_PlayerState->GetPlayerLevel(), ContextString);
			const float ActualCD = this->GetCooldownTimeRemaining(ActorInfo);
			UE_LOG(Countess_Log, Warning, TEXT("From %s. Shiled CooldownDuration calculated from AbilityData= %f and Actual CD = %f"), TEXT(__FUNCTION__), Cooldownduration, ActualCD);

			const FGameplayEffectSpecHandle ShieldAbilitySpecHandle = PlayerASC->MakeOutgoingSpec(ShieldOnEffectClass, Countess_PlayerState->GetPlayerLevel(), PlayerASC->MakeEffectContext());
			ShieldAbilitySpecHandle.Data.Get()->SetSetByCallerMagnitude(CountessTags::FStatusTags::ShieldAbilityOnTag, Duration);
            ShieldAbilityOnEffectHandle = PlayerASC->ApplyGameplayEffectSpecToSelf(*ShieldAbilitySpecHandle.Data.Get());
			
			//FTimerDelegate ShieldAbilityTimerDelegate = FTimerDelegate::CreateUObject(this, &UCountess_GameplayAbility_Shield::OnShieldAbilityDurationCompleted);
			//GetWorld()->GetTimerManager().SetTimer(ShieldAbilityTimerHandle, ShieldAbilityTimerDelegate, Duration * UGameplayStatics::GetGlobalTimeDilation(this), false);

			UCountess_Timer_Component* TimerComponent_Duration = NewObject<UCountess_Timer_Component>(this, UCountess_Timer_Component::StaticClass());
			TimerComponent_Duration->RegisterComponent();
			TimerComponent_Duration->CountessTimerCompletedDelegate.AddDynamic(this, &UCountess_GameplayAbility_Shield::OnShieldAbilityDurationCompleted);
			TimerComponent_Duration->StartLerp(0, Duration);

			Countess_PlayerController->ShowTimerBarWidget(AbilityText, Duration, false, TimerComponent_Duration);

			if(!PlayerASC->CountessTimeSlowActivated.IsAlreadyBound(this, &UCountess_GameplayAbility_Shield::HandleDurationAndCooldownEffectsOnTimeSlow))
				PlayerASC->CountessTimeSlowActivated.AddDynamic(this, &UCountess_GameplayAbility_Shield::HandleDurationAndCooldownEffectsOnTimeSlow);

		}

		if (NiagaraSystemToSpawn.Get())
		{
			ShieldNiagaraComponent = NewObject<UNiagaraComponent>(Countess_PlayerCharacter, FName("Shield Niagara Component"));
			ShieldNiagaraComponent->SetupAttachment(Countess_PlayerCharacter->GetRootComponent());
			ShieldNiagaraComponent->RegisterComponent();

			ShieldNiagaraComponent->SetAsset(NiagaraSystemToSpawn.Get());
			ShieldNiagaraComponent->Activate();
			//UE_LOG(Countess_Log, Warning, TEXT("NiagaraSystemComponent spawned at %s"), TEXT(__FUNCTION__));
		}
	}
}

void UCountess_GameplayAbility_Shield::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ShieldNiagaraComponent)
	{
		ShieldNiagaraComponent->Deactivate();
		ShieldNiagaraComponent->DestroyComponent();
	}

	UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();
	if (ShieldAbilityOnEffectHandle.IsValid())
		PlayerASC->RemoveActiveGameplayEffect(ShieldAbilityOnEffectHandle);
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCountess_GameplayAbility_Shield::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ShieldNiagaraComponent)
	{
		ShieldNiagaraComponent->Deactivate();
		ShieldNiagaraComponent->DestroyComponent();
	}

	UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();
	if (ShieldAbilityOnEffectHandle.IsValid())
		PlayerASC->RemoveActiveGameplayEffect(ShieldAbilityOnEffectHandle);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCountess_GameplayAbility_Shield::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	//Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		CooldownHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, CooldownGE, GetAbilityLevel(Handle, ActorInfo));

		UE_LOG(Countess_Log, Warning, TEXT("From %s. Applied Custom Cooldown with handle %s"), TEXT(__FUNCTION__), *CooldownHandle.ToString());

	}
}

bool UCountess_GameplayAbility_Shield::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (ASC)
	{
		if (ASC->HasMatchingGameplayTag(CountessTags::FCooldownTags::ShieldAbilityCooldownTag)) // Our ASC has cooldown tag. so Ability is still in CD
			OptionalRelevantTags->AddTag(CountessTags::FCooldownTags::ShieldAbilityCooldownTag);

		if (!this->CheckCost(Handle, ActorInfo, OptionalRelevantTags)) // Check if we have enough mana. 
			OptionalRelevantTags->AddTag(CountessTags::FCostTags::ShieldAbilityCostTag);
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCountess_GameplayAbility_Shield::OnShieldAbilityDurationCompleted()
{
	//GetWorld()->GetTimerManager().ClearTimer(ShieldAbilityTimerHandle);
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), true, false);
}

void UCountess_GameplayAbility_Shield::HandleDurationAndCooldownEffectsOnTimeSlow(const float TimeDilationAmount, const float TimeRemaining, const float ActualDurationTime)
{
	Super::HandleAbilityDurationAndCooldownOnTimeSlowActivate(TimeDilationAmount, TimeRemaining, ActualDurationTime,
    this->GetCooldownTimeRemaining(), ShieldAbilityOnEffectHandle, CountessTags::FStatusTags::ShieldAbilityOnTag);
	
	UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(PlayerASC)
		PlayerASC->CountessTimeSlowActivated.RemoveDynamic(this, &UCountess_GameplayAbility_Shield::HandleDurationAndCooldownEffectsOnTimeSlow);

	/*
	UE_LOG(Countess_Log, Warning, TEXT("From %s. TimeDilationAmount is %f and TimeRemaining is %f"), TEXT(__FUNCTION__), TimeDilationAmount, TimeRemaining);
	UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(PlayerASC && ShieldAbilityOnEffectHandle.IsValid())
	{

		float CurrentRemainingDuration = this->GetCooldownTimeRemaining();

		CurrentRemainingDuration = (TimeRemaining != ActualDurationTime) ? (CurrentRemainingDuration / UGameplayStatics::GetGlobalTimeDilation(this)) : CurrentRemainingDuration;
		float NewDuration = CurrentRemainingDuration + TimeRemaining/TimeDilationAmount;
		UE_LOG(Countess_Log, Warning, TEXT("From %s. ShieldAbility Actual Remaining Cooldown is %f \n and New Cooldown is %f."), TEXT(__FUNCTION__), CurrentRemainingDuration, NewDuration);
		PlayerASC->SetGameplayEffectDurationHandle(CooldownHandle, NewDuration);
		*/
		/*
		FGameplayTagContainer TagContainer_CD;
		TagContainer_CD.AddTag(CountessTags::FCooldownTags::ShieldAbilityCooldownTag);
		FGameplayEffectQuery const Query_CD = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(TagContainer_CD);
		TArray<float> TimeRemainingArray_CD = PlayerASC->GetActiveEffectsTimeRemaining(Query_CD);
		if (TimeRemainingArray_CD.Num() > 0)
		{
			CurrentRemainingDuration = TimeRemainingArray_CD[0];
			NewDuration = CurrentRemainingDuration + TimeRemaining / TimeDilationAmount;
			UE_LOG(Countess_Log, Warning, TEXT("From %s. ShieldAbility Remaining Duration calculated manually is %f."), TEXT(__FUNCTION__), CurrentRemainingDuration);
		}
		*/
		/*
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(CountessTags::FStatusTags::ShieldAbilityOnTag);
		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyEffectTags(TagContainer);
		TArray<float> TimeRemainingArray = PlayerASC->GetActiveEffectsTimeRemaining(Query);
		if(TimeRemainingArray.Num() >0)
		{
			CurrentRemainingDuration = TimeRemainingArray[0];
			NewDuration = CurrentRemainingDuration + TimeRemaining/TimeDilationAmount;
			UE_LOG(Countess_Log, Warning, TEXT("From %s. ShieldAbility Remaining Duration is %f."), TEXT(__FUNCTION__), CurrentRemainingDuration);
			*/
			/*
			PlayerASC->RemoveActiveGameplayEffect(ShieldAbilityOnEffectHandle);
			ACountess_PlayerState* Countess_PlayerState = Cast<ACountess_PlayerState>(this->GetCurrentActorInfo()->OwnerActor.Get());
			ShieldAbilitySpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(ShieldOnEffectClass, Countess_PlayerState->GetPlayerLevel(), GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());
			ShieldAbilitySpecHandle.Data.Get()->SetSetByCallerMagnitude(CountessTags::FStatusTags::ShieldAbilityOnTag, NewDuration);
			ShieldAbilityOnEffectHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*ShieldAbilitySpecHandle.Data.Get());
			*/
	/*
			PlayerASC->SetGameplayEffectDurationHandle(ShieldAbilityOnEffectHandle, NewDuration);
		}

		PlayerASC->CountessTimeSlowActivated.RemoveDynamic(this, &UCountess_GameplayAbility_Shield::HandleDurationAndCooldownEffectsOnTimeSlow);
	}
	*/
}