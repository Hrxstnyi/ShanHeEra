#include "Culture/ReligionSystem.h"
#include "Core/ShanHeLog.h"

void UReligionSystem::InitializeReligions()
{
    FReligionData Buddhist;
    Buddhist.Type = EReligionType::Buddhism;
    Buddhist.Name = NSLOCTEXT("Rel","Buddhism","佛教");
    Buddhist.Description = NSLOCTEXT("Rel","BuddhismDesc","慈悲为怀，普度众生");
    Buddhist.Followers = 100000;
    Buddhist.Influence = 30.0f;
    Religions.Add(EReligionType::Buddhism, Buddhist);

    FReligionData Taoist;
    Taoist.Type = EReligionType::Taoism;
    Taoist.Name = NSLOCTEXT("Rel","Taoism","道教");
    Taoist.Description = NSLOCTEXT("Rel","TaoismDesc","道法自然，无为而治");
    Taoist.Followers = 50000;
    Taoist.Influence = 20.0f;
    Religions.Add(EReligionType::Taoism, Taoist);

    FReligionData Confucian;
    Confucian.Type = EReligionType::Confucianism;
    Confucian.Name = NSLOCTEXT("Rel","Confucian","儒教");
    Confucian.Description = NSLOCTEXT("Rel","ConfucianDesc","仁义礼智信，修身齐家治国平天下");
    Confucian.Followers = 200000;
    Confucian.Influence = 50.0f;
    Religions.Add(EReligionType::Confucianism, Confucian);

    // 初始化寺庙
    FTempleData Shaolin;
    Shaolin.TempleID = TEXT("Shaolin");
    Shaolin.TempleName = NSLOCTEXT("Temple","Shaolin","少林寺");
    Shaolin.Religion = EReligionType::Buddhism;
    Shaolin.CityID = TEXT("Dengfeng");
    Shaolin.Incense = 10000;
    Temples.Add(Shaolin);

    FTempleData Wudang;
    Wudang.TempleID = TEXT("Wudang");
    Wudang.TempleName = NSLOCTEXT("Temple","Wudang","武当山");
    Wudang.Religion = EReligionType::Taoism;
    Wudang.CityID = TEXT("Wudang");
    Wudang.Incense = 8000;
    Temples.Add(Wudang);

    UE_LOG(LogShanHe, Log, TEXT("宗教系统初始化 - 4种信仰，%d 座寺庙"), Temples.Num());
}

void UReligionSystem::DonateToTemple(FName TempleID, int32 Silver)
{
    for (FTempleData& T : Temples)
    {
        if (T.TempleID == TempleID)
        {
            T.Incense += Silver;
            if (FReligionData* R = Religions.Find(T.Religion))
                R->Influence = FMath::Min(100.0f, R->Influence + Silver / 1000.0f);
            UE_LOG(LogShanHe, Log, TEXT("捐赠 %d 银两给 %s"), Silver, *T.TempleName.ToString());
            return;
        }
    }
}

void UReligionSystem::ConvertTo(EReligionType NewReligion)
{
    PlayerFaith = NewReligion;
    UE_LOG(LogShanHe, Log, TEXT("皈依：%d"), (int32)NewReligion);
}

void UReligionSystem::SimulateReligion(int32 Days)
{
    // 宗教影响力缓慢变化
    for (auto& Pair : Religions)
    {
        FReligionData& R = Pair.Value;
        R.Influence = FMath::Clamp(R.Influence + FMath::RandRange(-0.1f, 0.1f) * Days, 0.0f, 100.0f);
    }
}

float UReligionSystem::GetReligionInfluence(EReligionType Type) const
{
    if (const FReligionData* R = Religions.Find(Type))
        return R->Influence;
    return 0.0f;
}
