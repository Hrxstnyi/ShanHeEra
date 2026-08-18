#include "Core/DebugConsole.h"
#include "Core/ShanHeLog.h"

void UDebugConsole::InitializeConsole()
{
    struct FCmdDef { FName Cmd; const TCHAR* Desc; bool Cheat; };
    FCmdDef CmdDefs[] = {
        {TEXT("help"), NSLOCTEXT("Cmd","help","显示所有命令"), false},
        {TEXT("status"), NSLOCTEXT("Cmd","status","显示世界状态"), false},
        {TEXT("tp"), NSLOCTEXT("Cmd","tp","传送到城市"), true},
        {TEXT("money"), NSLOCTEXT("Cmd","money","增加银两"), true},
        {TEXT("god"), NSLOCTEXT("Cmd","god","切换无敌模式"), true},
        {TEXT("time"), NSLOCTEXT("Cmd","time","设置时间流速"), true},
        {TEXT("kill"), NSLOCTEXT("Cmd","kill","杀死目标"), true},
        {TEXT("heal"), NSLOCTEXT("Cmd","heal","恢复满血"), true},
        {TEXT("weather"), NSLOCTEXT("Cmd","weather","设置天气"), true},
        {TEXT("event"), NSLOCTEXT("Cmd","event","触发事件"), true},
    };
    for (const auto& C : CmdDefs)
    {
        FDebugCommand Cmd;
        Cmd.Command = C.Cmd; Cmd.Description = C.Desc; Cmd.bCheat = C.Cheat;
        Commands.Add(Cmd);
    }
    UE_LOG(LogShanHe, Log, TEXT("调试控制台初始化 - %d 条命令"), Commands.Num());
}

FString UDebugConsole::ExecuteCommand(const FString& Command)
{
    FString Result;
    TArray<FString> Parts;
    Command.ParseIntoArray(Parts, TEXT(" "), true);
    if (Parts.Num() == 0) return TEXT("无效命令");

    FString Cmd = Parts[0].ToLower();
    if (Cmd == TEXT("help")) Result = ProcessHelp();
    else if (Cmd == TEXT("status")) Result = ProcessStatus();
    else if (Cmd == TEXT("tp") && Parts.Num() > 1) Result = ProcessTeleport(Parts[1]);
    else if (Cmd == TEXT("money") && Parts.Num() > 1) { AddSilver(FCString::Atoi(*Parts[1])); Result = FString::Printf(TEXT("增加 %d 银两"), FCString::Atoi(*Parts[1])); }
    else if (Cmd == TEXT("god")) { ToggleGodMode(); Result = bGodMode ? TEXT("无敌模式开启") : TEXT("无敌模式关闭"); }
    else Result = TEXT("未知命令，输入 help 查看所有命令");

    OnOutput.Broadcast(Result);
    return Result;
}

FString UDebugConsole::GetWorldStatus()
{
    return TEXT("世界状态：运行中");
}

void UDebugConsole::AddSilver(int32 Amount)
{
    UE_LOG(LogShanHe, Log, TEXT("调试：增加 %d 银两"), Amount);
}

void UDebugConsole::SetTimeScale(float Scale)
{
    UE_LOG(LogShanHe, Log, TEXT("调试：时间流速 %.1fx"), Scale);
}

FString UDebugConsole::ProcessHelp()
{
    FString Result = TEXT("可用命令：\n");
    for (const FDebugCommand& C : Commands)
        Result += FString::Printf(TEXT("  %s - %s\n"), *C.Command.ToString(), *C.Description.ToString());
    return Result;
}

FString UDebugConsole::ProcessStatus()
{
    return GetWorldStatus();
}

FString UDebugConsole::ProcessTeleport(const FString& Args)
{
    return FString::Printf(TEXT("传送到：%s"), *Args);
}
