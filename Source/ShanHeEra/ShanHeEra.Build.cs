using UnrealBuildTool;

public class ShanHeEra : ModuleRules
{
    public ShanHeEra(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;
        bEnableExceptions = true;
        bUseUnity = false; // 禁用unity build，便于排查编译错误

        // 公共依赖：其他模块需要访问的类型
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "GameplayTags",      // 游戏标签系统，用于状态/事件/分类
            "UMG",
            "Slate",
            "SlateCore",
            "Niagara",
            "Json",
            "JsonUtilities",
            "DeveloperSettings"
        });

        // 私有依赖：仅本模块内部使用
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "AIModule",
            "NavigationSystem",
            "GameplayTasks",
            "HTTP",
            "RenderCore",
            "RHI",
            "Projects"
        });

        // 公共包含路径
        PublicIncludePaths.AddRange(new string[]
        {
            "ShanHeEra",
            "ShanHeEra/Core",
            "ShanHeEra/World",
            "ShanHeEra/Character"
        });
    }
}
