// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LeiCSTarget : TargetRules
{
	public LeiCSTarget( TargetInfo Target) : base(Target)
	{
		bOverrideBuildEnvironment = true;

        Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "LeiCS" } );
	}
}
