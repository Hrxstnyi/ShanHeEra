#include "Core/ShanHeGameState.h"

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
