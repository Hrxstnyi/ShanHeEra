using UnrealBuildTool;
using System.Collections.Generic;

public class ShanHeEraTarget : TargetRules
{
    public ShanHeEraTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V3;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        ExtraModuleNames.AddRange(new string[] { "ShanHeEra" });
    }
}
