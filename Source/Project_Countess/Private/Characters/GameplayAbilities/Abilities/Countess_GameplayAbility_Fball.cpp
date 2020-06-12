// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Fball.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ArrowComponent.h"
#include "Actors/Countess_Actor_Fireball.h"
#include "Player/Countess_PlayerState.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Fireball_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Fireball_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Fireball_Damage.h"
#include "Characters/GameplayAbilities/AbilityTasks/CT_PlayMontageAndWaitForEvent.h"

UCountess_GameplayAbility_Fball::UCountess_GameplayAbility_Fball()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);

	AbilityTags.AddTag(CountessTags::FAbilityTags::FireballAbilityTag);

	/* loading fireball sound*/
	static ConstructorHelpers::FObjectFinder<USoundWave> FireballSoundObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/SFX_Fireball_wav.SFX_Fireball_wav'"));
	if (FireballSoundObject.Succeeded())
		SoundToPlay = FireballSoundObject.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find Fireball Sound wave in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/

	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/FireballAbility/BP_AbilityData_Fireball'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	/*Load Fireball Montage*/
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireballMontageObject(TEXT("AnimMontage'/Game/MyProjectMain/Animations/AnimMontage_Cast.AnimMontage_Cast'"));
	if (FireballMontageObject.Succeeded())
		FireballAnimMontage = FireballMontageObject.Object;
	else
		UE_LOG(Countess_Log, Warning, TEXT("Can't find Fireball Anim Montage in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/* loading Fireball emitter*/
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireballParticleEffectObject(TEXT("ParticleSystem'/Game/MyProjectMain/Particles/p_Countess_BlinkStrikeFlash.p_Countess_BlinkStrikeFlash'"));
	if (FireballParticleEffectObject.Succeeded())
		EmitterToSpawn = FireballParticleEffectObject.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find particle effect in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/* loading Fireball emitter*/
// 	static ConstructorHelpers::FClassFinder<ACountess_Actor_Fireball> FireballParticleClassFinder(TEXT("'/Game/MyProjectMain/Blueprints/Actors/BP_Countess_Actor_Fireball'"));
// 	if (FireballParticleClassFinder.Succeeded())
// 		FireballClass = FireballParticleClassFinder.Class;
// 	else
// 		UE_LOG(LogTemp, Warning, TEXT("Can't find particle effect in %s. Check if it is moved."), TEXT(__FUNCTION__));

	CooldownGameplayEffectClass = UCountess_GE_Fireball_CoolDown::StaticClass();
	CostGameplayEffectClass = UCountess_GE_Fireball_Cost::StaticClass();
	FireballClass = ACountess_Actor_Fireball::StaticClass();
	FireballDamageEffectClass = UCountess_GE_Fireball_Damage::StaticClass();
}

void UCountess_GameplayAbility_Fball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	ACountess_Character_Player* Player = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		UE_LOG(Countess_Log, Error, TEXT("CountessPlayerCharacter ie Avatar Actor not found in %s"), TEXT(__FUNCTION__));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UCT_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent = UCT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		this, FName("Fireball Montage"), FireballAnimMontage, FGameplayTagContainer(), 1.f, NAME_None, false, 1.f
	);
	PlayMontageAndWaitForEvent->OnCompleted.AddDynamic(this, &UCountess_GameplayAbility_Fball::OnCompleted);
	PlayMontageAndWaitForEvent->OnInterrupted.AddDynamic(this, &UCountess_GameplayAbility_Fball::OnCompleted);
	PlayMontageAndWaitForEvent->OnBlendOut.AddDynamic(this, &UCountess_GameplayAbility_Fball::OnCompleted);
	PlayMontageAndWaitForEvent->OnCancelled.AddDynamic(this, &UCountess_GameplayAbility_Fball::OnCompleted);
	PlayMontageAndWaitForEvent->EventReceived.AddDynamic(this, &UCountess_GameplayAbility_Fball::OnEventReceived);
	PlayMontageAndWaitForEvent->ReadyForActivation();

}

void UCountess_GameplayAbility_Fball::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UCountess_GameplayAbility_Base::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	//UE_LOG(Countess_Log, Warning, TEXT("Player Double Jump Ability was cancelled!! from %s"), TEXT(__FUNCTION__));
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCountess_GameplayAbility_Fball::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCountess_GameplayAbility_Fball::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

void UCountess_GameplayAbility_Fball::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
}

void UCountess_GameplayAbility_Fball::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(Countess_Log, Warning, TEXT("Event %s Received from %s"), *EventTag.ToString(), TEXT(__FUNCTION__)); 
	// We received Event.Ability.Fireball.Cast Tag at the correct instant in montage which we setup in
	// FireballAnimMontage:notify -> AnimBP:make event data & send to actor
	// We can check for matching that exact tag but we are sending only one tag to actor at the moment. So we can afford to skip this check for now.
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

	FGameplayEffectSpecHandle SpecHandle = Player->GetAbilitySystemComponent()->MakeOutgoingSpec(FireballDamageEffectClass, Countess_PlayerState->GetPlayerLevel(), EffectContextHandle);

	FTransform ProjectileSpawnTransform = Player->GetFireballSpawnLocationArrowComponent()->GetComponentTransform();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACountess_Actor_Fireball* Fireball = GetWorld()->SpawnActorDeferred<ACountess_Actor_Fireball>(FireballClass, ProjectileSpawnTransform, GetOwningActorFromActorInfo(),
		Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Fireball->Range = 4500.f;
	Fireball->DamageEffectSpecHandle = SpecHandle;
	Fireball->FinishSpawning(ProjectileSpawnTransform);

	if (SoundToPlay.IsValid(false))
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay.Get(false), ProjectileSpawnTransform.GetLocation());

	if (EmitterToSpawn.IsValid(false))
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterToSpawn.Get(false), ProjectileSpawnTransform.GetLocation());
	
}