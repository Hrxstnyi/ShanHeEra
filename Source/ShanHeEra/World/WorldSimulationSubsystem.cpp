#include "World/WorldSimulationSubsystem.h"
#include "Core/ShanHeLog.h"

void UWorldSimulationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    RegionManager = NewObject<URegionManager>(this);
    CityManager = NewObject<UCityManager>(this);
    CourtSystem = NewObject<UCourtSystem>(this);
    EconomySystem = NewObject<UEconomySystem>(this);
    WarSystem = NewObject<UWarSystem>(this);
    HistorySystem = NewObject<UHistorySystem>(this);
    SocietySystem = NewObject<USocietySystem>(this);
    UE_LOG(LogShanHe, Log, TEXT("世界模拟子系统初始化完成 - 天下不会等待任何人"));
}

void UWorldSimulationSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UWorldSimulationSubsystem::InitializeWorld()
{
    if (RegionManager) RegionManager->InitializeRegions();
    if (CityManager) CityManager->InitializeCities();
    if (CourtSystem) CourtSystem->InitializeCourt();
    if (EconomySystem) EconomySystem->InitializeEconomy();
    if (WarSystem) WarSystem->InitializeMilitary();
    if (HistorySystem) HistorySystem->InitializeHistory();
    if (SocietySystem) SocietySystem->InitializeSociety();
    UE_LOG(LogShanHe, Log, TEXT("世界初始化完成 - 亿万普通人的选择，共同组成时代"));
}

void UWorldSimulationSubsystem::SimulateTick(int32 Days)
{
    TotalSimulatedDays += Days;
    SimulateEconomy(Days);
    SimulatePolitics(Days);
    SimulateMilitary(Days);
    SimulateSociety(Days);
    SimulateDisasters(Days);
    CheckHistoricalEvents();
}

void UWorldSimulationSubsystem::SimulateEconomy(int32 Days) { if (EconomySystem) EconomySystem->Simulate(Days); }
void UWorldSimulationSubsystem::SimulatePolitics(int32 Days) { if (CourtSystem) CourtSystem->Simulate(Days); }
void UWorldSimulationSubsystem::SimulateMilitary(int32 Days) { if (WarSystem) WarSystem->Simulate(Days); }
void UWorldSimulationSubsystem::SimulateSociety(int32 Days) { if (SocietySystem) SocietySystem->Simulate(Days); }
void UWorldSimulationSubsystem::SimulateDisasters(int32 Days) { if (HistorySystem) HistorySystem->SimulateDisasters(Days); }
void UWorldSimulationSubsystem::CheckHistoricalEvents() { if (HistorySystem) HistorySystem->CheckEvents(); }
