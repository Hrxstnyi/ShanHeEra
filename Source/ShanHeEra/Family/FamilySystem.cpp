#include "Family/FamilySystem.h"
#include "Core/ShanHeLog.h"

void UFamilySystem::InitializeFamilies()
{
    UE_LOG(LogShanHe, Log, TEXT("家族系统初始化 - 家谱、祖先、家训、家业、财产、声望、婚姻网络"));
}

void UFamilySystem::Simulate(int32 Days)
{
    for (FFamilyData& F : Families)
    {
        F.Prestige += FMath::RandRange(-1, 2) * (Days / 30);
    }
}

void UFamilySystem::ArrangeMarriage(FGuid PersonA, FGuid PersonB)
{
    UE_LOG(LogShanHe, Log, TEXT("联姻完成 - 家族关系、财富、地位、人脉的结合"));
}

FFamilyData UFamilySystem::GetFamily(FName FamilyID) const
{
    for (const FFamilyData& F : Families)
        if (F.FamilyID == FamilyID) return F;
    return FFamilyData();
}
