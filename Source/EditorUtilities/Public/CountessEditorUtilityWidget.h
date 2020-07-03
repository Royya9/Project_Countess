// Free to play. By SoNa Games.

#pragma once

#include "EditorUtilities.h"
#include "EditorUtilityWidget.h"
#include "Engine/SpotLight.h"
#include "Engine/PointLight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/ReflectionCapture.h"
#include "CountessEditorUtilityWidget.generated.h"

/**
 * 
 */
UCLASS()
class EDITORUTILITIES_API UCountessEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OrganizeWorldOutliner")
	TMap<UClass*, FName> FolerMap = {
		{AStaticMeshActor::StaticClass(), "Props"},
		{AReflectionCapture::StaticClass(), "ReflectionCaptures"},
		{APointLight::StaticClass(), "Point Lights"},
		{ASpotLight::StaticClass(), "Spot Lights"}
	};

	UFUNCTION(CallInEditor, BlueprintCallable)
	void OrganizeWorldOutliner();
};
