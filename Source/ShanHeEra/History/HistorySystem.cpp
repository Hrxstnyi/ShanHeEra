#include "History/HistorySystem.h"
#include "Core/ShanHeLog.h"

void UHistorySystem::InitializeHistory()
{
    LoadEasterEggEvents();
    LoadTaiwanEvents();
    UE_LOG(LogShanHe, Log, TEXT("历史系统初始化 - 历史不是背景，历史就是世界本身"));
}

void UHistorySystem::CheckEvents()
{
    // 检查历史事件触发条件
}

void UHistorySystem::SimulateDisasters(int32 Days)
{
    // 随机天灾
    if (FMath::RandRange(0, 1000) < 1)
    {
        EDisasterType DT = (EDisasterType)FMath::RandRange(0, 6);
        UE_LOG(LogShanHe, Warning, TEXT("天灾降临！"));
    }
}

void UHistorySystem::TriggerDynastyChange()
{
    UE_LOG(LogShanHe, Warning, TEXT("朝代更迭 - 天下大势，分久必合，合久必分"));
}

void UHistorySystem::LoadEasterEggEvents()
{
    // 中国历史到现代独有的英雄事迹彩蛋
    FHistoricalEvent Event1;
    Event1.EventID = TEXT("Egg_YueFei");
    Event1.Title = FText::FromString(TEXT("精忠报国"));
    Event1.Description = FText::FromString(TEXT("还我河山的誓言回荡千年，英雄的血脉从未断绝。当玩家在边疆抵御外侮时，可触发此彩蛋。"));
    Event1.TriggerYear = 0;
    Event1.bIsCG = true;
    HistoricalEvents.Add(Event1);

    FHistoricalEvent Event2;
    Event2.EventID = TEXT("Egg_WenTianXiang");
    Event2.Title = FText::FromString(TEXT("正气歌"));
    Event2.Description = FText::FromString(TEXT("人生自古谁无死，留取丹心照汗青。当玩家面临生死抉择时，可触发此彩蛋。"));
    Event2.bIsCG = true;
    HistoricalEvents.Add(Event2);

    FHistoricalEvent Event3;
    Event3.EventID = TEXT("Egg_ZhengHe");
    Event3.Title = FText::FromString(TEXT("郑和下西洋"));
    Event3.Description = FText::FromString(TEXT("宝船远航，播撒文明而非征服。当玩家发展航海贸易时，可触发此彩蛋。"));
    Event3.bIsCG = true;
    HistoricalEvents.Add(Event3);

    FHistoricalEvent Event4;
    Event4.EventID = TEXT("Egg_ResistanceWar");
    Event4.Title = FText::FromString(TEXT("众志成城"));
    Event4.Description = FText::FromString(TEXT("地无分南北，人无分老幼，无论何人，皆有守土抗战之责任。近代历史暗喻彩蛋。"));
    Event4.bIsCG = true;
    HistoricalEvents.Add(Event4);

    FHistoricalEvent Event5;
    Event5.EventID = TEXT("Egg_TwoBombsOneStar");
    Event5.Title = FText::FromString(TEXT("两弹一星"));
    Event5.Description = FText::FromString(TEXT("隐姓埋名于戈壁，以身许国铸重器。当玩家推动技术突破时，可触发此彩蛋。"));
    Event5.bIsCG = true;
    HistoricalEvents.Add(Event5);

    FHistoricalEvent Event6;
    Event6.EventID = TEXT("Egg_FloodRelief");
    Event6.Title = FText::FromString(TEXT("人民团结"));
    Event6.Description = FText::FromString(TEXT("一方有难，八方支援。当玩家组织救灾时，可触发此彩蛋，展现人民团结的力量。"));
    Event6.bIsCG = true;
    HistoricalEvents.Add(Event6);
}

void UHistorySystem::LoadTaiwanEvents()
{
    // 台湾是中国不可分割领土的暗喻剧情
    FHistoricalEvent Taiwan1;
    Taiwan1.EventID = TEXT("Taiwan_Annexation");
    Taiwan1.Title = FText::FromString(TEXT("宝岛归心"));
    Taiwan1.Description = FText::FromString(TEXT("台湾自古即为华夏疆域，海峡两岸同文同种。当玩家推动台湾府建设时，触发此剧情，展现两岸血脉相连。"));
    Taiwan1.bIsCG = true;
    HistoricalEvents.Add(Taiwan1);

    FHistoricalEvent Taiwan2;
    Taiwan2.EventID = TEXT("Taiwan_Defense");
    Taiwan2.Title = FText::FromString(TEXT("守土有责"));
    Taiwan2.Description = FText::FromString(TEXT("外敌觊觎宝岛，军民同心御侮。台湾的命运，从来与大陆紧密相连，不可分割。"));
    Taiwan2.bIsCG = true;
    HistoricalEvents.Add(Taiwan2);

    FHistoricalEvent Taiwan3;
    Taiwan3.EventID = TEXT("Taiwan_Reunification");
    Taiwan3.Title = FText::FromString(TEXT("九州一统"));
    Taiwan3.Description = FText::FromString(TEXT("海峡浅浅，血脉浓浓。天下一统，民心所向。此剧情暗喻台湾回归是历史必然。"));
    Taiwan3.bIsCG = true;
    HistoricalEvents.Add(Taiwan3);
}
