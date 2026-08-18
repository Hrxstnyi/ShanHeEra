#include "Core/AttributeSystem.h"
#include "Core/ShanHeLog.h"

void UAttributeSystem::InitializeAttributes(FCharacterAttributes InitialAttrs)
{
    BaseAttributes = InitialAttrs;
    // 初始化所有技能为0级
    for (int32 i = 0; i < (int32)ESkillType::Stealth + 1; i++)
    {
        FSkillData S;
        S.SkillType = (ESkillType)i;
        Skills.Add((ESkillType)i, S);
    }
    UE_LOG(LogShanHe, Log, TEXT("属性系统初始化 - 六维属性+17技能"));
}

void UAttributeSystem::AddSkillExperience(ESkillType Skill, int32 Amount)
{
    if (FSkillData* Data = Skills.Find(Skill))
    {
        // 悟性影响经验获取
        float IntBonus = 1.0f + BaseAttributes.Intelligence / 100.0f;
        Data->Experience += (int32)(Amount * IntBonus);
        CheckLevelUp(Skill);
    }
}

int32 UAttributeSystem::GetSkillLevel(ESkillType Skill) const
{
    if (const FSkillData* Data = Skills.Find(Skill))
        return Data->Level;
    return 0;
}

void UAttributeSystem::AddStatusEffect(FStatusEffect Effect)
{
    ActiveEffects.Add(Effect);
    UE_LOG(LogShanHe, Log, TEXT("获得状态：%s，持续 %d 天"), *Effect.EffectName.ToString(), Effect.DurationDays);
}

void UAttributeSystem::RemoveStatusEffect(FName EffectID)
{
    for (int32 i = ActiveEffects.Num() - 1; i >= 0; i--)
    {
        if (ActiveEffects[i].EffectID == EffectID)
        {
            UE_LOG(LogShanHe, Log, TEXT("状态消失：%s"), *ActiveEffects[i].EffectName.ToString());
            ActiveEffects.RemoveAt(i);
            return;
        }
    }
}

void UAttributeSystem::SimulateEffects(int32 Days)
{
    for (int32 i = ActiveEffects.Num() - 1; i >= 0; i--)
    {
        ActiveEffects[i].DurationDays -= Days;
        if (ActiveEffects[i].DurationDays <= 0)
        {
            UE_LOG(LogShanHe, Log, TEXT("状态结束：%s"), *ActiveEffects[i].EffectName.ToString());
            ActiveEffects.RemoveAt(i);
        }
    }
}

int32 UAttributeSystem::GetEffectiveAttribute(EAttributeType Type) const
{
    int32 Base = BaseAttributes.GetAttribute(Type);
    int32 Modifier = 0;
    for (const FStatusEffect& E : ActiveEffects)
    {
        if (const int32* Mod = E.AttributeModifiers.Find(Type))
            Modifier += *Mod;
    }
    return FMath::Max(1, Base + Modifier);
}

void UAttributeSystem::CheckLevelUp(ESkillType Skill)
{
    if (FSkillData* Data = Skills.Find(Skill))
    {
        while (Data->Experience >= Data->ExpToNext && Data->Level < 100)
        {
            Data->Experience -= Data->ExpToNext;
            Data->Level++;
            Data->ExpToNext = (int32)(Data->ExpToNext * 1.5f);
            UE_LOG(LogShanHe, Log, TEXT("技能升级！%d 级"), Data->Level);
        }
    }
}
