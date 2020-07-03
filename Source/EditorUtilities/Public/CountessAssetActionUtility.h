// Free to play. By SoNa Games.

#pragma once

#include "EditorUtilities.h"
#include "AssetActionUtility.h"
#include "Sound/SoundCue.h"
#include "NiagaraSystem.h"
#include "CountessAssetActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class EDITORUTILITIES_API UCountessAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:

	UFUNCTION(CallInEditor)
	void RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase);
	
	UFUNCTION(CallInEditor)
	void CheckPowerOfTwo();

	UFUNCTION(CallInEditor)
	void AddPreFixes();

	UFUNCTION(CallInEditor)
	void CleanUpFolder(FString ParentFolder = FString("/Game"));

	UFUNCTION(CallInEditor)
	void RemoveUnusedAssets(bool bDeleteImmediately);

private:

	void PrintToScreen(FString Message, FColor Color);

	void GiveFeedback(FString Method, uint32 Counter);

	bool IsPowerOfTwo(int32 NumberToCheck);

	const TMap<UClass*, FString> PrefixMap = {
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UStaticMesh::StaticClass(), TEXT("SM_")},
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstance::StaticClass(), TEXT("MI_")},
		{UParticleSystem::StaticClass(), TEXT("P_")},
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("S_")},
		{UNiagaraSystem::StaticClass(), TEXT("NS_")},
		{UNiagaraEmitter::StaticClass(), TEXT("NE_")}
	};
};
