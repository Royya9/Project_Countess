// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Countess : ModuleRules
{
	public Project_Countess(ReadOnlyTargetRules Target) : base(Target)
	{
		//PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		//PCHUsage = PCHUsageMode.NoSharedPCHs;
		PrivatePCHHeaderFile = "Public/Globals/Project_Countess.h";

		PublicDependencyModuleNames.AddRange(new string[] 
			{ "Core", "CoreUObject", "Engine" });
		
		PrivateDependencyModuleNames.AddRange(new string[] 
			{ "InputCore", "GameplayAbilities", "GameplayTasks", "GameplayTags", "Slate", "SlateCore", "AIModule"});
		
		CppStandard = CppStandardVersion.Cpp17;
	}
}
