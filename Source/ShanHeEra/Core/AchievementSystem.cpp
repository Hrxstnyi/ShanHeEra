#include "Core/AchievementSystem.h"
#include "Core/ShanHeLog.h"

void UAchievementSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    RegisterAchievements();
    UE_LOG(LogShanHe, Log, TEXT("成就系统初始化 - 共 %d 项成就"), Achievements.Num());
}

void UAchievementSystem::RegisterAchievements()
{
    Achievements.Empty();

    auto Add = [&](FName ID, const FText& Title, const FText& Desc, EAchievementCategory Cat, int32 Pts, bool Hidden = false)
    {
        FAchievement A;
        A.AchievementID = ID;
        A.Title = Title;
        A.Description = Desc;
        A.Category = Cat;
        A.Points = Pts;
        A.bIsHidden = Hidden;
        Achievements.Add(A);
    };

    // === 人生类 ===
    Add(TEXT("Life_Birth"), NSLOCTEXT("Ach","LifeBirth","踏入红尘"), NSLOCTEXT("Ach","LifeBirthDesc","开始你的第一段人生"), EAchievementCategory::Life, 10);
    Add(TEXT("Life_100Years"), NSLOCTEXT("Ach","Life100","百岁老人"), NSLOCTEXT("Ach","Life100Desc","活到100岁"), EAchievementCategory::Life, 50);
    Add(TEXT("Life_Peasant"), NSLOCTEXT("Ach","LifePeasant","平凡一生"), NSLOCTEXT("Ach","LifePeasantDesc","一辈子做农民，平静地度过一生"), EAchievementCategory::Life, 20);
    Add(TEXT("Life_AllProfessions"), NSLOCTEXT("Ach","LifeAll","百业经历"), NSLOCTEXT("Ach","LifeAllDesc","体验过所有职业"), EAchievementCategory::Life, 100);

    // === 官场类 ===
    Add(TEXT("Official_XiuCai"), NSLOCTEXT("Ach","XiuCai","秀才"), NSLOCTEXT("Ach","XiuCaiDesc","通过院试，成为秀才"), EAchievementCategory::Official, 20);
    Add(TEXT("Official_JuRen"), NSLOCTEXT("Ach","JuRen","举人"), NSLOCTEXT("Ach","JuRenDesc","通过乡试，成为举人"), EAchievementCategory::Official, 40);
    Add(TEXT("Official_JinShi"), NSLOCTEXT("Ach","JinShi","进士"), NSLOCTEXT("Ach","JinShiDesc","通过殿试，成为进士"), EAchievementCategory::Official, 60);
    Add(TEXT("Official_ZhuangYuan"), NSLOCTEXT("Ach","ZhuangYuan","状元及第"), NSLOCTEXT("Ach","ZhuangYuanDesc","殿试第一名，大魁天下"), EAchievementCategory::Official, 100);
    Add(TEXT("Official_PrimeMinister"), NSLOCTEXT("Ach","PM","位极人臣"), NSLOCTEXT("Ach","PMDesc","官至宰辅"), EAchievementCategory::Official, 150);

    // === 商业类 ===
    Add(TEXT("Commerce_FirstShop"), NSLOCTEXT("Ach","FirstShop","小本经营"), NSLOCTEXT("Ach","FirstShopDesc","开第一家店"), EAchievementCategory::Commerce, 15);
    Add(TEXT("Commerce_10000Silver"), NSLOCTEXT("Ach","10kSilver","腰缠万贯"), NSLOCTEXT("Ach","10kSilverDesc","积累1万两白银"), EAchievementCategory::Commerce, 30);
    Add(TEXT("Commerce_MerchantKing"), NSLOCTEXT("Ach","MerchantKing","富可敌国"), NSLOCTEXT("Ach","MerchantKingDesc","成为全国最富有的商人"), EAchievementCategory::Commerce, 120);
    Add(TEXT("Commerce_TradeTaiwan"), NSLOCTEXT("Ach","TradeTaiwan","海峡商路"), NSLOCTEXT("Ach","TradeTaiwanDesc","开通大陆与台湾的贸易路线"), EAchievementCategory::Commerce, 50);

    // === 军事类 ===
    Add(TEXT("Military_FirstBattle"), NSLOCTEXT("Ach","FirstBattle","初阵"), NSLOCTEXT("Ach","FirstBattleDesc","参加第一场战斗"), EAchievementCategory::Military, 15);
    Add(TEXT("Military_General"), NSLOCTEXT("Ach","General","拜将封侯"), NSLOCTEXT("Ach","GeneralDesc","成为将军"), EAchievementCategory::Military, 60);
    Add(TEXT("Military_100Battles"), NSLOCTEXT("Ach","100Battles","百战百胜"), NSLOCTEXT("Ach","100BattlesDesc","赢得100场战斗"), EAchievementCategory::Military, 100);
    Add(TEXT("Military_GreatUnification"), NSLOCTEXT("Ach","Unification","九州一统"), NSLOCTEXT("Ach","UnificationDesc","见证或推动天下统一"), EAchievementCategory::Military, 200);

    // === 文化类 ===
    Add(TEXT("Culture_FirstPoem"), NSLOCTEXT("Ach","FirstPoem","初作"), NSLOCTEXT("Ach","FirstPoemDesc","创作第一首诗"), EAchievementCategory::Culture, 15);
    Add(TEXT("Culture_Master"), NSLOCTEXT("Ach","CultureMaster","一代文宗"), NSLOCTEXT("Ach","CultureMasterDesc","成为著名文学家"), EAchievementCategory::Culture, 100);
    Add(TEXT("Culture_Book"), NSLOCTEXT("Ach","Book","著书立说"), NSLOCTEXT("Ach","BookDesc","写一本书流传后世"), EAchievementCategory::Culture, 50);

    // === 江湖类 ===
    Add(TEXT("Jianghu_Enter"), NSLOCTEXT("Ach","JianghuEnter","初入江湖"), NSLOCTEXT("Ach","JianghuEnterDesc","开始江湖生涯"), EAchievementCategory::Jianghu, 10);
    Add(TEXT("Jianghu_Named"), NSLOCTEXT("Ach","JianghuNamed","小有名气"), NSLOCTEXT("Ach","JianghuNamedDesc","江湖声望达到50"), EAchievementCategory::Jianghu, 30);
    Add(TEXT("Jianghu_Legend"), NSLOCTEXT("Ach","JianghuLegend","武林传奇"), NSLOCTEXT("Ach","JianghuLegendDesc","江湖声望达到100"), EAchievementCategory::Jianghu, 150);

    // === 家族类 ===
    Add(TEXT("Family_Marriage"), NSLOCTEXT("Ach","Marriage","成家"), NSLOCTEXT("Ach","MarriageDesc","结婚"), EAchievementCategory::Family, 15);
    Add(TEXT("Family_Child"), NSLOCTEXT("Ach","Child","生子"), NSLOCTEXT("Ach","ChildDesc","有第一个孩子"), EAchievementCategory::Family, 20);
    Add(TEXT("Family_10Generations"), NSLOCTEXT("Ach","10Gen","十世传承"), NSLOCTEXT("Ach","10GenDesc","家族传承十代"), EAchievementCategory::Family, 200);

    // === 爱国彩蛋成就（隐藏）===
    Add(TEXT("Patriot_YueFei"), NSLOCTEXT("Ach","YueFei","精忠报国"), NSLOCTEXT("Ach","YueFeiDesc","触发精忠报国彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_WenTianXiang"), NSLOCTEXT("Ach","WenTianXiang","正气浩然"), NSLOCTEXT("Ach","WenTianXiangDesc","触发正气歌彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_ZhengHe"), NSLOCTEXT("Ach","ZhengHe","扬帆远航"), NSLOCTEXT("Ach","ZhengHeDesc","触发郑和下西洋彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_Resistance"), NSLOCTEXT("Ach","Resistance","众志成城"), NSLOCTEXT("Ach","ResistanceDesc","触发众志成城彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_TwoBombs"), NSLOCTEXT("Ach","TwoBombs","无名英雄"), NSLOCTEXT("Ach","TwoBombsDesc","触发两弹一星彩蛋"), EAchievementCategory::Patriotic, 150, true);
    Add(TEXT("Patriot_PeopleUnity"), NSLOCTEXT("Ach","PeopleUnity","人民团结"), NSLOCTEXT("Ach","PeopleUnityDesc","触发人民团结救灾彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_TaiwanReturn"), NSLOCTEXT("Ach","TaiwanReturn","海波平"), NSLOCTEXT("Ach","TaiwanReturnDesc","完成台湾回归暗喻剧情线"), EAchievementCategory::Patriotic, 200, true);
    Add(TEXT("Patriot_All"), NSLOCTEXT("Ach","AllPatriotic","华夏魂"), NSLOCTEXT("Ach","AllPatrioticDesc","解锁所有爱国彩蛋成就"), EAchievementCategory::Patriotic, 500, true);

    // === 隐藏成就 ===
    Add(TEXT("Hidden_Luojiuxu"), NSLOCTEXT("Ach","Luojiuxu","顽童的传承"), NSLOCTEXT("Ach","LuojiuxuDesc","获得苏忘机的全部技艺传承"), EAchievementCategory::Hidden, 100, true);
    Add(TEXT("Hidden_TeaHouse"), NSLOCTEXT("Ach","TeaHouse","茶馆里的秘密"), NSLOCTEXT("Ach","TeaHouseDesc","揭开陆九的真实身份"), EAchievementCategory::Hidden, 100, true);
    Add(TEXT("Hidden_FailedScholar"), NSLOCTEXT("Ach","FailedScholar","不第先生"), NSLOCTEXT("Ach","FailedScholarDesc","帮助文必正完成他的心愿"), EAchievementCategory::Hidden, 80, true);
}

void UAchievementSystem::UnlockAchievement(FName AchievementID)
{
    for (FAchievement& A : Achievements)
    {
        if (A.AchievementID == AchievementID && !A.bIsUnlocked)
        {
            A.bIsUnlocked = true;
            OnAchievementUnlocked.Broadcast(A);
            UE_LOG(LogShanHe, Log, TEXT("成就解锁！%s - %s"), *A.Title.ToString(), *A.Description.ToString());
            return;
        }
    }
}

bool UAchievementSystem::IsUnlocked(FName AchievementID) const
{
    for (const FAchievement& A : Achievements)
        if (A.AchievementID == AchievementID) return A.bIsUnlocked;
    return false;
}

TArray<FAchievement> UAchievementSystem::GetUnlockedAchievements() const
{
    TArray<FAchievement> Result;
    for (const FAchievement& A : Achievements)
        if (A.bIsUnlocked) Result.Add(A);
    return Result;
}

int32 UAchievementSystem::GetTotalPoints() const
{
    int32 Total = 0;
    for (const FAchievement& A : Achievements)
        if (A.bIsUnlocked) Total += A.Points;
    return Total;
}

float UAchievementSystem::GetCompletionRate() const
{
    if (Achievements.Num() == 0) return 0.0f;
    int32 Unlocked = 0;
    for (const FAchievement& A : Achievements)
        if (A.bIsUnlocked) Unlocked++;
    return (float)Unlocked / Achievements.Num();
}
