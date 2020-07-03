// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class EditorUtilities : ModuleRules
{
	public EditorUtilities(ReadOnlyTargetRules Target)
		: base(Target)
	{
		PrivatePCHHeaderFile = "Public/EditorUtilities.h";

		//PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(new string[] { System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "Source/Editor/Blutility/Private" });
		PrivateIncludePaths.Add("EditorUtilities/Private");

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Blutility",
				"EditorScriptingUtilities",
				"UMG"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"InputCore",
				"Niagara"
			}
		);
	}
}
