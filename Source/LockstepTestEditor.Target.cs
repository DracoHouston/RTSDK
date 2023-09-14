// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LockstepTestEditorTarget : TargetRules
{
	public LockstepTestEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("LockstepTest");
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
	}
}
