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

    auto Add = [&](FName ID, const TCHAR* Title, const TCHAR* Desc, EAchievementCategory Cat, int32 Pts, bool Hidden = false)
    {
        FAchievement A;
        A.AchievementID = ID;
        A.Title = FText::FromString(Title);
        A.Description = FText::FromString(Desc);
        A.Category = Cat;
        A.Points = Pts;
        A.bIsHidden = Hidden;
        Achievements.Add(A);
    };

    // === 人生类 ===
    Add(TEXT("Life_Birth"), TEXT("踏入红尘"), TEXT("开始你的第一段人生"), EAchievementCategory::Life, 10);
    Add(TEXT("Life_100Years"), TEXT("百岁老人"), TEXT("活到100岁"), EAchievementCategory::Life, 50);
    Add(TEXT("Life_Peasant"), TEXT("平凡一生"), TEXT("一辈子做农民，平静地度过一生"), EAchievementCategory::Life, 20);
    Add(TEXT("Life_AllProfessions"), TEXT("百业经历"), TEXT("体验过所有职业"), EAchievementCategory::Life, 100);

    // === 官场类 ===
    Add(TEXT("Official_XiuCai"), TEXT("秀才"), TEXT("通过院试，成为秀才"), EAchievementCategory::Official, 20);
    Add(TEXT("Official_JuRen"), TEXT("举人"), TEXT("通过乡试，成为举人"), EAchievementCategory::Official, 40);
    Add(TEXT("Official_JinShi"), TEXT("进士"), TEXT("通过殿试，成为进士"), EAchievementCategory::Official, 60);
    Add(TEXT("Official_ZhuangYuan"), TEXT("状元及第"), TEXT("殿试第一名，大魁天下"), EAchievementCategory::Official, 100);
    Add(TEXT("Official_PrimeMinister"), TEXT("位极人臣"), TEXT("官至宰辅"), EAchievementCategory::Official, 150);

    // === 商业类 ===
    Add(TEXT("Commerce_FirstShop"), TEXT("小本经营"), TEXT("开第一家店"), EAchievementCategory::Commerce, 15);
    Add(TEXT("Commerce_10000Silver"), TEXT("腰缠万贯"), TEXT("积累1万两白银"), EAchievementCategory::Commerce, 30);
    Add(TEXT("Commerce_MerchantKing"), TEXT("富可敌国"), TEXT("成为全国最富有的商人"), EAchievementCategory::Commerce, 120);
    Add(TEXT("Commerce_TradeTaiwan"), TEXT("海峡商路"), TEXT("开通大陆与台湾的贸易路线"), EAchievementCategory::Commerce, 50);

    // === 军事类 ===
    Add(TEXT("Military_FirstBattle"), TEXT("初阵"), TEXT("参加第一场战斗"), EAchievementCategory::Military, 15);
    Add(TEXT("Military_General"), TEXT("拜将封侯"), TEXT("成为将军"), EAchievementCategory::Military, 60);
    Add(TEXT("Military_100Battles"), TEXT("百战百胜"), TEXT("赢得100场战斗"), EAchievementCategory::Military, 100);
    Add(TEXT("Military_GreatUnification"), TEXT("九州一统"), TEXT("见证或推动天下统一"), EAchievementCategory::Military, 200);

    // === 文化类 ===
    Add(TEXT("Culture_FirstPoem"), TEXT("初作"), TEXT("创作第一首诗"), EAchievementCategory::Culture, 15);
    Add(TEXT("Culture_Master"), TEXT("一代文宗"), TEXT("成为著名文学家"), EAchievementCategory::Culture, 100);
    Add(TEXT("Culture_Book"), TEXT("著书立说"), TEXT("写一本书流传后世"), EAchievementCategory::Culture, 50);

    // === 江湖类 ===
    Add(TEXT("Jianghu_Enter"), TEXT("初入江湖"), TEXT("开始江湖生涯"), EAchievementCategory::Jianghu, 10);
    Add(TEXT("Jianghu_Named"), TEXT("小有名气"), TEXT("江湖声望达到50"), EAchievementCategory::Jianghu, 30);
    Add(TEXT("Jianghu_Legend"), TEXT("武林传奇"), TEXT("江湖声望达到100"), EAchievementCategory::Jianghu, 150);

    // === 家族类 ===
    Add(TEXT("Family_Marriage"), TEXT("成家"), TEXT("结婚"), EAchievementCategory::Family, 15);
    Add(TEXT("Family_Child"), TEXT("生子"), TEXT("有第一个孩子"), EAchievementCategory::Family, 20);
    Add(TEXT("Family_10Generations"), TEXT("十世传承"), TEXT("家族传承十代"), EAchievementCategory::Family, 200);

    // === 爱国彩蛋成就（隐藏）===
    Add(TEXT("Patriot_YueFei"), TEXT("精忠报国"), TEXT("触发精忠报国彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_WenTianXiang"), TEXT("正气浩然"), TEXT("触发正气歌彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_ZhengHe"), TEXT("扬帆远航"), TEXT("触发郑和下西洋彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_Resistance"), TEXT("众志成城"), TEXT("触发众志成城彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_TwoBombs"), TEXT("无名英雄"), TEXT("触发两弹一星彩蛋"), EAchievementCategory::Patriotic, 150, true);
    Add(TEXT("Patriot_PeopleUnity"), TEXT("人民团结"), TEXT("触发人民团结救灾彩蛋"), EAchievementCategory::Patriotic, 100, true);
    Add(TEXT("Patriot_TaiwanReturn"), TEXT("海波平"), TEXT("完成台湾回归暗喻剧情线"), EAchievementCategory::Patriotic, 200, true);
    Add(TEXT("Patriot_All"), TEXT("华夏魂"), TEXT("解锁所有爱国彩蛋成就"), EAchievementCategory::Patriotic, 500, true);

    // === 隐藏成就 ===
    Add(TEXT("Hidden_Luojiuxu"), TEXT("顽童的传承"), TEXT("获得苏忘机的全部技艺传承"), EAchievementCategory::Hidden, 100, true);
    Add(TEXT("Hidden_TeaHouse"), TEXT("茶馆里的秘密"), TEXT("揭开陆九的真实身份"), EAchievementCategory::Hidden, 100, true);
    Add(TEXT("Hidden_FailedScholar"), TEXT("不第先生"), TEXT("帮助文必正完成他的心愿"), EAchievementCategory::Hidden, 80, true);
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
