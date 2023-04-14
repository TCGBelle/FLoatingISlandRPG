// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FLoatingISlandRPG : ModuleRules
{
	public FLoatingISlandRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Slate", "SlateCore" });
	}
}
