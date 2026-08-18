#include "World/ConstructionSystem.h"
#include "Core/ShanHeLog.h"

void UConstructionSystem::StartConstruction(FName BuildingType, FName CityID, int32 Workers)
{
    FConstructionProject Project;
    Project.ProjectID = FName(*FString::Printf(TEXT("Build_%d"), FDateTime::Now().GetTicks()));
    Project.Building.CityID = CityID;
    Project.WorkersAssigned = Workers;

    if (BuildingType == TEXT("Wall"))
    {
        Project.Building.BuildingName = NSLOCTEXT("Build","Wall","城墙");
        Project.Building.Type = EBuildingType::Wall;
        Project.Building.ConstructionDays = 365;
        Project.Building.SilverCost = 50000;
        Project.Building.WorkersRequired = 5000;
        Project.Building.CityEffects.Add(TEXT("Defense"), 50.0f);
    }
    else if (BuildingType == TEXT("Canal"))
    {
        Project.Building.BuildingName = NSLOCTEXT("Build","Canal","运河");
        Project.Building.Type = EBuildingType::Canal;
        Project.Building.ConstructionDays = 730;
        Project.Building.SilverCost = 200000;
        Project.Building.WorkersRequired = 10000;
        Project.Building.CityEffects.Add(TEXT("Trade"), 100.0f);
    }
    else if (BuildingType == TEXT("Granary"))
    {
        Project.Building.BuildingName = NSLOCTEXT("Build","Granary","粮仓");
        Project.Building.Type = EBuildingType::Granary;
        Project.Building.ConstructionDays = 60;
        Project.Building.SilverCost = 5000;
        Project.Building.WorkersRequired = 100;
        Project.Building.CityEffects.Add(TEXT("FoodStorage"), 200.0f);
    }
    else
    {
        Project.Building.BuildingName = NSLOCTEXT("Build","House","民居");
        Project.Building.Type = EBuildingType::House;
        Project.Building.ConstructionDays = 15;
        Project.Building.SilverCost = 200;
        Project.Building.WorkersRequired = 5;
    }

    Project.Building.DaysRemaining = Project.Building.ConstructionDays;
    ActiveProjects.Add(Project);
    UE_LOG(LogShanHe, Log, TEXT("开工：%s，工期 %d 天，需 %d 人"),
        *Project.Building.BuildingName.ToString(), Project.Building.ConstructionDays, Project.Building.WorkersRequired);
}

void UConstructionSystem::SimulateConstruction(int32 Days)
{
    for (int32 i = ActiveProjects.Num() - 1; i >= 0; i--)
    {
        FConstructionProject& P = ActiveProjects[i];
        float Efficiency = (float)P.WorkersAssigned / P.Building.WorkersRequired;
        int32 ActualDays = (int32)(Days * Efficiency);
        P.Building.DaysRemaining -= ActualDays;
        P.Progress = 1.0f - (float)P.Building.DaysRemaining / P.Building.ConstructionDays;

        if (P.Building.DaysRemaining <= 0)
        {
            P.Building.bIsComplete = true;
            CompletedBuildings.Add(P.Building);
            UE_LOG(LogShanHe, Warning, TEXT("竣工：%s！"), *P.Building.BuildingName.ToString());
            ActiveProjects.RemoveAt(i);
        }
    }
}

TArray<FConstructionProject> UConstructionSystem::GetActiveProjects(FName CityID) const
{
    TArray<FConstructionProject> Result;
    for (const FConstructionProject& P : ActiveProjects)
        if (P.Building.CityID == CityID) Result.Add(P);
    return Result;
}

TArray<FBuildingData> UConstructionSystem::GetCompletedBuildings(FName CityID) const
{
    TArray<FBuildingData> Result;
    for (const FBuildingData& B : CompletedBuildings)
        if (B.CityID == CityID) Result.Add(B);
    return Result;
}
