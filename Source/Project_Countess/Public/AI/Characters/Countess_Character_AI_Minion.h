// Free to play. By SoNa Games.

#pragma once

#include "Globals/Project_Countess.h"
#include "Characters/Countess_Character_Base.h"
#include "GameplayEffectTypes.h"
#include "Countess_Character_AI_Minion.generated.h"

class UCountess_AbilitySystemComponent;
class UCountess_AI_Widget;
class UWidgetComponent;
class UCountess_AttributeSet_Base;
class UBehaviorTree;

/*AI Minion Class :) */
UCLASS()
class PROJECT_COUNTESS_API ACountess_Character_AI_Minion : public ACountess_Character_Base
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACountess_Character_AI_Minion();

	UCountess_AI_Widget* Minion_Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UWidgetComponent* Minion_Widget_Component;

	TSubclassOf<UCountess_AI_Widget> Minion_Widget_Class;

	int32 GetMinionLevel() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* MinionBehaviorTree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void GiveStartupEffects();

	virtual bool IsAlive() const override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Countesss | Minion", meta = (AllowPrivateAccess = "true"))
	UCountess_AbilitySystemComponent* MinionAbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Countesss | Minion", meta = (AllowPrivateAccess = "true"))
	UCountess_AttributeSet_Base* MinionAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Minion | Level", meta = (AllowPrivateAccess = "true"))
	int32 MinionLevel;

public:

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Minion | ASC")
	UCountess_AbilitySystemComponent* GetMinionAbilitySystemComponent();
};
