#include "Society/MartialArtsSystem.h"
#include "Core/ShanHeLog.h"

void UMartialArtsSystem::InitializeSkills()
{
    struct FSkillDef { FName ID; const TCHAR* Name; EMartialSkillType Type; ESkillGrade Grade; float Dmg; float Sta; };
    FSkillDef Skills[] = {
        {TEXT("iron_palm"), TEXT("铁砂掌"), EMartialSkillType::External, ESkillGrade::Common, 15.0f, 8.0f},
        {TEXT("taiji_fist"), TEXT("太极拳"), EMartialSkillType::External, ESkillGrade::Master, 25.0f, 5.0f},
        {TEXT("eight_trigram"), TEXT("八卦掌"), EMartialSkillType::External, ESkillGrade::Excellent, 22.0f, 10.0f},
        {TEXT("shaolin_fist"), TEXT("少林拳"), EMartialSkillType::External, ESkillGrade::Fine, 18.0f, 8.0f},
        {TEXT("sunflower"), TEXT("葵花宝典"), EMartialSkillType::Internal, ESkillGrade::Legendary, 50.0f, 20.0f},
        {TEXT("nine_yang"), TEXT("九阳神功"), EMartialSkillType::Internal, ESkillGrade::Legendary, 40.0f, 0.0f},
        {TEXT("small_zhou"), TEXT("小周天功"), EMartialSkillType::Internal, ESkillGrade::Common, 10.0f, 0.0f},
        {TEXT("lightness_cloud"), TEXT("踏雪无痕"), EMartialSkillType::Lightness, ESkillGrade::Excellent, 0.0f, 15.0f},
        {TEXT("lightness_wind"), TEXT("御风而行"), EMartialSkillType::Lightness, ESkillGrade::Master, 0.0f, 20.0f},
        {TEXT("hidden_throwing"), TEXT("弹指神通"), EMartialSkillType::Hidden, ESkillGrade::Master, 30.0f, 5.0f},
        {TEXT("hidden_needle"), TEXT("暴雨梨花针"), EMartialSkillType::Hidden, ESkillGrade::Legendary, 45.0f, 10.0f},
    };

    for (const auto& S : Skills)
    {
        FMartialSkill Sk;
        Sk.SkillID = S.ID; Sk.SkillName = FText::FromString(UTF8_TO_TCHAR(S.Name));
        Sk.Type = S.Type; Sk.Grade = S.Grade;
        Sk.BaseDamage = S.Dmg; Sk.StaminaCost = S.Sta;
        SkillDatabase.Add(S.ID, Sk);
    }

    UE_LOG(LogShanHe, Log, TEXT("武功系统初始化 - %d 种武功"), SkillDatabase.Num());
}

bool UMartialArtsSystem::LearnSkill(FName SkillID)
{
    if (LearnedSkills.Contains(SkillID)) return false;
    if (!SkillDatabase.Contains(SkillID)) return false;
    LearnedSkills.Add(SkillID);
    UE_LOG(LogShanHe, Log, TEXT("学会武功：%s"), *SkillID.ToString());
    return true;
}

bool UMartialArtsSystem::PracticeSkill(FName SkillID, float Hours)
{
    if (FMartialSkill* Sk = SkillDatabase.Find(SkillID))
    {
        if (!LearnedSkills.Contains(SkillID)) return false;
        float Gain = Hours * 2.0f; // 每小时2点
        Sk->Level = FMath::Min(Sk->MaxLevel, Sk->Level + (int32)Gain);
        UE_LOG(LogShanHe, Log, TEXT("修炼 %s：%.1f小时，当前等级 %d"), *SkillID.ToString(), Hours, Sk->Level);
        return true;
    }
    return false;
}

bool UMartialArtsSystem::ComprehendManual(FName ManualID, int32 Intelligence)
{
    if (const FSkillManual* M = ManualDatabase.Find(ManualID))
    {
        int32 Roll = FMath::RandRange(1, 100) + Intelligence;
        if (Roll >= M->ComprehensionDifficulty)
        {
            LearnSkill(M->SkillID);
            UE_LOG(LogShanHe, Log, TEXT("参悟成功：%s → %s"), *ManualID.ToString(), *M->SkillID.ToString());
            return true;
        }
        UE_LOG(LogShanHe, Log, TEXT("参悟失败：%s (roll=%d, need=%d)"), *ManualID.ToString(), Roll, M->ComprehensionDifficulty);
    }
    return false;
}

float UMartialArtsSystem::GetSkillPower(FName SkillID) const
{
    if (const FMartialSkill* Sk = SkillDatabase.Find(SkillID))
        return Sk->BaseDamage * (1.0f + Sk->Level / 50.0f);
    return 0.0f;
}

TArray<FMartialSkill> UMartialArtsSystem::GetLearnedSkills() const
{
    TArray<FMartialSkill> Result;
    for (FName ID : LearnedSkills)
        if (const FMartialSkill* Sk = SkillDatabase.Find(ID))
            Result.Add(*Sk);
    return Result;
}

bool UMartialArtsSystem::CanLearnSkill(FName SkillID, int32 Strength, int32 Intelligence, int32 Agility) const
{
    if (const FMartialSkill* Sk = SkillDatabase.Find(SkillID))
    {
        // 不同类型有不同属性要求
        switch (Sk->Type)
        {
            case EMartialSkillType::External: return Strength >= 8;
            case EMartialSkillType::Internal: return Intelligence >= 8;
            case EMartialSkillType::Lightness: return Agility >= 10;
            case EMartialSkillType::Hidden: return Agility >= 8 && Intelligence >= 6;
        }
    }
    return false;
}
