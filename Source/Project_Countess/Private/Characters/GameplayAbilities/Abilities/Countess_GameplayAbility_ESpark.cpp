// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_ESpark.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_ESpark_CoolDown.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_ESpark_Cost.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_ESpark_Damage.h"
#include "AbilitySystemComponent.h"
#include "Characters/GameplayAbilities/AbilityTasks/CT_PlayMontageAndWaitForEvent.h"
#include "Actors/Countess_Actor_ElectroSpark.h"
#include "Player/Countess_PlayerState.h"
#include "Characters/Player/Countess_Character_Player.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/Countess_Fireball_CameraShake.h"
#include "Components/CapsuleComponent.h"

UCountess_GameplayAbility_ESpark::UCountess_GameplayAbility_ESpark()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.ESpark")));

	/* loading ElectroSpark sound*/
	static ConstructorHelpers::FObjectFinder<USoundWave> ElectroSparkSoundObject(TEXT("SoundWave'/Game/MyProjectMain/Audio/SFX_ElectroSpark.SFX_ElectroSpark'"));
	if (ElectroSparkSoundObject.Succeeded())
		SoundToPlay = ElectroSparkSoundObject.Object;
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find ElectroSpark Sound wave in %s. Check if it is moved."), TEXT(__FUNCTION__));

	/*Find AbilityData from our Content Folder that describe this Ability as Class and get CDO from it*/
	static ConstructorHelpers::FClassFinder<UAbilityData> AbilityDataObject(TEXT("'/Game/MyProjectMain/Blueprints/Characters/Abilities/ElectroSparkAbility/BP_AbilityData_ESpark'"));
	if (AbilityDataObject.Succeeded())
	{
		AbilityData = Cast<UAbilityData>(AbilityDataObject.Class->GetDefaultObject(false));
		UE_LOG(Countess_Log, Warning, TEXT("Success. Ability Description is %s"), *AbilityData->Description.ToString());
	}
	else
		UE_LOG(Countess_Log, Error, TEXT("Corresponding AbilityData Not found.. in %s"), TEXT(__FUNCTION__));

	/*Load ElectroSpark Montage*/
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ElectroSparkMontageObject(TEXT("AnimMontage'/Game/MyProjectMain/Animations/AnimMontage_ESpark.AnimMontage_ESpark'"));
	if (ElectroSparkMontageObject.Succeeded())
		ElectroSparkMontage = ElectroSparkMontageObject.Object;
	else
		UE_LOG(Countess_Log, Warning, TEXT("Can't find ElectroSpark Anim Montage in %s. Check if it is moved."), TEXT(__FUNCTION__));


	CooldownGameplayEffectClass = UCountess_GE_ESpark_CoolDown::StaticClass();
	CostGameplayEffectClass = UCountess_GE_ESpark_Cost::StaticClass();
	ElectroSparkDamageEffectClass = UCountess_GE_ESpark_Damage::StaticClass();
	ElectroSparkClass = ACountess_Actor_ElectroSpark::StaticClass();
}

void UCountess_GameplayAbility_ESpark::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
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

		UGameplayStatics::PlayWorldCameraShake(this, UCountess_Fireball_CameraShake::StaticClass(), Player->GetCapsuleComponent()->GetComponentLocation(), 0.f, 1000.f);

		UCT_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent = UCT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
			this, FName("ESpark Montage"), ElectroSparkMontage, FGameplayTagContainer(), 1.f, NAME_None, false, 1.f
		);

		//Disables input and allows floating of Player
		Player->ElectroSparkOn();

		PlayMontageAndWaitForEvent->OnCompleted.AddDynamic(this, &UCountess_GameplayAbility_ESpark::OnCompleted);
		PlayMontageAndWaitForEvent->OnInterrupted.AddDynamic(this, &UCountess_GameplayAbility_ESpark::OnCompleted);
		PlayMontageAndWaitForEvent->OnBlendOut.AddDynamic(this, &UCountess_GameplayAbility_ESpark::OnCompleted);
		PlayMontageAndWaitForEvent->OnCancelled.AddDynamic(this, &UCountess_GameplayAbility_ESpark::OnCompleted);
		PlayMontageAndWaitForEvent->EventReceived.AddDynamic(this, &UCountess_GameplayAbility_ESpark::OnEventReceived);
		PlayMontageAndWaitForEvent->ReadyForActivation();
		

	}
}

void UCountess_GameplayAbility_ESpark::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UCountess_GameplayAbility_Base::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	//UE_LOG(Countess_Log, Warning, TEXT("Player Double Jump Ability was cancelled!! from %s"), TEXT(__FUNCTION__));
	ACountess_Character_Player* Player = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
	if (Player)
		Player->ElectroSparkOff(); //returns player to normal state

	if (ElectroSparkActor)
		ElectroSparkActor->Destroy();

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCountess_GameplayAbility_ESpark::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ACountess_Character_Player* Player = Cast<ACountess_Character_Player>(ActorInfo->AvatarActor.Get());
	if (Player)
		Player->ElectroSparkOff();

	if (ElectroSparkActor)
		ElectroSparkActor->Destroy();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCountess_GameplayAbility_ESpark::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

void UCountess_GameplayAbility_ESpark::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
}

void UCountess_GameplayAbility_ESpark::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	//UE_LOG(Countess_Log, Warning, TEXT("Event %s Received from %s"), *EventTag.ToString(), TEXT(__FUNCTION__)); 
// We received Event.Ability.ESpark.Cast Tag at the correct instant in montage which we setup in
// ElectroSparkAnimMontage:notify -> AnimBP:make event data & send to actor
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

	FGameplayEffectSpecHandle SpecHandle = Player->GetAbilitySystemComponent()->MakeOutgoingSpec(ElectroSparkDamageEffectClass, Countess_PlayerState->GetPlayerLevel(), EffectContextHandle);

	FTransform ElectroSparkSpawnTransform = Player->GetCapsuleComponent()->GetComponentTransform();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ElectroSparkActor = GetWorld()->SpawnActorDeferred<ACountess_Actor_ElectroSpark>(ElectroSparkClass, ElectroSparkSpawnTransform, GetOwningActorFromActorInfo(),
		Player, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	ElectroSparkActor->DamageEffectSpecHandle = SpecHandle;
	ElectroSparkActor->FinishSpawning(ElectroSparkSpawnTransform);

	if (SoundToPlay.IsValid(false))
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay.Get(false), ElectroSparkSpawnTransform.GetLocation());
}
