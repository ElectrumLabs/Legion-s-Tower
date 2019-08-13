// Electrum Labs 2019

using UnrealBuildTool;
using System.Collections.Generic;

public class LegionsTowerEditorTarget : TargetRules
{
	public LegionsTowerEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "LegionsTower" } );
	}
}
