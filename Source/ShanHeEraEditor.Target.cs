using UnrealBuildTool;
using System.Collections.Generic;

public class ShanHeEraEditorTarget : TargetRules
{
    public ShanHeEraEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        bOverrideBuildEnvironment = true;
        ExtraModuleNames.AddRange(new string[] { "ShanHeEra" });
    }
}
