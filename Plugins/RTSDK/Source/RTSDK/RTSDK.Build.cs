// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.Collections.Generic;
using System.Linq;
using UnrealBuildTool;

public class RTSDK : ModuleRules
{
	public RTSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
				
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "NetCore",
                "Chaos",
                "DeveloperSettings",
                "InputCore",
                "EnhancedInput",
				"UMG",
                "GameplayTags",
                "MassEntity",
				"MassActors",
				"MassCommon",
				"MassSpawner",
                "MassSignals",
                "CommonUI",                
                "GameFeatures",
                "DeterministicMath",
                "FixedPoint",
                "DeterministicFloat",
                "GameSettings",
				"CommonGame",
                "ModularGameplayActors",
                "CommonLoadingScreen",
                "RTSDKModularGameplay",
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
		//sets default plugin option defines if the target does not
        if ((!Target.GlobalDefinitions.Any(x => x.StartsWith("RTSDK_USE_FIXED_POINT", StringComparison.Ordinal))) && (!Target.ProjectDefinitions.Any(x => x.StartsWith("RTSDK_USE_FIXED_POINT", StringComparison.Ordinal))))
		{
			PublicDefinitions.Add("RTSDK_USE_FIXED_POINT=0");
		}
        if ((!Target.GlobalDefinitions.Any(x => x.StartsWith("RTSDK_USE_SERVER_CLIENT", StringComparison.Ordinal))) && (!Target.ProjectDefinitions.Any(x => x.StartsWith("RTSDK_USE_SERVER_CLIENT", StringComparison.Ordinal))))
        {
            PublicDefinitions.Add("RTSDK_USE_SERVER_CLIENT=0");
        }
    }
}
