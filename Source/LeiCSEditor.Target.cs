// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LeiCSEditorTarget : TargetRules
{
	public LeiCSEditorTarget( TargetInfo Target) : base(Target)
	{
        bOverrideBuildEnvironment = true;
        Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "LeiCS" } );
	}
}
