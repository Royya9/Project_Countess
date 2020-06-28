// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Abilities/Countess_GameplayAbility_Primary.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Primary_Damage.h"
#include "Characters/GameplayAbilities/Effects/Countess_GE_Primary_CoolDown.h"
#include "Characters/Countess_Character_Base.h"
#include "Characters/GameplayAbilities/AbilityTasks/CT_PlayMontageAndWaitForEvent.h"
#include "AbilitySystemComponent.h"

UCountess_GameplayAbility_Primary::UCountess_GameplayAbility_Primary()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    ActivationBlockedTags.AddTag(CountessTags::FStatusTags::StunTag);
    AbilityTags.AddTag(CountessTags::FAbilityTags::PrimaryAbilityTag);

    static ConstructorHelpers::FObjectFinder<UAnimMontage> PrimaryAttackMontageObject(TEXT("AnimMontage'/Game/MyProjectMain/Animations/Primary_Attack_A_Normal_Montage.Primary_Attack_A_Normal_Montage'"));
    if(PrimaryAttackMontageObject.Succeeded())
        PrimaryAttackAnimMontage = PrimaryAttackMontageObject.Object;
    else
        UE_LOG(Countess_Log, Error, TEXT("Can't find PrimaryAttackMontage in %s. Please check if it is moved!"), TEXT(__FUNCTION__));


    CooldownGameplayEffectClass = UCountess_GE_Primary_CoolDown::StaticClass();
    DamageEffectClass = UCountess_GE_Primary_Damage::StaticClass();
}    

void UCountess_GameplayAbility_Primary::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    }
    CommitAbility(Handle, ActorInfo, ActivationInfo);
// 	float cooldownduration = this->GetCooldownTimeRemaining(ActorInfo);
// 	UE_LOG(Countess_Log, Warning, TEXT("From %s. Cooldown remaining is %f"), TEXT(__FUNCTION__), cooldownduration);

    ACountess_Character_Base* Character = Cast<ACountess_Character_Base>(ActorInfo->AvatarActor.Get());

    if (!Character)
    {
        UE_LOG(Countess_Log, Error, TEXT("CountessCharacterBase ie Avatar Actor not found in %s"), TEXT(__FUNCTION__));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    UCT_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent = UCT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
        this, FName("Fireball Montage"), PrimaryAttackAnimMontage, FGameplayTagContainer(), 1.f, NAME_None, false, 1.f
    );
    PlayMontageAndWaitForEvent->OnCompleted.AddDynamic(this, &UCountess_GameplayAbility_Primary::OnCompleted);
    PlayMontageAndWaitForEvent->OnInterrupted.AddDynamic(this, &UCountess_GameplayAbility_Primary::OnCompleted);
    PlayMontageAndWaitForEvent->OnBlendOut.AddDynamic(this, &UCountess_GameplayAbility_Primary::OnCompleted);
    PlayMontageAndWaitForEvent->OnCancelled.AddDynamic(this, &UCountess_GameplayAbility_Primary::OnCompleted);
    PlayMontageAndWaitForEvent->EventReceived.AddDynamic(this, &UCountess_GameplayAbility_Primary::OnEventReceived);
    PlayMontageAndWaitForEvent->ReadyForActivation();
}

void UCountess_GameplayAbility_Primary::CancelAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UCountess_GameplayAbility_Primary::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCountess_GameplayAbility_Primary::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
    Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

void UCountess_GameplayAbility_Primary::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), false, false);
}

void UCountess_GameplayAbility_Primary::OnEventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    /*
    UE_LOG(Countess_Log, Warning, TEXT("Event with tag %s received from Primary Ability."), *EventTag.ToString());
    if(EventData.Target && EventData.Instigator)
        UE_LOG(Countess_Log, Warning, TEXT("Target is %s and Instigator is %s"), *EventData.Target->GetFName().ToString(), *EventData.Instigator->GetFName().ToString());
        */

    const ACountess_Character_Base* Instigator = Cast<ACountess_Character_Base>(EventData.Instigator);
    const ACountess_Character_Base* Target = Cast<ACountess_Character_Base>(EventData.Target);
    if(!Instigator || !Target)
    {
        EndAbility(this->GetCurrentAbilitySpecHandle(), this->GetCurrentActorInfo(), this->GetCurrentActivationInfo(), true, false);
        return;
    }
    
    UAbilitySystemComponent* InstigatorASC = Instigator->GetAbilitySystemComponent();
    UAbilitySystemComponent* TargetASC = Target->GetAbilitySystemComponent();

    FGameplayEffectContextHandle EffectContextHandle = InstigatorASC->MakeEffectContext();
    EffectContextHandle.AddSourceObject(Instigator);

    const FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(DamageEffectClass, Instigator->GetCharacterLevel(), EffectContextHandle);
    TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

