// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.Collections.Generic;
using System.Linq;
using UnrealBuildTool;

public class RTSDKUI : ModuleRules
{
	public RTSDKUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
				
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "NetCore",
                "InputCore",
                "Chaos",
                "DeveloperSettings",
                "EnhancedInput",
				"UMG",
				"GameplayTags",
                "MassEntity",
				"MassActors",
				"MassCommon",
				"MassSpawner",
                "MassSignals",
                "DeterministicMath",
                "FixedPoint",
                "DeterministicFloat",
                "CommonUI",
                "GameSettings",
                "CommonGame",
                "ModularGameplayActors",
                "RTSDK"
				// ... add other public dependencies that you statically link with here ...
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "PhysicsCore",
				// ... add private dependencies that you statically link with here ...	
			});
    }
}
