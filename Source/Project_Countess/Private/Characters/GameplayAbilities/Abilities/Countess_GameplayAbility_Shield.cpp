// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Shield.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Shield_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Shield_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Shield_Bonus.h"
#include "Player/Countess_PlayerController.h"
#include "Player/Countess_PlayerState.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

UCountess_GameplayAbility_Shield::UCountess_GameplayAbility_Shield()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);
	AbilityTags.AddTag(CountessTags::FAbilityTags::ShieldAbilityTag);

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
		}
		CommitAbility(Handle, ActorInfo, ActivationInfo);

		ACountess_PlayerController* Countess_PlayerController = Cast<ACountess_PlayerController>(ActorInfo->PlayerController.Get());
		ACountess_Character_Player* Countess_PlayerCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
		ACountess_PlayerState* Countess_PlayerState = Cast<ACountess_PlayerState>(ActorInfo->OwnerActor.Get());
		if (!Countess_PlayerCharacter || !Countess_PlayerState)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		UAbilitySystemComponent* PlayerASC = Cast<UAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
		
		FGameplayEffectSpecHandle ShieldAbilitySpecHandle = PlayerASC->MakeOutgoingSpec(ShieldOnEffectClass, Countess_PlayerState->GetPlayerLevel(), PlayerASC->MakeEffectContext());
		ShieldAbilityOnEffectHandle = PlayerASC->ApplyGameplayEffectSpecToSelf(*ShieldAbilitySpecHandle.Data.Get());

		// Show timer Widget on Player
		if (AbilityData.Get())
		{
			const FText& AbilityText = AbilityData.Get()->Title;
			const FString ContextString;
			const float Duration = AbilityData.Get()->AbilityDurationHandle.Eval(Countess_PlayerState->GetPlayerLevel(), ContextString);
			Countess_PlayerController->ShowTimerBarWidget(AbilityText, Duration);

			FTimerDelegate ShieldAbilityTimerDelegate = FTimerDelegate::CreateUObject(this, &UCountess_GameplayAbility_Shield::OnShieldAbilityDurationCompleted);
			GetWorld()->GetTimerManager().SetTimer(ShieldAbilityTimerHandle, ShieldAbilityTimerDelegate, Duration * UGameplayStatics::GetGlobalTimeDilation(this), false);
		}

		if (NiagaraSystemToSpawn.Get())
		{
			ShieldNiagaraComponent = NewObject<UNiagaraComponent>(Countess_PlayerCharacter, FName("Shield Niagara Component"));
			ShieldNiagaraComponent->SetupAttachment(Countess_PlayerCharacter->GetRootComponent());
			ShieldNiagaraComponent->RegisterComponent();

			ShieldNiagaraComponent->SetAsset(NiagaraSystemToSpawn.Get());
			ShieldNiagaraComponent->Activate();
			UE_LOG(Countess_Log, Warning, TEXT("NiagaraSystemComponent spawned at %s"), TEXT(__FUNCTION__));
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
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
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
	GetWorld()->GetTimerManager().ClearTimer(ShieldAbilityTimerHandle);
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), true, false);
}

