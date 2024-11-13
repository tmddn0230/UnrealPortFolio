// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UPCore : ModuleRules
{
    public UPCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "ProceduralMeshComponent",
            "EnhancedInput",
            "InputCore",
            "Json",
            "JsonUtilities",
            "EnhancedInput",
            "Niagara",
            "Slate",
            "SlateCore",
            "HTTP",
            "Networking",
            "Sockets"
		});
    }
}