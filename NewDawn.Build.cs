// Copyright (c) Parallax 2022-2023

using UnrealBuildTool;

public class NewDawn : ModuleRules
{
    public NewDawn( ReadOnlyTargetRules target )
        : base(target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange( new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Voxel"
        });
    }
}