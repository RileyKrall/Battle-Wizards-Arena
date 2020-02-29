// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WizardBattle : ModuleRules
{
	public WizardBattle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AdvancedSessions", "AdvancedSteamSessions", "OnlineSubsystem", "OnlineSubsystemUtils", "ProceduralMeshComponent" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    }
}
