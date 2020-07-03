// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "Engine/Engine.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

/** Module interface for this game's loading screens */
class FEditorUtilitiesModule : public IModuleInterface
{
public:
	/** Loads the module so it can be turned on */
/*
	static inline IEditorUtilitiesModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IEditorUtilitiesModule>("EditorUtilities");
	}*/

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
