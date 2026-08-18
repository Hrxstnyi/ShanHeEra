#include "Core/ShanHeGameState.h"
#include "Core/ShanHeLog.h"

AShanHeGameState::AShanHeGameState()
{
    CurrentDate.Year = 1368;
    CurrentDate.Month = 1;
    CurrentDate.Day = 1;
    CurrentDate.Era = EEraType::Unified;
}

void AShanHeGameState::AdvanceDays(int32 Days)
{
    CurrentDate.Day += Days;
    while (CurrentDate.Day > 30)
    {
        CurrentDate.Day -= 30;
        CurrentDate.Month++;
        if (CurrentDate.Month > 12)
        {
            CurrentDate.Month = 1;
            CurrentDate.Year++;
        }
    }
}

ESeason AShanHeGameState::GetCurrentSeason() const
{
    // 农历月份对应季节：正月-三月春，四月-六月夏，七月-九月秋，十月-腊月冬
    switch (CurrentDate.Month)
    {
    case 1: case 2: case 3:  return ESeason::Spring;
    case 4: case 5: case 6:  return ESeason::Summer;
    case 7: case 8: case 9:  return ESeason::Autumn;
    case 10: case 11: case 12: return ESeason::Winter;
    default: return ESeason::Spring;
    }
}

ESolarTerm AShanHeGameState::GetCurrentSolarTerm() const
{
    // 简化版二十四节气：每月两个节气，1-15日上半月，16-30月下半月
    int32 TermIndex = (CurrentDate.Month - 1) * 2 + (CurrentDate.Day > 15 ? 1 : 0);
    return (ESolarTerm)FMath::Clamp(TermIndex, 0, 23);
}

FString AShanHeGameState::GetFormattedDate() const
{
    static const TCHAR* EraNames[] = {
        TEXT("远古"), TEXT("诸侯"), TEXT("统一"), TEXT("盛世"), TEXT("衰落"), TEXT("乱世"), TEXT("近现代")
    };
    int32 EraIndex = (int32)CurrentDate.Era;
    return FString::Printf(TEXT("%s%d年%d月%d日"),
        EraIndex < 7 ? EraNames[EraIndex] : TEXT(""),
        CurrentDate.Year, CurrentDate.Month, CurrentDate.Day);
}

FString AShanHeGameState::GetSeasonName() const
{
    switch (GetCurrentSeason())
    {
    case ESeason::Spring: return TEXT("春");
    case ESeason::Summer: return TEXT("夏");
    case ESeason::Autumn: return TEXT("秋");
    case ESeason::Winter: return TEXT("冬");
    default: return TEXT("");
    }
}

FString AShanHeGameState::GetSolarTermName() const
{
    static const TCHAR* TermNames[] = {
        TEXT("立春"), TEXT("雨水"), TEXT("惊蛰"), TEXT("春分"), TEXT("清明"), TEXT("谷雨"),
        TEXT("立夏"), TEXT("小满"), TEXT("芒种"), TEXT("夏至"), TEXT("小暑"), TEXT("大暑"),
        TEXT("立秋"), TEXT("处暑"), TEXT("白露"), TEXT("秋分"), TEXT("寒露"), TEXT("霜降"),
        TEXT("立冬"), TEXT("小雪"), TEXT("大雪"), TEXT("冬至"), TEXT("小寒"), TEXT("大寒")
    };
    int32 Index = (int32)GetCurrentSolarTerm();
    return Index >= 0 && Index < 24 ? TermNames[Index] : TEXT("");
}
