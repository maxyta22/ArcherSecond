// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArcherPRT : ModuleRules
{
	public ArcherPRT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{	"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay",
			"GameplayTasks",
			"NavigationSystem",
			"Niagara",
			"GameplayTasks",
			"NavigationSystem",
			"GameplayAbilities",
			"GameplayTags"
		});

	}
}
