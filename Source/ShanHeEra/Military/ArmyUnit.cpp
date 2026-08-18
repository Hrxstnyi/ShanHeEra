#include "Military/ArmyUnit.h"
#include "Core/ShanHeLog.h"

float UArmyUnit::CalculatePower() const
{
    if (Data.Count <= 0 || Data.bIsRouting) return 0.0f;
    // 综合战力 = 兵力 * (攻击+防御) * 士气系数 * 训练系数
    float MoraleFactor = Data.Morale / 80.0f;
    float TrainingFactor = 0.5f + Data.Training / 100.0f;
    float Power = Data.Count * (Data.Attack + Data.Defense) * MoraleFactor * TrainingFactor / 10.0f;
    return Power;
}

void UArmyUnit::TakeCasualties(int32 Amount)
{
    Data.Count = FMath::Max(0, Data.Count - Amount);
    // 伤亡影响士气
    if (Data.MaxCount > 0)
    {
        float LossRatio = (float)Amount / Data.MaxCount;
        ChangeMorale(-(int32)(LossRatio * 30));
    }
    if (Data.Count <= 0)
    {
        UE_LOG(LogShanHe, Log, TEXT("部队 %s 全军覆没"), *Data.UnitName.ToString());
    }
}

void UArmyUnit::ChangeMorale(int32 Amount)
{
    Data.Morale = FMath::Clamp(Data.Morale + Amount, 0, 100);
    if (CheckRouting())
    {
        Data.bIsRouting = true;
        UE_LOG(LogShanHe, Warning, TEXT("部队 %s 士气崩溃，开始溃逃！"), *Data.UnitName.ToString());
    }
}

bool UArmyUnit::CheckRouting() const
{
    // 士气低于20，或伤亡超过70%，则溃逃
    return Data.Morale < 20 || (Data.MaxCount > 0 && Data.Count < Data.MaxCount * 0.3f);
}

void UArmyUnit::Rally()
{
    if (Data.bIsRouting)
    {
        Data.bIsRouting = false;
        Data.Morale = FMath::Max(30, Data.Morale + 20);
        UE_LOG(LogShanHe, Log, TEXT("部队 %s 重整旗鼓"), *Data.UnitName.ToString());
    }
}
