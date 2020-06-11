// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_DJump.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ArrowComponent.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_DJump_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_DJump_Cost.h"

UCountess_GameplayAbility_DJump::UCountess_GameplayAbility_DJump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);

	AbilityTags.AddTag(CountessTags::FAbilityTags::DoubleJumpAbilityTag);

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

	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/DoubleJumpAbility/BP_AbilityData_DJump'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	/*Our Jump Ability CoolDown Class*/

	CooldownGameplayEffectClass = UCountess_GE_DJump_CoolDown::StaticClass();
	CostGameplayEffectClass = UCountess_GE_DJump_Cost::StaticClass();
}

void UCountess_GameplayAbility_DJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

		FVector Velocity = MyCharacter->GetVelocity();
		FVector LaunchVelocity = FVector(Velocity.X, Velocity.Y, 1200.f); // 1200 is hardcoded here. #TODO may be bind this to any attribute? 
		MyCharacter->LaunchCharacter(LaunchVelocity, false, false);
		MyCharacter->SetIsDoubleJumping(true); // This value is in AnimBP to play appropriate animation
		//UE_LOG(Countess_Log, Warning, TEXT("Player just Double Jumped!! from %s"), TEXT(__FUNCTION__));
		GetAbilitySystemComponentFromActorInfo()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Jumping")));
		MyCharacter->LandedDelegate.AddDynamic(this, &UCountess_GameplayAbility_DJump::OnLanded); // This is correct way but giving ensure errors. fix it.
		
		if (SoundToPlay.IsValid(false))
		{
			UGameplayStatics::PlaySound2D(this, SoundToPlay.Get(false), 3.f);
		}
		if (EmitterToSpawn.IsValid(false))
		{
			UArrowComponent* ArrowComp = Cast<UArrowComponent>(MyCharacter->GetFeetLocationArrowComponent());
			UGameplayStatics::SpawnEmitterAttached(EmitterToSpawn.Get(false), ArrowComp, FName(NAME_None), FVector(0), FRotator(0), FVector(1.5f));
		}

	}
}

void UCountess_GameplayAbility_DJump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UCountess_GameplayAbility_Base::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	//UE_LOG(Countess_Log, Warning, TEXT("Player Double Jump Ability was cancelled!! from %s"), TEXT(__FUNCTION__));
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Jumping")));
}

void UCountess_GameplayAbility_DJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACountess_Character_Player* MyCharacter = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
	if (MyCharacter)
	{
		MyCharacter->LandedDelegate.Remove(this, FName("OnLanded"));
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Jumping")));
}

void UCountess_GameplayAbility_DJump::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

void UCountess_GameplayAbility_DJump::OnLanded(const FHitResult& Hit)
{
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
}

