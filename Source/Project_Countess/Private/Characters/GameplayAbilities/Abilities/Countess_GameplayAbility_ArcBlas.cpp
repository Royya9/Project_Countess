// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_ArcBlas.h"
#include "GameplayTagContainer.h"
#include "Characters/GameplayAbilities/Countess_AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ArrowComponent.h"
#include "Actors/Countess_Actor_ArcBlast.h"
#include "Player/Countess_PlayerState.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_ArcBlast_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_ArcBlast_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_ArcBlast_Damage.h"
#include "Characters/GameplayAbilities/AbilityTasks/CT_PlayMontageAndWaitForEvent.h"

UCountess_GameplayAbility_ArcBlas::UCountess_GameplayAbility_ArcBlas()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityType = EAbilityType::Active;
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);

	AbilityTags.AddTag(CountessTags::FAbilityTags::ArcticBlastAbilityTag);

	/* loading ArcticBlast sound*/
	static ConstructorHelpers::FObjectFinder<USoundWave> ArcticBlastSoundObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/ArcticBlast_Cast.ArcticBlast_Cast'"));
	if (ArcticBlastSoundObject.Succeeded())
		SoundToPlay = ArcticBlastSoundObject.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find Arctic Blast Sound wave in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/

	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/ArcticBlastAbility/BP_AbilityData_ArcticBlast'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	/*Load ArcticBlast Montage*/
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ArcticBlastMontageObject(TEXT("AnimMontage'/Game/MyProjectMain/Animations/AnimMontage_Cast.AnimMontage_Cast'"));
	if (ArcticBlastMontageObject.Succeeded())
		ArcticBlastAnimMontage = ArcticBlastMontageObject.Object;
	else
		UE_LOG(Countess_Log, Warning, TEXT("Can't find ArcticBlast Anim Montage in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/* loading Arctic Blast emitter*/
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ArcticBlastParticleEffectObject(TEXT("ParticleSystem'/Game/MyProjectMain/Particles/p_Countess_ArcBlast_Flash.p_Countess_ArcBlast_Flash'"));
	if (ArcticBlastParticleEffectObject.Succeeded())
		EmitterToSpawn = ArcticBlastParticleEffectObject.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find particle effect in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/* loading Fireball emitter*/
// 	static ConstructorHelpers::FClassFinder<ACountess_Actor_Fireball> FireballParticleClassFinder(TEXT("'/Game/MyProjectMain/Blueprints/Actors/BP_Countess_Actor_Fireball'"));
// 	if (FireballParticleClassFinder.Succeeded())
// 		FireballClass = FireballParticleClassFinder.Class;
// 	else
// 		UE_LOG(LogTemp, Warning, TEXT("Can't find particle effect in %s. Check if it is moved."), TEXT(__FUNCTION__));

	CooldownGameplayEffectClass = UCountess_GE_ArcBlast_CoolDown::StaticClass();
	CostGameplayEffectClass = UCountess_GE_ArcBlast_Cost::StaticClass();
	ArcticBlastClass = ACountess_Actor_ArcBlast::StaticClass();
	ArcticBlastDamageEffectClass = UCountess_GE_ArcBlast_Damage::StaticClass();
}

void UCountess_GameplayAbility_ArcBlas::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	//CommitAbility(Handle, ActorInfo, ActivationInfo);

	ACountess_Character_Player* Player = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
	UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	if (!Player || !PlayerASC)
	{
		UE_LOG(Countess_Log, Error, TEXT("CountessPlayerCharacter ie Avatar Actor not found in %s"), TEXT(__FUNCTION__));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UCT_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent = UCT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this, FName("ArcticBlast Montage"), ArcticBlastAnimMontage, FGameplayTagContainer(), 1.f, NAME_None, false, 1.f
	);
	PlayMontageAndWaitForEvent->OnCompleted.AddDynamic(this, &UCountess_GameplayAbility_ArcBlas::OnCompleted);
	PlayMontageAndWaitForEvent->OnInterrupted.AddDynamic(this, &UCountess_GameplayAbility_ArcBlas::OnCompleted);
	PlayMontageAndWaitForEvent->OnBlendOut.AddDynamic(this, &UCountess_GameplayAbility_ArcBlas::OnCompleted);
	PlayMontageAndWaitForEvent->OnCancelled.AddDynamic(this, &UCountess_GameplayAbility_ArcBlas::OnCompleted);
	PlayMontageAndWaitForEvent->EventReceived.AddDynamic(this, &UCountess_GameplayAbility_ArcBlas::OnEventReceived);
	PlayMontageAndWaitForEvent->ReadyForActivation();

	if(!PlayerASC->CountessTimeSlowActivated.IsAlreadyBound(this, &UCountess_GameplayAbility_ArcBlas::HandleDurationAndCooldownEffectsOnTimeSlow))
		PlayerASC->CountessTimeSlowActivated.AddDynamic(this, &UCountess_GameplayAbility_ArcBlas::HandleDurationAndCooldownEffectsOnTimeSlow);
}

void UCountess_GameplayAbility_ArcBlas::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UCountess_GameplayAbility_Base::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	//UE_LOG(Countess_Log, Warning, TEXT("Player Double Jump Ability was cancelled!! from %s"), TEXT(__FUNCTION__));
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCountess_GameplayAbility_ArcBlas::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCountess_GameplayAbility_ArcBlas::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	//Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		CooldownHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, CooldownGE, GetAbilityLevel(Handle, ActorInfo));

		UE_LOG(Countess_Log, Warning, TEXT("From %s. Applied Custom Cooldown with handle %s"), TEXT(__FUNCTION__), *CooldownHandle.ToString());

	}
}

bool UCountess_GameplayAbility_ArcBlas::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /* = nullptr */, const FGameplayTagContainer* TargetTags /* = nullptr */, OUT FGameplayTagContainer* OptionalRelevantTags /* = nullptr */) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (ASC)
	{
		if (ASC->HasMatchingGameplayTag(CountessTags::FCooldownTags::ArcticBlastAbilityCooldownTag)) // Our ASC has cooldown tag. so Ability is still in CD
			OptionalRelevantTags->AddTag(CountessTags::FCooldownTags::ArcticBlastAbilityCooldownTag);

		if (!this->CheckCost(Handle, ActorInfo, OptionalRelevantTags)) // Check if we have enough mana. 
			OptionalRelevantTags->AddTag(CountessTags::FCostTags::ArcticBlastAbilityCostTag);
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCountess_GameplayAbility_ArcBlas::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
}

void UCountess_GameplayAbility_ArcBlas::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(Countess_Log, Warning, TEXT("Event %s Received from %s"), *EventTag.ToString(), TEXT(__FUNCTION__)); 
// We received Event.Ability.Fireball.Cast Tag at the correct instant in montage which we setup in
// FireballAnimMontage:notify -> AnimBP:make event data & send to actor
// We can check for matching that exact tag but we are sending only one tag to actor at the moment. So we can afford to skip this check for now.
//	if (EventTag != CountessTags::FEventTags::ArcticBlastCastEventTag)
//		return;

	ACountess_Character_Player* Player = Cast<ACountess_Character_Player>(GetCurrentActorInfo()->AvatarActor.Get());
	ACountess_PlayerState* Countess_PlayerState = Cast<ACountess_PlayerState>(GetCurrentActorInfo()->OwnerActor.Get());
	if (!Player || !Countess_PlayerState)
	{
		UE_LOG(Countess_Log, Error, TEXT("CountessPlayerCharacter ie Avatar Actor or Countess PlayerState not found in %s"), TEXT(__FUNCTION__));
		EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
		return;
	}
	FGameplayEffectContextHandle EffectContextHandle = Player->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = Player->GetAbilitySystemComponent()->MakeOutgoingSpec(ArcticBlastDamageEffectClass, Countess_PlayerState->GetPlayerLevel(), EffectContextHandle);

	FTransform ProjectileSpawnTransform = Player->GetFireballSpawnLocationArrowComponent()->GetComponentTransform();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACountess_Actor_ArcBlast* ArcticBlast = GetWorld()->SpawnActorDeferred<ACountess_Actor_ArcBlast>(ArcticBlastClass, ProjectileSpawnTransform, GetOwningActorFromActorInfo(),
		Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	ArcticBlast->Range = 4500.f;
	ArcticBlast->DamageEffectSpecHandle = SpecHandle;
	ArcticBlast->FinishSpawning(ProjectileSpawnTransform);
	ArcticBlast->CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(this);

	if (SoundToPlay.IsValid(false))
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay.Get(false), ProjectileSpawnTransform.GetLocation());

	if (EmitterToSpawn.IsValid(false))
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterToSpawn.Get(false), ProjectileSpawnTransform.GetLocation());
}

void UCountess_GameplayAbility_ArcBlas::HandleDurationAndCooldownEffectsOnTimeSlow(const float TimeDilationAmount,
	const float TimeRemaining, const float ActualDurationTime)
{
	Super::HandleAbilityDurationAndCooldownOnTimeSlowActivate(TimeDilationAmount, TimeRemaining, ActualDurationTime,
this->GetCooldownTimeRemaining(), FActiveGameplayEffectHandle(), FGameplayTag());
	
	UCountess_AbilitySystemComponent* PlayerASC = Cast<UCountess_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if(PlayerASC)
		PlayerASC->CountessTimeSlowActivated.RemoveDynamic(this, &UCountess_GameplayAbility_ArcBlas::HandleDurationAndCooldownEffectsOnTimeSlow);
}
