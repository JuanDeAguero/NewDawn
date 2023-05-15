// Copyright (c) Parallax 2022

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
			"Voxel",
			"VoxelGraph"
        });
		
		PrivateDependencyModuleNames.AddRange( new string[] {} );
	}
}