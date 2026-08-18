using UnrealBuildTool;
using System.Collections.Generic;

public class ShanHeEraEditorTarget : TargetRules
{
    public ShanHeEraEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V3;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        ExtraModuleNames.AddRange(new string[] { "ShanHeEra" });
    }
}
