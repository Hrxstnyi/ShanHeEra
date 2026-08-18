#include "History/DisasterSystem.h"
#include "Core/ShanHeLog.h"

void UDisasterSystem::InitializeDisasters()
{
    UE_LOG(LogShanHe, Log, TEXT("天灾疫病系统初始化 - 洪水/干旱/蝗灾/地震/疫病/寒灾/饥荒"));
}

void UDisasterSystem::Simulate(int32 Days, int32 CurrentYear, int32 CurrentMonth)
{
    // 每年检查一次随机天灾
    if (CurrentMonth == 1 && FMath::RandRange(0.0f, 1.0f) < DisasterChancePerYear)
    {
        CheckRandomDisaster(CurrentYear, CurrentMonth);
    }

    // 更新活跃灾害
    UpdateDisasterEffects(Days);

    // 疫病传播
    if (ActivePlague.Infected > 0 && !ActivePlague.bIsContained)
    {
        SimulatePlague(Days);
    }
}

void UDisasterSystem::CheckRandomDisaster(int32 Year, int32 Month)
{
    // 根据月份选择可能的灾害类型
    EDisasterType PossibleTypes[] = {
        EDisasterType::Flood,    // 夏季洪水
        EDisasterType::Drought,  // 干旱
        EDisasterType::Locust,   // 蝗灾
        EDisasterType::Earthquake, // 地震(全年)
        EDisasterType::Plague,   // 疫病
        EDisasterType::Cold,     // 寒灾(冬季)
        EDisasterType::Famine    // 饥荒(歉收后)
    };

    int32 TypeIndex = FMath::RandRange(0, 6);
    EDisasterType Type = PossibleTypes[TypeIndex];
    int32 Severity = FMath::RandRange(1, 5);

    // 随机选择区域
    FName Regions[] = {TEXT("NorthPlain"), TEXT("Jiangnan"), TEXT("Northwest"), TEXT("Southwest"), TEXT("Coast"), TEXT("Inland")};
    FName Region = Regions[FMath::RandRange(0, 5)];

    TriggerDisaster(Type, Region, Severity);
}

void UDisasterSystem::TriggerDisaster(EDisasterType Type, FName RegionID, int32 Severity)
{
    FDisasterEvent Disaster;
    Disaster.DisasterID = FName(*FString::Printf(TEXT("Disaster_%d"), FDateTime::Now().GetTicks()));
    Disaster.Type = Type;
    Disaster.RegionID = RegionID;
    Disaster.Severity = Severity;
    Disaster.bIsActive = true;

    // 根据类型设置参数
    switch (Type)
    {
        case EDisasterType::Flood:
            Disaster.Title = FText::FromString(TEXT("洪水泛滥"));
            Disaster.Description = FText::FromString(TEXT("连月暴雨，江河决堤，良田被淹。"));
            Disaster.DurationDays = 30 + Severity * 15;
            Disaster.GrainPriceMultiplier = 1.2f + Severity * 0.2f;
            Disaster.StabilityImpact = -5.0f * Severity;
            Disaster.bHasCG = Severity >= 4;
            break;
        case EDisasterType::Drought:
            Disaster.Title = FText::FromString(TEXT("大旱连年"));
            Disaster.Description = FText::FromString(TEXT("滴雨未降，赤地千里，禾苗枯槁。"));
            Disaster.DurationDays = 90 + Severity * 30;
            Disaster.GrainPriceMultiplier = 1.5f + Severity * 0.3f;
            Disaster.StabilityImpact = -8.0f * Severity;
            Disaster.bHasCG = Severity >= 4;
            break;
        case EDisasterType::Locust:
            Disaster.Title = FText::FromString(TEXT("蝗灾遮天"));
            Disaster.Description = FText::FromString(TEXT("蝗虫遮天蔽日，所过之处寸草不生。"));
            Disaster.DurationDays = 15 + Severity * 5;
            Disaster.GrainPriceMultiplier = 2.0f + Severity * 0.5f;
            Disaster.StabilityImpact = -10.0f * Severity;
            Disaster.bHasCG = true;
            break;
        case EDisasterType::Earthquake:
            Disaster.Title = FText::FromString(TEXT("大地震"));
            Disaster.Description = FText::FromString(TEXT("山崩地裂，屋舍倒塌，生灵涂炭。"));
            Disaster.DurationDays = 7 + Severity * 3;
            Disaster.GrainPriceMultiplier = 1.3f;
            Disaster.StabilityImpact = -15.0f * Severity;
            Disaster.Casualties = 1000 * Severity;
            Disaster.bHasCG = Severity >= 3;
            break;
        case EDisasterType::Plague:
            Disaster.Title = FText::FromString(TEXT("瘟疫流行"));
            Disaster.Description = FText::FromString(TEXT("疫病蔓延，人心惶惶，十室九空。"));
            Disaster.DurationDays = 60 + Severity * 20;
            Disaster.GrainPriceMultiplier = 1.1f;
            Disaster.StabilityImpact = -12.0f * Severity;
            Disaster.bHasCG = Severity >= 3;
            // 触发疫病传播
            ActivePlague = FPlagueData();
            ActivePlague.PlagueID = Disaster.DisasterID;
            ActivePlague.OriginCity = RegionID;
            ActivePlague.InfectionRate = 0.05f + Severity * 0.02f;
            ActivePlague.MortalityRate = 0.03f + Severity * 0.02f;
            ActivePlague.Infected = 100 * Severity;
            break;
        case EDisasterType::Cold:
            Disaster.Title = FText::FromString(TEXT("大寒奇寒"));
            Disaster.Description = FText::FromString(TEXT("暴雪封路，滴水成冰，人畜冻死。"));
            Disaster.DurationDays = 30 + Severity * 10;
            Disaster.GrainPriceMultiplier = 1.3f;
            Disaster.StabilityImpact = -3.0f * Severity;
            break;
        case EDisasterType::Famine:
            Disaster.Title = FText::FromString(TEXT("大饥荒"));
            Disaster.Description = FText::FromString(TEXT("粮食绝收，饿殍遍野，民不聊生。"));
            Disaster.DurationDays = 180 + Severity * 30;
            Disaster.GrainPriceMultiplier = 3.0f + Severity * 0.5f;
            Disaster.StabilityImpact = -20.0f * Severity;
            Disaster.bHasCG = true;
            break;
    }

    DisasterHistory.Add(Disaster);
    OnDisasterTriggered.Broadcast(Disaster);

    UE_LOG(LogShanHe, Warning, TEXT("天灾降临！%s，区域: %s，严重度: %d"),
        *Disaster.Title.ToString(), *RegionID.ToString(), Severity);
}

void UDisasterSystem::StartRelief(FName DisasterID, int32 Grain, int32 Silver, int32 Medicine, bool bPlayerLed)
{
    FReliefEffort Relief;
    Relief.DisasterID = DisasterID;
    Relief.GrainDistributed = Grain;
    Relief.SilverSpent = Silver;
    Relief.MedicineSent = Medicine;
    Relief.bIsPlayerLed = bPlayerLed;
    Relief.ReliefEffectiveness = FMath::Min(1.0f, (Grain / 1000.0f + Silver / 5000.0f + Medicine / 100.0f) / 3.0f);
    ReliefEfforts.Add(DisasterID, Relief);

    UE_LOG(LogShanHe, Log, TEXT("救灾开始！粮食: %d，银两: %d，药材: %d，效率: %.0f%%"),
        Grain, Silver, Medicine, Relief.ReliefEffectiveness * 100);
}

TArray<FDisasterEvent> UDisasterSystem::GetActiveDisasters() const
{
    TArray<FDisasterEvent> Active;
    for (const FDisasterEvent& D : DisasterHistory)
        if (D.bIsActive) Active.Add(D);
    return Active;
}

void UDisasterSystem::SimulatePlague(int32 Days)
{
    // 疫病传播模型
    for (int32 i = 0; i < Days; i++)
    {
        ActivePlague.DaysSinceOutbreak++;
        // 新感染
        int32 NewInfections = (int32)(ActivePlague.Infected * ActivePlague.InfectionRate);
        ActivePlague.Infected += NewInfections;
        // 死亡
        int32 NewDeaths = (int32)(ActivePlague.Infected * ActivePlague.MortalityRate / 10);
        ActivePlague.Deaths += NewDeaths;
        ActivePlague.Infected = FMath::Max(0, ActivePlague.Infected - NewDeaths);
        // 恢复
        int32 NewRecovered = (int32)(ActivePlague.Infected * 0.02f);
        ActivePlague.Recovered += NewRecovered;
        ActivePlague.Infected = FMath::Max(0, ActivePlague.Infected - NewRecovered);
    }

    // 救灾有效则疫情受控
    if (const FReliefEffort* Relief = ReliefEfforts.Find(ActivePlague.PlagueID))
    {
        if (Relief->ReliefEffectiveness > 0.5f && ActivePlague.DaysSinceOutbreak > 30)
        {
            ActivePlague.bIsContained = true;
            UE_LOG(LogShanHe, Log, TEXT("疫情得到控制！累计死亡: %d"), ActivePlague.Deaths);
        }
    }
}

void UDisasterSystem::UpdateDisasterEffects(int32 Days)
{
    for (FDisasterEvent& D : DisasterHistory)
    {
        if (!D.bIsActive) continue;
        D.DurationDays -= Days;
        if (D.DurationDays <= 0)
        {
            EndDisaster(D.DisasterID);
        }
    }
}

void UDisasterSystem::EndDisaster(FName DisasterID)
{
    for (FDisasterEvent& D : DisasterHistory)
    {
        if (D.DisasterID == DisasterID)
        {
            D.bIsActive = false;
            UE_LOG(LogShanHe, Log, TEXT("灾害结束: %s"), *D.Title.ToString());
            break;
        }
    }
}

float UDisasterSystem::GetDisasterProbability(EDisasterType Type, int32 Month) const
{
    // 不同季节不同灾害概率不同
    switch (Type)
    {
        case EDisasterType::Flood: return (Month >= 6 && Month <= 8) ? 0.15f : 0.02f;
        case EDisasterType::Drought: return (Month >= 3 && Month <= 5) ? 0.12f : 0.03f;
        case EDisasterType::Locust: return (Month >= 6 && Month <= 9) ? 0.08f : 0.01f;
        case EDisasterType::Cold: return (Month >= 11 || Month <= 2) ? 0.10f : 0.01f;
        default: return 0.05f;
    }
}
