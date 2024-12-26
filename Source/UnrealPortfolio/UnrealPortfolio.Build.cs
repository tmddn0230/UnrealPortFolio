// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealPortfolio : ModuleRules
{
	public UnrealPortfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
		    "UPCore",
            "UMG",
            "Slate",          //Loading Screen
            "SlateCore",      //Loading Screen
            "PreLoadScreen",  //Loading Screen
            "XmlParser",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
            "NavigationSystem",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "Json",
            "JsonUtilities",
            "HTTP",
            "Networking",
            "Sockets",
            "AudioMixer",
            "AudioCapture",
            "Voice",
            "SpeechRecognition",
            "RuntimeAudioImporter",
            "OpenAIAPI",
            "RuntimeSpeechRecognizer"
            //"ZipUtility",
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "OnlineSubsystem",
            "OnlineSubsystemUtils"
        });

        ////////////////////////////////////////////////////////////////////////////////////////////////
        //// opus 외부 라이브러리 추가 
        ////////////////////////////////////////////////////////////////////////////////////////////////
        //
        //string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/"));
        //string IncludePath = ThirdPartyPath + "Opus/include/";
        //string LibraryPath = ThirdPartyPath + "Opus/lib/";
        //
        //PublicIncludePaths.Add(IncludePath);
        //
        //PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, "opus.lib"));

    }
}
