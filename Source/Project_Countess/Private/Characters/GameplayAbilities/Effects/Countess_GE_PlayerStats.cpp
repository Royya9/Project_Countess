// Free to play. By SoNa Games.


#include "Characters/GameplayAbilities/Effects/Countess_GE_PlayerStats.h"
#include "Characters/GameplayAbilities/AttributeSets/Countess_AttributeSet_Base.h"

UCountess_GE_PlayerStats::UCountess_GE_PlayerStats()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	if (AbilityDetailsTable)
	{
		const uint32 NumOfPlayerStatsToCapture = 12;

		TArray<FScalableFloat> ScalableFloats;
		ScalableFloats.SetNum(NumOfPlayerStatsToCapture);

		TArray<FCurveTableRowHandle> CurveTableRowHandles;
		CurveTableRowHandles.SetNum(NumOfPlayerStatsToCapture);

		TArray<FGameplayModifierInfo> ModifierInfos;
		ModifierInfos.SetNum(NumOfPlayerStatsToCapture);

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

			CurveTableRowHandles[7].RowName = FName("MagicResistancePercentage");
			ModifierInfos[7].Attribute = Countess_AttributeSet->GetMagicResistanceAttribute();

			CurveTableRowHandles[8].RowName = FName("MaxExp");
			ModifierInfos[8].Attribute = Countess_AttributeSet->GetMaxExpAttribute();

			CurveTableRowHandles[9].RowName = FName("Exp");
			ModifierInfos[9].Attribute = Countess_AttributeSet->GetExpAttribute();

			CurveTableRowHandles[10].RowName = FName("FireballAbilityDamage");
			ModifierInfos[10].Attribute = Countess_AttributeSet->GetFireballDamageAttribute();

			CurveTableRowHandles[11].RowName = FName("ElectroSparkAbilityDamage");
			ModifierInfos[11].Attribute = Countess_AttributeSet->GetElectroSparkDamageAttribute();

		}

		for (uint32 i = 0; i < NumOfPlayerStatsToCapture; i++)
		{
			CurveTableRowHandles[i].CurveTable = AbilityDetailsTable;
			ScalableFloats[i] = FScalableFloat(1.f);
			ScalableFloats[i].Curve = CurveTableRowHandles[i];
			ModifierInfos[i].ModifierOp = EGameplayModOp::Override;
			ModifierInfos[i].ModifierMagnitude = FGameplayEffectModifierMagnitude(ScalableFloats[i]);
			Modifiers.Add(ModifierInfos[i]);
		}



	}

}
