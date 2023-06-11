// Copyright (c) Parallax 2022-2023

using UnrealBuildTool;
using System.Collections.Generic;

public class NewDawnTarget : TargetRules
{
    public NewDawnTarget( TargetInfo target )
        : base(target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("NewDawn");
    }
}