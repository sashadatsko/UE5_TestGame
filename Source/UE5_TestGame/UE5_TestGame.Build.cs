// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE5_TestGame : ModuleRules
{
	public UE5_TestGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
