// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LockstepTest : ModuleRules
{
	public LockstepTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"NavigationSystem", 
			"AIModule", 
			"Niagara", 
			"MassEntity", 
			"MassActors",
			"MassCommon",
			"MassSpawner", 
			"StructUtils",
            "DeterministicMath",
            "FixedPoint",
            "DeterministicFloat",
			"CommonUI",
            "GameSettings",
            "RTSDK",
            "RTSDKUI",
            "RTSDKGameActors",
            "RTSDKMovementCore",
            "RTSDKComplexMovement",
        });
    }
}
