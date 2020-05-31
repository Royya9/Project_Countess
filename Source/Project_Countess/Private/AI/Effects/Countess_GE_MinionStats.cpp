// Free to play. By SoNa Games.


#include "AI/Effects/Countess_GE_MinionStats.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GE_MinionStats::UCountess_GE_MinionStats()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	if (MinionDetailsTable)
	{
		const uint32 NumOfMinionStatsToCapture = 11;

		TArray<FScalableFloat> ScalableFloats;
		ScalableFloats.SetNum(NumOfMinionStatsToCapture);

		TArray<FCurveTableRowHandle> CurveTableRowHandles;
		CurveTableRowHandles.SetNum(NumOfMinionStatsToCapture);

		TArray<FGameplayModifierInfo> ModifierInfos;
		ModifierInfos.SetNum(NumOfMinionStatsToCapture);

		if (Countess_AttributeSet)
		{
			CurveTableRowHandles[0].RowName = FName("MaxHealth");
			ModifierInfos[0].Attribute = Countess_AttributeSet->GetMaxHealthAttribute();

			CurveTableRowHandles[1].RowName = FName("MaxStamina");
			ModifierInfos[1].Attribute = Countess_AttributeSet->GetMaxStaminaAttribute();

			CurveTableRowHandles[2].RowName = FName("MaxMana");
			ModifierInfos[2].Attribute = Countess_AttributeSet->GetMaxManaAttribute();

			CurveTableRowHandles[3].RowName = FName("HealthRegenRate");
			ModifierInfos[3].Attribute = Countess_AttributeSet->GetHealthRegenRateAttribute();

			CurveTableRowHandles[4].RowName = FName("ManaRegenRate");
			ModifierInfos[4].Attribute = Countess_AttributeSet->GetManaRegenRateAttribute();

			CurveTableRowHandles[5].RowName = FName("StaminaRegenRate");
			ModifierInfos[5].Attribute = Countess_AttributeSet->GetStaminaRegenRateAttribute();

			CurveTableRowHandles[6].RowName = FName("Armor");
			ModifierInfos[6].Attribute = Countess_AttributeSet->GetArmorAttribute();

			CurveTableRowHandles[7].RowName = FName("Health");
			ModifierInfos[7].Attribute = Countess_AttributeSet->GetHealthAttribute();

			CurveTableRowHandles[8].RowName = FName("Mana");
			ModifierInfos[8].Attribute = Countess_AttributeSet->GetManaAttribute();

			CurveTableRowHandles[9].RowName = FName("Stamina");
			ModifierInfos[9].Attribute = Countess_AttributeSet->GetStaminaAttribute();

			CurveTableRowHandles[10].RowName = FName("MagicResistancePercentage");
			ModifierInfos[10].Attribute = Countess_AttributeSet->GetMagicResistanceAttribute();

		}

		for (uint32 i = 0; i < NumOfMinionStatsToCapture; i++)
		{
			CurveTableRowHandles[i].CurveTable = MinionDetailsTable;
			ScalableFloats[i] = FScalableFloat(1.f);
			ScalableFloats[i].Curve = CurveTableRowHandles[i];
			ModifierInfos[i].ModifierOp = EGameplayModOp::Override;
			ModifierInfos[i].ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloats[i]);
			Modifiers.Add(ModifierInfos[i]);
		}



	}
}
