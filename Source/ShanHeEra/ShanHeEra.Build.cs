using UnrealBuildTool;

public class ShanHeEra : ModuleRules
{
    public ShanHeEra(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore",
            "EnhancedInput", "UMG", "Slate", "SlateCore",
            "AIModule", "NavigationSystem", "GameplayTasks",
            "Niagara",
            "Json", "JsonUtilities", "HTTP", "DeveloperSettings"
        });

        PublicIncludePaths.AddRange(new string[] { "ShanHeEra" });
    }
}
