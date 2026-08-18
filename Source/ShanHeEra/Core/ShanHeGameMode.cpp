#include "Core/ShanHeGameMode.h"
#include "Core/ShanHeGameState.h"
#include "Core/ShanHePlayerController.h"
#include "World/WorldSimulationSubsystem.h"
#include "Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"

AShanHeGameMode::AShanHeGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    PlayerControllerClass = AShanHePlayerController::StaticClass();
    GameStateClass = AShanHeGameState::StaticClass();
}

void AShanHeGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogShanHe, Log, TEXT("山河纪元 - 游戏开始。天下不会等待一个英雄。"));

    if (UAudioManager* AM = GetGameInstance()->GetSubsystem<UAudioManager>())
    {
        AM->PlayBackgroundMusic(TEXT("MainTheme"));
    }
}

void AShanHeGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    DayAccumulator += DeltaSeconds * TimeScale;
    if (DayAccumulator >= 1.0f)
    {
        DayAccumulator -= 1.0f;
        AdvanceTime(DaysPerTick);
    }
}

void AShanHeGameMode::AdvanceTime(int32 Days)
{
    if (AShanHeGameState* GS = GetGameState<AShanHeGameState>())
    {
        GS->AdvanceDays(Days);
    }
    if (UWorldSimulationSubsystem* WS = GetGameInstance()->GetSubsystem<UWorldSimulationSubsystem>())
    {
        WS->SimulateTick(Days);
    }
}

FDate AShanHeGameMode::GetCurrentDate() const
{
    if (AShanHeGameState* GS = GetGameState<AShanHeGameState>())
        return GS->GetCurrentDate();
    return FDate();
}

bool AShanHeGameMode::SaveGame(const FString& SlotName) { return false; }
bool AShanHeGameMode::LoadGame(const FString& SlotName) { return false; }
