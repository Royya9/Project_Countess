// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Jump.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Characters/Project_CountessCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ArrowComponent.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Jump_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Jump_Cost.h"

UCountess_GameplayAbility_Jump::UCountess_GameplayAbility_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));

	/* loading jump sound*/
	static ConstructorHelpers::FObjectFinder<USoundWave> JumpSoundObject(TEXT("SoundWave'/Game/ParagonCountess/Characters/Heroes/Countess_Sounds/Sounds/SoundWaves/Countess_Effort_Ability_Primary_02.Countess_Effort_Ability_Primary_02'"));
	if (JumpSoundObject.Succeeded())
		SoundToPlay = JumpSoundObject.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find Jump Sound wave in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/* loading jump emitter*/
	static ConstructorHelpers::FObjectFinder<UParticleSystem> JumpParticleEffectObject(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Sentinel/P_Sentinel_Leap_Launch_Trail.P_Sentinel_Leap_Launch_Trail'"));
	if (JumpParticleEffectObject.Succeeded())
		EmitterToSpawn = JumpParticleEffectObject.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find particle effect in %s. Check if it is moved."), TEXT(__FUNCTION__));

	
	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/

	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/JumpAbility/BP_AbilityData_Jump'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	/*Our Jump Ability CoolDown Class*/
	
	CooldownGameplayEffectClass = UCountess_GE_Jump_CoolDown::StaticClass();
	CostGameplayEffectClass = UCountess_GE_Jump_Cost::StaticClass();
	//CooldownGameplayEffectClass-> = AbilityData->Cooldown;

}

void UCountess_GameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
		CommitAbility(Handle, ActorInfo, ActivationInfo);

		AProject_CountessCharacter* MyCharacter = Cast<AProject_CountessCharacter>(ActorInfo->AvatarActor.Get());

		if (!MyCharacter)
		{
			UE_LOG(Countess_Log, Error, TEXT("CountessPlayerCharacter ie Avatar Actor not found in %s"), TEXT(__FUNCTION__));
			return;
		}

		MyCharacter->Jump();
		if (SoundToPlay)
		{
			UGameplayStatics::PlaySound2D(this, SoundToPlay, 3.f);
		}
		if (EmitterToSpawn)
		{
			UArrowComponent* ArrowComp = Cast<UArrowComponent>(MyCharacter->GetFeetLocationArrowComponent());
			UGameplayStatics::SpawnEmitterAttached(EmitterToSpawn, ArrowComp, FName(NAME_None), FVector(0), FRotator(0), FVector(0.5f));
		}

	}
}

void UCountess_GameplayAbility_Jump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UCountess_GameplayAbility_Base::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	//AProject_CountessCharacter* MyCharacter = CastChecked<AProject_CountessCharacter>(ActorInfo->AvatarActor.Get());
	AProject_CountessCharacter* MyCharacter = Cast<AProject_CountessCharacter>(ActorInfo->AvatarActor.Get());

	if (!MyCharacter)
	{
		UE_LOG(Countess_Log, Error, TEXT("CountessPlayerCharacter ie Avatar Actor not found in %s"), TEXT(__FUNCTION__));
		return;
	}
	MyCharacter->StopJumping();
}

void UCountess_GameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCountess_GameplayAbility_Jump::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle,ActorInfo,ActivationInfo);
}
