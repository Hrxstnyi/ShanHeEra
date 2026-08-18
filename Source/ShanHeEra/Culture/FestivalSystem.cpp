#include "Culture/FestivalSystem.h"
#include "Core/ShanHeLog.h"

void UFestivalSystem::InitializeFestivals()
{
    Festivals.Empty();

    struct FFestDef { EFestivalType Type; const TCHAR* Name; int32 Month; int32 Day; const TCHAR* Food; int32 Bonus; bool Event; };
    FFestDef Defs[] = {
        {EFestivalType::SpringFestival, TEXT("春节"), 1, 1, TEXT("饺子/年糕"), 30, true},
        {EFestivalType::Lantern, TEXT("元宵节"), 1, 15, TEXT("汤圆"), 15, true},
        {EFestivalType::Qingming, TEXT("清明节"), 3, 5, TEXT("青团"), 10, true},
        {EFestivalType::DragonBoat, TEXT("端午节"), 5, 5, TEXT("粽子"), 20, true},
        {EFestivalType::Qixi, TEXT("七夕节"), 7, 7, TEXT("巧果"), 15, true},
        {EFestivalType::MidAutumn, TEXT("中秋节"), 8, 15, TEXT("月饼"), 25, true},
        {EFestivalType::DoubleNinth, TEXT("重阳节"), 9, 9, TEXT("菊花酒"), 10, false},
        {EFestivalType::WinterSolstice, TEXT("冬至"), 11, 15, TEXT("饺子/汤圆"), 15, false},
        {EFestivalType::Laba, TEXT("腊八节"), 12, 8, TEXT("腊八粥"), 10, false},
    };

    for (const auto& D : Defs)
    {
        FFestivalData F;
        F.Type = D.Type; F.Name = FText::FromString(D.Name);
        F.LunarMonth = D.Month; F.LunarDay = D.Day;
        F.SpecialFood = FText::FromString(D.Food); F.HappinessBonus = D.Bonus;
        F.bHasSpecialEvent = D.Event;
        Festivals.Add(F);
    }

    UE_LOG(LogShanHe, Log, TEXT("节日系统初始化 - %d 个传统节日"), Festivals.Num());
}

FFestivalData UFestivalSystem::GetFestivalForDate(int32 LunarMonth, int32 LunarDay) const
{
    for (const FFestivalData& F : Festivals)
        if (F.LunarMonth == LunarMonth && F.LunarDay == LunarDay)
            return F;
    return FFestivalData();
}

bool UFestivalSystem::IsFestivalDay(int32 LunarMonth, int32 LunarDay) const
{
    for (const FFestivalData& F : Festivals)
        if (F.LunarMonth == LunarMonth && F.LunarDay == LunarDay)
            return true;
    return false;
}

void UFestivalSystem::TriggerFestivalEvent(EFestivalType Type)
{
    CurrentFestival = Type;
    bIsFestivalActive = true;
    for (const FFestivalData& F : Festivals)
    {
        if (F.Type == Type)
        {
            UE_LOG(LogShanHe, Log, TEXT("节日来临：%s！习俗：%s，美食：%s"),
                *F.Name.ToString(), *F.Customs.ToString(), *F.SpecialFood.ToString());
            return;
        }
    }
}
