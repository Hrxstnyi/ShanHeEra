#include "Culture/FestivalSystem.h"
#include "Core/ShanHeLog.h"

void UFestivalSystem::InitializeFestivals()
{
    Festivals.Empty();

    struct FFestDef { EFestivalType Type; const TCHAR* Name; int32 Month; int32 Day; const TCHAR* Food; int32 Bonus; bool Event; };
    FFestDef Defs[] = {
        {EFestivalType::SpringFestival, NSLOCTEXT("Fest","Spring","春节"), 1, 1, NSLOCTEXT("Fest","SpringFood","饺子/年糕"), 30, true},
        {EFestivalType::Lantern, NSLOCTEXT("Fest","Lantern","元宵节"), 1, 15, NSLOCTEXT("Fest","LanternFood","汤圆"), 15, true},
        {EFestivalType::Qingming, NSLOCTEXT("Fest","Qingming","清明节"), 3, 5, NSLOCTEXT("Fest","QingmingFood","青团"), 10, true},
        {EFestivalType::DragonBoat, NSLOCTEXT("Fest","Dragon","端午节"), 5, 5, NSLOCTEXT("Fest","DragonFood","粽子"), 20, true},
        {EFestivalType::Qixi, NSLOCTEXT("Fest","Qixi","七夕节"), 7, 7, NSLOCTEXT("Fest","QixiFood","巧果"), 15, true},
        {EFestivalType::MidAutumn, NSLOCTEXT("Fest","Mid","中秋节"), 8, 15, NSLOCTEXT("Fest","MidFood","月饼"), 25, true},
        {EFestivalType::DoubleNinth, NSLOCTEXT("Fest","Double","重阳节"), 9, 9, NSLOCTEXT("Fest","DoubleFood","菊花酒"), 10, false},
        {EFestivalType::WinterSolstice, NSLOCTEXT("Fest","Winter","冬至"), 11, 15, NSLOCTEXT("Fest","WinterFood","饺子/汤圆"), 15, false},
        {EFestivalType::Laba, NSLOCTEXT("Fest","Laba","腊八节"), 12, 8, NSLOCTEXT("Fest","LabaFood","腊八粥"), 10, false},
    };

    for (const auto& D : Defs)
    {
        FFestivalData F;
        F.Type = D.Type; F.Name = D.Name;
        F.LunarMonth = D.Month; F.LunarDay = D.Day;
        F.SpecialFood = D.Food; F.HappinessBonus = D.Bonus;
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
