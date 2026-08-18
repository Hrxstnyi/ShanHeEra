#include "Core/ScenarioSystem.h"
#include "Core/ShanHeLog.h"

void UScenarioSystem::InitializeScenarios()
{
    // 三国群雄
    FScenarioData ThreeK;
    ThreeK.ID = EScenarioID::ThreeKingdoms;
    ThreeK.ScenarioName = FText::FromString(TEXT("三国群雄"));
    ThreeK.Description = FText::FromString(TEXT("东汉末年，天下大乱，群雄并起。你将在这个英雄辈出的时代中寻找自己的位置。"));
    ThreeK.StartYear = 184; ThreeK.StartMonth = 1;
    ThreeK.BackgroundStory = FText::FromString(TEXT("黄巾起义爆发，汉室衰微。各路诸侯拥兵自重，天下三分的大势正在形成。"));
    ThreeK.DifficultyModifier = 1.2f;
    ThreeK.SpecialEvents = {TEXT("YellowTurban"), TEXT("BattleOfRedCliffs"), TEXT("ThreeKingdomsForm")};
    Scenarios.Add(EScenarioID::ThreeKingdoms, ThreeK);

    // 盛唐气象
    FScenarioData Tang;
    Tang.ID = EScenarioID::GoldenTang;
    Tang.ScenarioName = FText::FromString(TEXT("盛唐气象"));
    Tang.Description = FText::FromString(TEXT("贞观之治，万国来朝。这是中华文明最辉煌的时代，你将见证大唐的盛世荣光。"));
    Tang.StartYear = 627; Tang.StartMonth = 1;
    Tang.BackgroundStory = FText::FromString(TEXT("李世民登基，改元贞观。大唐国力蒸蒸日上，丝绸之路畅通无阻，文化艺术空前繁荣。"));
    Tang.DifficultyModifier = 0.8f;
    Tang.SpecialEvents = {TEXT("ZhenguanZhiZhi"), TEXT("SilkRoadPeak"), TEXT("AnLushanRebellion")};
    Scenarios.Add(EScenarioID::GoldenTang, Tang);

    // 弱宋偏安
    FScenarioData Song;
    Song.ID = EScenarioID::WeakSong;
    Song.ScenarioName = FText::FromString(TEXT("弱宋偏安"));
    Song.Description = FText::FromString(TEXT("靖康之耻后，宋室南渡。外有强敌环伺，内有党争不断，你能否为大宋找到一条出路？"));
    Song.StartYear = 1127; Song.StartMonth = 1;
    Song.BackgroundStory = FText::FromString(TEXT("金兵南下，徽钦二帝被掳。赵构在临安即位，建立南宋。岳飞等名将正在为收复中原而战。"));
    Song.DifficultyModifier = 1.5f;
    Song.SpecialEvents = {TEXT("YueFeiNorthernExpedition"), TEXT("PeaceWithJin"), TEXT("MongolInvasion")};
    Scenarios.Add(EScenarioID::WeakSong, Song);

    // 明末风云
    FScenarioData Ming;
    Ming.ID = EScenarioID::LateMing;
    Ming.ScenarioName = FText::FromString(TEXT("明末风云"));
    Ming.Description = FText::FromString(TEXT("内有流寇作乱，外有后金虎视。崇祯皇帝有心无力，大明王朝走到了命运的十字路口。"));
    Ming.StartYear = 1628; Ming.StartMonth = 1;
    Ming.BackgroundStory = FText::FromString(TEXT("崇祯登基，铲除魏忠贤。但天下已经千疮百孔——陕西大旱，李自成起义，努尔哈赤的后金在辽东崛起。"));
    Ming.DifficultyModifier = 1.8f;
    Ming.SpecialEvents = {TEXT("LiZichengRebellion"), TEXT("QingInvasion"), TEXT("FallOfBeijing")};
    Scenarios.Add(EScenarioID::LateMing, Ming);

    UE_LOG(LogShanHe, Log, TEXT("历史剧本系统初始化 - %d 个剧本"), Scenarios.Num());
}

FScenarioData UScenarioSystem::GetScenario(EScenarioID ID) const
{
    if (const FScenarioData* S = Scenarios.Find(ID))
        return *S;
    return FScenarioData();
}

TArray<FScenarioData> UScenarioSystem::GetAllScenarios() const
{
    TArray<FScenarioData> Result;
    Scenarios.GenerateValueArray(Result);
    return Result;
}

void UScenarioSystem::SelectScenario(EScenarioID ID)
{
    CurrentScenario = ID;
    UE_LOG(LogShanHe, Log, TEXT("选择剧本：%d"), (int32)ID);
}

void UScenarioSystem::ApplyScenarioSettings()
{
    if (const FScenarioData* S = Scenarios.Find(CurrentScenario))
    {
        UE_LOG(LogShanHe, Log, TEXT("应用剧本设置：%s，起始年份：%d，难度：%.1f"),
            *S->ScenarioName.ToString(), S->StartYear, S->DifficultyModifier);
    }
}
