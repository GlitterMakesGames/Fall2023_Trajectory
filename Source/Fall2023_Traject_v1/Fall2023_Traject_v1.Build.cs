// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Fall2023_Traject_v1 : ModuleRules
{
	public Fall2023_Traject_v1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
