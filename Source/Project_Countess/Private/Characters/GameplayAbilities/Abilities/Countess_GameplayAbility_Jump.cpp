// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Jump.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Characters/Player/Countess_Character_Player.h"
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

	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);

	AbilityTags.AddTag(CountessTags::FAbilityTags::JumpAbilityTag);

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

		ACountess_Character_Player* MyCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());

		if (!MyCharacter)
		{
			UE_LOG(Countess_Log, Error, TEXT("CountessPlayerCharacter ie Avatar Actor not found in %s"), TEXT(__FUNCTION__));
			return;
		}

		MyCharacter->Jump();
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(CountessTags::FStatusTags::JumpingTag);
		MyCharacter->LandedDelegate.AddDynamic(this, &UCountess_GameplayAbility_Jump::OnLanded); // This is correct way. But giving ensure fail errors. Fix it
		if (SoundToPlay.IsValid(false))
		{
			UGameplayStatics::PlaySound2D(this, SoundToPlay.Get(false), 3.f);
		}
		if (EmitterToSpawn.IsValid(false))
		{
			UArrowComponent* ArrowComp = Cast<UArrowComponent>(MyCharacter->GetFeetLocationArrowComponent());
			UGameplayStatics::SpawnEmitterAttached(EmitterToSpawn.Get(false), ArrowComp, FName(NAME_None), FVector(0), FRotator(0), FVector(0.5f));
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
	ACountess_Character_Player* MyCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
	if (MyCharacter)
	{
		MyCharacter->LandedDelegate.Remove(this, FName("OnLanded"));
	}
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(CountessTags::FStatusTags::JumpingTag);
	
}

void UCountess_GameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACountess_Character_Player* MyCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
	if (MyCharacter)
	{
		MyCharacter->LandedDelegate.Remove(this, FName("OnLanded"));
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(CountessTags::FStatusTags::JumpingTag);
}

void UCountess_GameplayAbility_Jump::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle,ActorInfo,ActivationInfo);
}

void UCountess_GameplayAbility_Jump::OnLanded(const FHitResult& Hit)
{
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
}
