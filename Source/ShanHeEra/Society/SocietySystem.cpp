#include "Society/SocietySystem.h"
#include "Core/ShanHeLog.h"

void USocietySystem::InitializeSociety()
{
    FOrganizationData HuiZhou;
    HuiZhou.OrgID = TEXT("HuiZhouMerchants");
    HuiZhou.OrgName = FText::FromString(TEXT("徽州商帮"));
    HuiZhou.Type = FText::FromString(TEXT("商帮"));
    HuiZhou.BaseCity = TEXT("Hangzhou");
    HuiZhou.Influence = 60;
    HuiZhou.Members = 5000;
    HuiZhou.Wealth = 500000;
    Organizations.Add(HuiZhou);

    FOrganizationData Donglin;
    Donglin.OrgID = TEXT("DonglinAcademy");
    Donglin.OrgName = FText::FromString(TEXT("东林书院"));
    Donglin.Type = FText::FromString(TEXT("书院"));
    Donglin.BaseCity = TEXT("Yingtian");
    Donglin.Influence = 45;
    Donglin.Members = 2000;
    Organizations.Add(Donglin);

    FOrganizationData BiaoJu;
    BiaoJu.OrgID = TEXT("TianXiaBiaoJu");
    BiaoJu.OrgName = FText::FromString(TEXT("天下镖局"));
    BiaoJu.Type = FText::FromString(TEXT("镖局"));
    BiaoJu.BaseCity = TEXT("Beijing");
    BiaoJu.Influence = 30;
    BiaoJu.Members = 800;
    Organizations.Add(BiaoJu);

    UE_LOG(LogShanHe, Log, TEXT("社会组织系统初始化 - 宗族、行会、商帮、书院、镖局、江湖"));
}

void USocietySystem::Simulate(int32 Days)
{
    for (FOrganizationData& O : Organizations)
        O.Influence += FMath::RandRange(-1, 1) * (Days / 30);
}
