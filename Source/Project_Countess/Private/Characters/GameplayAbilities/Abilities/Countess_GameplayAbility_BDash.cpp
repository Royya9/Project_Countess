// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_BDash.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ArrowComponent.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_BDash_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_BDash_Cost.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UCountess_GameplayAbility_BDash::UCountess_GameplayAbility_BDash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityType = EAbilityType::Active;
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::JumpingTag); // Jumping also blocks this ability
	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::MistAbilityOnTag);

	AbilityTags.AddTag(CountessTags::FAbilityTags::BackDashAbilityTag);

	/* loading BackDash sound cue*/
	static ConstructorHelpers::FObjectFinder<USoundCue> BDashSoundObject(TEXT("SoundCue'/Game/MyProjectMain/Audio/SFX-BackDash_Cue.SFX-BackDash_Cue'"));
	if (BDashSoundObject.Succeeded())
	{
		SoundCueToPlay = BDashSoundObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("Found BackDash Sound Cue!!! in %s. "), TEXT(__FUNCTION__));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find BackDash Sound Cue in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/* loading BDash emitter*/
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BDashParticleEffectObject(TEXT("ParticleSystem'/Game/MyProjectMain/Particles/P_BackDash.P_BackDash'"));
	if (BDashParticleEffectObject.Succeeded())
		EmitterToSpawn = BDashParticleEffectObject.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find particle effect in %s. Check if it is moved."), TEXT(__FUNCTION__));


	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/

	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/BackDashAbility/BP_AbilityData_BDash'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));


	static ConstructorHelpers::FObjectFinder<UAnimMontage> BackDashMontageObject(TEXT("AnimMontage'/Game/MyProjectMain/Animations/AnimMontage_Knock_Bwd.AnimMontage_Knock_Bwd'"));
	if (BackDashMontageObject.Succeeded())
		BackDashMontage = BackDashMontageObject.Object;
	else
		UE_LOG(Countess_Log, Warning, TEXT("Can't find Back Dash Anim Montage in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/*Our Jump Ability CoolDown Class*/

	CooldownGameplayEffectClass = UCountess_GE_BDash_CoolDown::StaticClass();
	CostGameplayEffectClass = UCountess_GE_BDash_Cost::StaticClass();
}

void UCountess_GameplayAbility_BDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		CommitAbility(Handle, ActorInfo, ActivationInfo);

		ACountess_Character_Player* MyCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());

		if (!MyCharacter)
		{
			UE_LOG(Countess_Log, Error, TEXT("CountessPlayerCharacter ie Avatar Actor not found in %s"), TEXT(__FUNCTION__));
			return;
		}

		UAbilityTask_PlayMontageAndWait* PlayMontagenWait = UAbilityTask_PlayMontageAndWait::NewAbilityTask<UAbilityTask_PlayMontageAndWait>(this, FName("Back Dash Montage"));
		
		if (BackDashMontage)
		{
			PlayMontagenWait = PlayMontagenWait->CreatePlayMontageAndWaitProxy(this, FName("Back Dash Montage"), BackDashMontage, 4.f);
		}

		PlayMontagenWait->Activate(); //Start montage. 
		MyCharacter->BeginBackDash(); //While montage is playing, tell character to move left/right depending on facing left/right by some amount.

		PlayMontagenWait->OnCompleted.AddDynamic(this, &UCountess_GameplayAbility_BDash::MontageEnded); // End Ability when montage ends.
		PlayMontagenWait->OnInterrupted.AddDynamic(this, &UCountess_GameplayAbility_BDash::MontageEnded);
		PlayMontagenWait->OnBlendOut.AddDynamic(this, &UCountess_GameplayAbility_BDash::MontageEnded);
		PlayMontagenWait->OnCancelled.AddDynamic(this, &UCountess_GameplayAbility_BDash::MontageEnded);


		if (SoundCueToPlay.IsValid(false))
		{
			UGameplayStatics::PlaySound2D(this, SoundCueToPlay.Get(false), 3.f);
		}
		if (EmitterToSpawn.IsValid(false))
		{
			UArrowComponent* ArrowComp = Cast<UArrowComponent>(MyCharacter->GetFeetLocationArrowComponent());
			UGameplayStatics::SpawnEmitterAttached(EmitterToSpawn.Get(false), ArrowComp, FName(NAME_None), FVector(0), FRotator(0), FVector(1.5f));
		}

	}
}

void UCountess_GameplayAbility_BDash::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UCountess_GameplayAbility_Base::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	//UE_LOG(Countess_Log, Warning, TEXT("Player Double Jump Ability was cancelled!! from %s"), TEXT(__FUNCTION__));
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	
}

void UCountess_GameplayAbility_BDash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCountess_GameplayAbility_BDash::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

void UCountess_GameplayAbility_BDash::MontageEnded()
{
	//UE_LOG(Countess_Log, Warning, TEXT("Back Dash Montage Completed. From %s"), TEXT(__FUNCTION__));
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
}
