// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.Collections.Generic;
using System.Linq;
using UnrealBuildTool;

public class RTSDKEditor : ModuleRules
{
	public RTSDKEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
				
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "RTSDK",
				// ... add other public dependencies that you statically link with here ...
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "CoreUObject",
                "Engine",
                "InputCore",
                "Slate",
                "SlateCore",
				// ... add private dependencies that you statically link with here ...	
			});
    }
}
