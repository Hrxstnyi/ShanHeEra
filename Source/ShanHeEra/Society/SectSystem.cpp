#include "Society/SectSystem.h"
#include "Core/ShanHeLog.h"

void USectSystem::InitializeSects()
{
    struct FSectDef { FName ID; const TCHAR* Name; const TCHAR* Desc; int32 Members; int32 Influence; const TCHAR* Location; };
    FSectDef Sects[] = {
        {TEXT("shaolin"), TEXT("少林寺"), TEXT("天下第一名刹，武学正宗。"), 500, 9000, TEXT("嵩山")},
        {TEXT("wudang"), TEXT("武当派"), TEXT("道家圣地，以柔克刚。"), 300, 8000, TEXT("武当山")},
        {TEXT("emei"), TEXT("峨眉派"), TEXT("巾帼不让须眉，女子武学。"), 200, 6000, TEXT("峨眉山")},
        {TEXT("gaibang"), TEXT("丐帮"), TEXT("天下第一大帮，弟子遍布天下。"), 1000, 7000, TEXT("君山")},
        {TEXT("huashan"), TEXT("华山派"), TEXT("剑派翘楚，剑宗气宗之争。"), 150, 5500, TEXT("华山")},
        {TEXT("mingjiao"), TEXT("明教"), TEXT("波斯传入，行事诡秘。"), 400, 7500, TEXT("光明顶")},
        {TEXT("tangmen"), TEXT("唐门"), TEXT("暗器毒术，天下无双。"), 100, 5000, TEXT("蜀中")},
        {TEXT("quanzhen"), TEXT("全真教"), TEXT("道教正宗，内功深厚。"), 250, 6500, TEXT("终南山")},
    };

    for (const auto& S : Sects)
    {
        FSect Sect;
        Sect.SectID = S.ID;
        Sect.SectName = FText::FromString(UTF8_TO_TCHAR(S.Name));
        Sect.Description = FText::FromString(UTF8_TO_TCHAR(S.Desc));
        Sect.MemberCount = S.Members; Sect.Influence = S.Influence;
        Sect.BaseLocation = FName(S.Location);
        SectDatabase.Add(S.ID, Sect);
    }

    UE_LOG(LogShanHe, Log, TEXT("帮派系统初始化 - %d 个门派"), SectDatabase.Num());
}

bool USectSystem::JoinSect(FName SectID)
{
    if (CurrentSect != NAME_None) return false;
    if (FSect* S = SectDatabase.Find(SectID))
    {
        CurrentSect = SectID;
        S->PlayerRank = ESectRank::Disciple;
        S->MemberCount++;
        UE_LOG(LogShanHe, Log, TEXT("加入门派：%s"), *SectID.ToString());
        return true;
    }
    return false;
}

bool USectSystem::CreateSect(FText SectName, FName BaseLocation)
{
    if (CurrentSect != NAME_None) return false;
    FName NewID = FName(*FString::Printf(TEXT("custom_%d"), FMath::Rand()));
    FSect NewSect;
    NewSect.SectID = NewID; NewSect.SectName = SectName;
    NewSect.BaseLocation = BaseLocation;
    NewSect.MemberCount = 1; NewSect.Influence = 100; NewSect.Wealth = 500;
    NewSect.PlayerRank = ESectRank::Leader;
    SectDatabase.Add(NewID, NewSect);
    CurrentSect = NewID;
    UE_LOG(LogShanHe, Log, TEXT("创建门派：%s"), *SectName.ToString());
    return true;
}

bool USectSystem::CompleteMission(FName MissionID)
{
    if (FSect* S = SectDatabase.Find(CurrentSect))
    {
        S->Influence += 10;
        S->Wealth += 50;
        UE_LOG(LogShanHe, Log, TEXT("完成门派任务：%s"), *MissionID.ToString());
        return true;
    }
    return false;
}

bool USectSystem::ChallengeLeadership()
{
    if (CurrentSect == NAME_None) return false;
    if (FSect* S = SectDatabase.Find(CurrentSect))
    {
        // 需要影响力足够
        if (S->Influence >= 5000)
        {
            S->PlayerRank = ESectRank::Leader;
            UE_LOG(LogShanHe, Log, TEXT("成为掌门：%s"), *CurrentSect.ToString());
            return true;
        }
        UE_LOG(LogShanHe, Log, TEXT("争夺掌门失败，影响力不足"));
    }
    return false;
}

void USectSystem::DeclareWar(FName EnemySectID)
{
    if (FSect* S = SectDatabase.Find(CurrentSect))
    {
        S->EnemySects.AddUnique(EnemySectID);
        UE_LOG(LogShanHe, Log, TEXT("向 %s 宣战"), *EnemySectID.ToString());
    }
}

void USectSystem::ProposeAlliance(FName AlliedSectID)
{
    if (FSect* S = SectDatabase.Find(CurrentSect))
    {
        S->AlliedSects.AddUnique(AlliedSectID);
        UE_LOG(LogShanHe, Log, TEXT("与 %s 结盟"), *AlliedSectID.ToString());
    }
}

ESectRank USectSystem::GetPlayerRank(FName SectID) const
{
    if (const FSect* S = SectDatabase.Find(SectID))
        return S->PlayerRank;
    return ESectRank::Disciple;
}

TArray<FSectMission> USectSystem::GetAvailableMissions(FName SectID) const
{
    TArray<FSectMission> Missions;
    // 实际项目中根据门派和玩家等级返回任务列表
    return Missions;
}
