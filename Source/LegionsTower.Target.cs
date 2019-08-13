// Electrum Labs 2019

using UnrealBuildTool;
using System.Collections.Generic;

public class LegionsTowerTarget : TargetRules
{
	public LegionsTowerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "LegionsTower" } );
	}
}
