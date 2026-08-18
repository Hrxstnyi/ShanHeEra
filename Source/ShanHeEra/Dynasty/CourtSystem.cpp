#include "Dynasty/CourtSystem.h"
#include "Core/ShanHeLog.h"

void UCourtSystem::InitializeCourt()
{
    // 初始化皇帝
    Emperor.Name.Surname = TEXT("朱");
    Emperor.Name.GivenName = TEXT("元璋");
    Emperor.Age = 40;
    Emperor.Stats.Intelligence = 88;
    Emperor.Stats.Charisma = 85;
    Emperor.Stats.Governance = 90;
    Emperor.Stats.Martial = 80;
    Emperor.Traits = {ECharacterTrait::Brave, ECharacterTrait::Wise, ECharacterTrait::Ambitious};

    // 初始化派系
    FFactionData Reformers;
    Reformers.FactionID = TEXT("Reformers");
    Reformers.FactionName = FText::FromString(TEXT("改革派"));
    Reformers.Ideology = FText::FromString(TEXT("推行新法，富国强兵"));
    Reformers.Influence = 30;
    Factions.Add(Reformers);

    FFactionData Conservatives;
    Conservatives.FactionID = TEXT("Conservatives");
    Conservatives.FactionName = FText::FromString(TEXT("保守派"));
    Conservatives.Ideology = FText::FromString(TEXT("恪守祖制，稳定为先"));
    Conservatives.Influence = 35;
    Factions.Add(Conservatives);

    UE_LOG(LogShanHe, Log, TEXT("朝廷系统初始化 - 皇权、官僚、士绅、商业、军事五大力量博弈"));
}

void UCourtSystem::Simulate(int32 Days)
{
    SimulatePolitics(Days);
    SimulateFactions(Days);
    SimulateOfficialPerformance(Days);
    CheckSuccession();
}

void UCourtSystem::SimulatePolitics(int32 Days)
{
    Emperor.ReignYears += Days / 365;
    Emperor.Age += Days / 365;
    // 皇权波动
    ImperialAuthority += FMath::RandRange(-0.5f, 0.5f) * Days;
    ImperialAuthority = FMath::Clamp(ImperialAuthority, 0.0f, 100.0f);
}

void UCourtSystem::SimulateFactions(int32 Days)
{
    for (FFactionData& F : Factions)
        F.Influence += FMath::RandRange(-1, 1) * Days;
}

void UCourtSystem::SimulateOfficialPerformance(int32 Days)
{
    for (FOfficialData& O : Officials)
    {
        if (O.Traits.Contains(ECharacterTrait::Corrupt))
            O.Corruption += FMath::RandRange(0, 2) * Days;
        if (O.Traits.Contains(ECharacterTrait::Honest))
            O.Corruption = FMath::Max(0, O.Corruption - Days);
    }
}

void UCourtSystem::CheckSuccession()
{
    if (Emperor.Age > 70 && FMath::RandRange(0, 100) < 5)
    {
        UE_LOG(LogShanHe, Warning, TEXT("皇帝驾崩，皇位继承危机！"));
    }
}

void UCourtSystem::ConductImperialExamination()
{
    UE_LOG(LogShanHe, Log, TEXT("科举开考 - 寒门子弟的上升之路"));
}

void UCourtSystem::PromoteOfficial(FGuid OfficialID, int32 NewRank) {}
void UCourtSystem::StartReform(FText ReformName)
{
    UE_LOG(LogShanHe, Log, TEXT("改革启动：%s - 必将触动既得利益"), *ReformName.ToString());
}
