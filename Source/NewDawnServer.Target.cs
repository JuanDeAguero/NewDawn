// Copyright (c) Parallax 2022-2023

using UnrealBuildTool;
using System.Collections.Generic;

public class NewDawnServerTarget : TargetRules
{
    public NewDawnServerTarget( TargetInfo target )
        : base(target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("NewDawn");
    }
}