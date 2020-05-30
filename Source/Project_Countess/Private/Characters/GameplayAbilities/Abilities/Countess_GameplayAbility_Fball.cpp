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
#include "Characters/GameplayAbilities/Effects/Countess_GE_Fireball_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Fireball_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Fireball_Damage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UCountess_GameplayAbility_Fball::UCountess_GameplayAbility_Fball()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Fireball")));

	/* loading fireball sound*/
	static ConstructorHelpers::FObjectFinder<USoundWave> FireballSoundObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/SFX_Fireball_wav.SFX_Fireball_wav'"));
	if (FireballSoundObject.Succeeded())
		SoundToPlay = FireballSoundObject.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find Fireball Sound wave in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/

	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/FireballAbility/BP_AbilityData_BDash'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireballMontageObject(TEXT("AnimMontage'/Game/MyProjectMain/Animations/AnimMontage_Cast.AnimMontage_Cast'"));
	if (FireballMontageObject.Succeeded())
		FireballAnimMontage = FireballMontageObject.Object;
	else
		UE_LOG(Countess_Log, Warning, TEXT("Can't find Fireball Anim Montage in %s. Check if it is moved."), TEXT(__FUNCTION__));

	CooldownGameplayEffectClass = UCountess_GE_Fireball_CoolDown::StaticClass();
	CostGameplayEffectClass = UCountess_GE_Fireball_Cost::StaticClass();
	FireballClass = ACountess_Actor_Fireball::StaticClass();
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
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("Fireball Montage"), FireballAnimMontage);
		
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &UCountess_GameplayAbility_Fball::OnCompleted);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UCountess_GameplayAbility_Fball::OnCompleted);
	PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UCountess_GameplayAbility_Fball::OnCompleted);
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &UCountess_GameplayAbility_Fball::OnCompleted);
	PlayMontageAndWait->ReadyForActivation();

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

void UCountess_GameplayAbility_Fball::OnCompleted()
{
	
	ACountess_Character_Player* Player = Cast<ACountess_Character_Player>(GetCurrentActorInfo()->AvatarActor.Get());

	if (!Player)
	{
		UE_LOG(Countess_Log, Error, TEXT("CountessPlayerCharacter ie Avatar Actor not found in %s"), TEXT(__FUNCTION__));
		EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
		return;
	}

	FTransform ProjectileSpawnTransform = Player->GetFireballSpawnLocationArrowComponent()->GetComponentTransform();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACountess_Actor_Fireball* Fireball = GetWorld()->SpawnActorDeferred<ACountess_Actor_Fireball>(FireballClass, ProjectileSpawnTransform, GetOwningActorFromActorInfo(),
		Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Fireball->FinishSpawning(ProjectileSpawnTransform);

	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
}
