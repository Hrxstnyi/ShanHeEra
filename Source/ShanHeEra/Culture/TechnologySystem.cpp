#include "Culture/TechnologySystem.h"
#include "Core/ShanHeLog.h"

void UTechnologySystem::InitializeTechTree()
{
    TechTree.Empty();

    struct FTechDef { FName ID; const TCHAR* Name; ETechCategory Cat; int32 Tier; int32 Cost; bool Breakthrough; };
    FTechDef Defs[] = {
        // 农业
        {TEXT("Irrigation"), TEXT("水利灌溉"), ETechCategory::Agriculture, 1, 100, false},
        {TEXT("CropRotation"), TEXT("轮作制度"), ETechCategory::Agriculture, 2, 200, false},
        {TEXT("ImprovedPlow"), TEXT("曲辕犁"), ETechCategory::Agriculture, 2, 200, false},
        {TEXT("HighYieldCrops"), TEXT("高产作物"), ETechCategory::Agriculture, 4, 500, false},
        // 冶金
        {TEXT("BlastFurnace"), TEXT("高炉冶铁"), ETechCategory::Metallurgy, 2, 250, false},
        {TEXT("SteelMaking"), TEXT("炼钢术"), ETechCategory::Metallurgy, 3, 400, false},
        // 造船
        {TEXT("JunkShip"), TEXT("福船"), ETechCategory::Shipbuilding, 2, 300, false},
        {TEXT("TreasureShip"), TEXT("宝船"), ETechCategory::Shipbuilding, 4, 800, true}, // 郑和彩蛋
        // 建筑
        {TEXT("ArchBridge"), TEXT("拱桥技术"), ETechCategory::Construction, 2, 200, false},
        {TEXT("GreatWall"), TEXT("城防体系"), ETechCategory::Construction, 3, 500, false},
        // 医学
        {TEXT("HerbalMedicine"), TEXT("本草学"), ETechCategory::Medicine, 1, 100, false},
        {TEXT("Acupuncture"), TEXT("针灸"), ETechCategory::Medicine, 2, 250, false},
        {TEXT("Vaccination"), TEXT("人痘接种"), ETechCategory::Medicine, 4, 600, true}, // 医学突破
        // 印刷
        {TEXT("WoodblockPrint"), TEXT("雕版印刷"), ETechCategory::Printing, 1, 150, false},
        {TEXT("MovableType"), TEXT("活字印刷"), ETechCategory::Printing, 3, 400, true}, // 毕昇彩蛋
        // 兵器
        {TEXT("Crossbow"), TEXT("强弩"), ETechCategory::Weaponry, 2, 250, false},
        {TEXT("Gunpowder"), TEXT("火药"), ETechCategory::Weaponry, 3, 500, true}, // 四大发明
        {TEXT("Cannon"), TEXT("火炮"), ETechCategory::Weaponry, 4, 700, false},
        // 纺织
        {TEXT("Loom"), TEXT("织机"), ETechCategory::Textile, 1, 100, false},
        {TEXT("SilkReeling"), TEXT("缫丝术"), ETechCategory::Textile, 2, 200, false},
        // 航海
        {TEXT("Compass"), TEXT("指南针"), ETechCategory::Navigation, 2, 300, true}, // 四大发明
        {TEXT("Astrolabe"), TEXT("星盘航海"), ETechCategory::Navigation, 3, 500, false},
    };

    for (const auto& D : Defs)
    {
        FTechnology T;
        T.TechID = D.ID;
        T.TechName = FText::FromString(D.Name);
        T.Category = D.Cat;
        T.Tier = D.Tier;
        T.ResearchCost = D.Cost;
        T.bIsBreakthrough = D.Breakthrough;
        TechTree.Add(T);
    }

    UE_LOG(LogShanHe, Log, TEXT("技术系统初始化 - %d 项技术，9大类"), TechTree.Num());
}

void UTechnologySystem::ResearchTech(FName TechID, int32 ResearchPoints)
{
    for (FTechnology& T : TechTree)
    {
        if (T.TechID == TechID && !T.bIsResearched && CanResearch(TechID))
        {
            T.CurrentProgress += ResearchPoints;
            if (T.CurrentProgress >= T.ResearchCost)
            {
                T.bIsResearched = true;
                OnTechResearched(T);
            }
            return;
        }
    }
}

bool UTechnologySystem::CanResearch(FName TechID) const
{
    for (const FTechnology& T : TechTree)
    {
        if (T.TechID == TechID)
        {
            if (T.bIsResearched) return false;
            // 检查前置
            for (FName Pre : T.Prerequisites)
            {
                bool bFound = false;
                for (const FTechnology& P : TechTree)
                    if (P.TechID == Pre && P.bIsResearched) { bFound = true; break; }
                if (!bFound) return false;
            }
            return true;
        }
    }
    return false;
}

TArray<FTechnology> UTechnologySystem::GetAvailableTechs() const
{
    TArray<FTechnology> Result;
    for (const FTechnology& T : TechTree)
        if (!T.bIsResearched && CanResearch(T.TechID)) Result.Add(T);
    return Result;
}

TArray<FTechnology> UTechnologySystem::GetTechsByCategory(ETechCategory Category) const
{
    TArray<FTechnology> Result;
    for (const FTechnology& T : TechTree)
        if (T.Category == Category) Result.Add(T);
    return Result;
}

float UTechnologySystem::GetTechProgress(ETechCategory Category) const
{
    int32 Total = 0, Researched = 0;
    for (const FTechnology& T : TechTree)
    {
        if (T.Category == Category)
        {
            Total++;
            if (T.bIsResearched) Researched++;
        }
    }
    return Total > 0 ? (float)Researched / Total : 0.0f;
}

void UTechnologySystem::OnTechResearched(FTechnology& Tech)
{
    if (Tech.bIsBreakthrough)
    {
        TotalBreakthroughs++;
        UE_LOG(LogShanHe, Warning, TEXT("技术突破！%s - 触发历史彩蛋"), *Tech.TechName.ToString());
    }
    else
    {
        UE_LOG(LogShanHe, Log, TEXT("研发完成：%s"), *Tech.TechName.ToString());
    }
}
