// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LeiCS : ModuleRules
{
	public LeiCS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTags", "AIModule", "EnhancedInput" });
	}
}
