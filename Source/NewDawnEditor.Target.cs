// Copyright (c) Parallax 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class NewDawnEditorTarget : TargetRules
{
	public NewDawnEditorTarget( TargetInfo target )
		: base(target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "NewDawn" } );
	}
}