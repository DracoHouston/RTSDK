// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LockstepTestTarget : TargetRules
{
	public LockstepTestTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("LockstepTest");
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
	}
}
