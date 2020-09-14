// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Blood.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_BloodLust_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_BloodLust_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_BloodLust_Bonus.h"
#include "Player/Countess_PlayerController.h"
#include "Player/Countess_PlayerState.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"


UCountess_GameplayAbility_Blood::UCountess_GameplayAbility_Blood()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);
	AbilityTags.AddTag(CountessTags::FAbilityTags::BloodLustAbilityTag);

	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/
	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/BloodLustAbility/BP_AbilityData_BloodLust'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	CostGameplayEffectClass = UCountess_GE_BloodLust_Cost::StaticClass();
	CooldownGameplayEffectClass = UCountess_GE_BloodLust_CoolDown::StaticClass();
	BloodLustOnEffectClass = UCountess_GE_BloodLust_Bonus::StaticClass();
}

void UCountess_GameplayAbility_Blood::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		CommitAbility(Handle, ActorInfo, ActivationInfo);

		ACountess_PlayerController* Countess_PlayerController = Cast<ACountess_PlayerController>(ActorInfo->PlayerController.Get());
		ACountess_Character_Player* Countess_PlayerCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
		ACountess_PlayerState* Countess_PlayerState = Cast<ACountess_PlayerState>(ActorInfo->OwnerActor.Get());
		if (!Countess_PlayerCharacter || !Countess_PlayerState || !Countess_PlayerCharacter)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		UAbilitySystemComponent* PlayerASC = Cast<UAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

		//FGameplayEffectSpecHandle BloodLustAbilitySpecHandle = PlayerASC->MakeOutgoingSpec(BloodLustOnEffectClass, Countess_PlayerState->GetPlayerLevel(), PlayerASC->MakeEffectContext());
		//BloodLustAbilityOnEffectHandle = PlayerASC->ApplyGameplayEffectSpecToSelf(*BloodLustAbilitySpecHandle.Data.Get());
		BloodLustAbilityOnEffectHandle = PlayerASC->ApplyGameplayEffectToSelf(BloodLustOnEffectClass.GetDefaultObject(), Countess_PlayerState->GetPlayerLevel(), PlayerASC->MakeEffectContext());

		// Show timer Widget on Player
		if (AbilityData.Get())
		{
			const FText& AbilityText = AbilityData.Get()->Title;
			const FString ContextString;
			const float Duration = AbilityData.Get()->AbilityDurationHandle.Eval(Countess_PlayerState->GetPlayerLevel(), ContextString);
			Countess_PlayerController->ShowTimerBarWidget(AbilityText, Duration);

			FTimerDelegate BloodLustAbilityTimerDelegate = FTimerDelegate::CreateUObject(this, &UCountess_GameplayAbility_Blood::OnBloodLustAbilityDurationCompleted);
			GetWorld()->GetTimerManager().SetTimer(BloodLustAbilityTimerHandle, BloodLustAbilityTimerDelegate, Duration * UGameplayStatics::GetGlobalTimeDilation(this), false);
		}

		if (NiagaraSystemToSpawn.Get())
		{
			BloodLustNiagaraComponent = NewObject<UNiagaraComponent>(Countess_PlayerCharacter, FName("BloodLust Niagara Component"));
			BloodLustNiagaraComponent->SetupAttachment(Countess_PlayerCharacter->GetRootComponent());
			BloodLustNiagaraComponent->RegisterComponent();

			BloodLustNiagaraComponent->SetAsset(NiagaraSystemToSpawn.Get());
			BloodLustNiagaraComponent->Activate();
			//UE_LOG(Countess_Log, Warning, TEXT("NiagaraSystemComponent spawned at %s"), TEXT(__FUNCTION__));
		}
	}
}

void UCountess_GameplayAbility_Blood::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (BloodLustNiagaraComponent)
	{
		BloodLustNiagaraComponent->Deactivate();
		BloodLustNiagaraComponent->DestroyComponent();
	}

	UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();
	if (BloodLustAbilityOnEffectHandle.IsValid())
		PlayerASC->RemoveActiveGameplayEffect(BloodLustAbilityOnEffectHandle);
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCountess_GameplayAbility_Blood::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (BloodLustNiagaraComponent)
	{
		BloodLustNiagaraComponent->Deactivate();
		BloodLustNiagaraComponent->DestroyComponent();
	}

	UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponentFromActorInfo();
	if (BloodLustAbilityOnEffectHandle.IsValid())
		PlayerASC->RemoveActiveGameplayEffect(BloodLustAbilityOnEffectHandle);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCountess_GameplayAbility_Blood::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

bool UCountess_GameplayAbility_Blood::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (ASC)
	{
		if (ASC->HasMatchingGameplayTag(CountessTags::FCooldownTags::BloodLustAbilityCooldownTag)) // Our ASC has cooldown tag. so Ability is still in CD
			OptionalRelevantTags->AddTag(CountessTags::FCooldownTags::BloodLustAbilityCooldownTag);

		if (!this->CheckCost(Handle, ActorInfo, OptionalRelevantTags)) // Check if we have enough mana. 
			OptionalRelevantTags->AddTag(CountessTags::FCostTags::BloodLustAbilityCostTag);
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCountess_GameplayAbility_Blood::OnBloodLustAbilityDurationCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(BloodLustAbilityTimerHandle);
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), true, false);
}
