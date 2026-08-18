#include "Core/ShanHeGameMode.h"
#include "Core/ShanHeLog.h"
#include "Core/ShanHeGameState.h"
#include "Core/ShanHePlayerController.h"
#include "Core/ShanHeCharacter.h"
#include "Core/ShanHeHUD.h"
#include "Core/ShanHeSaveGame.h"
#include "World/WorldSimulationSubsystem.h"
#include "Audio/AudioManager.h"
#include "Kismet/GameplayStatics.h"

AShanHeGameMode::AShanHeGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    PlayerControllerClass = AShanHePlayerController::StaticClass();
    GameStateClass = AShanHeGameState::StaticClass();
    DefaultPawnClass = AShanHeCharacter::StaticClass();
    HUDClass = AShanHeHUD::StaticClass();
}

void AShanHeGameMode::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogShanHe, Log, TEXT("山河纪元 - 游戏开始。天下不会等待一个英雄。"));

    if (UAudioManager* AM = GetGameInstance()->GetSubsystem<UAudioManager>())
    {
        AM->PlayMusic(FName(TEXT("MainTheme")));
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

bool AShanHeGameMode::SaveGame(const FString& SlotName)
{
    UShanHeSaveGame* SaveObj = Cast<UShanHeSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UShanHeSaveGame::StaticClass()));
    if (!SaveObj) return false;

    // 保存玩家数据
    if (AShanHeCharacter* Player = Cast<AShanHeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
    {
        SaveObj->PlayerData.PlayerName = Player->GetFullName();
        SaveObj->PlayerData.Age = Player->Age;
        SaveObj->PlayerData.Gold = (int32)Player->Money;
        SaveObj->PlayerData.Attributes.Add(TEXT("Health"), Player->Health);
        SaveObj->PlayerData.Attributes.Add(TEXT("Stamina"), Player->Stamina);
    }

    // 保存世界数据
    if (AShanHeGameState* GS = GetGameState<AShanHeGameState>())
    {
        FDate Date = GS->GetCurrentDate();
        SaveObj->WorldData.Year = Date.Year;
        SaveObj->WorldData.Month = Date.Month;
        SaveObj->WorldData.Day = Date.Day;
    }

    // 保存元数据
    SaveObj->LastSaveTime = FDateTime::Now().ToString();
    SaveObj->PlayTimeSeconds += UGameplayStatics::GetRealTimeSeconds(this);

    bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveObj, SlotName, 0);
    UE_LOG(LogShanHe, Log, TEXT("存档 %s: %s"), *SlotName, bSuccess ? TEXT("成功") : TEXT("失败"));
    return bSuccess;
}

bool AShanHeGameMode::LoadGame(const FString& SlotName)
{
    if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        UE_LOG(LogShanHe, Warning, TEXT("存档 %s 不存在"), *SlotName);
        return false;
    }

    UShanHeSaveGame* SaveObj = Cast<UShanHeSaveGame>(
        UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if (!SaveObj) return false;

    // 迁移旧存档
    if (SaveObj->NeedsMigration())
    {
        SaveObj->MigrateToLatest();
    }

    // 恢复世界时间
    if (AShanHeGameState* GS = GetGameState<AShanHeGameState>())
    {
        FDate Date;
        Date.Year = SaveObj->WorldData.Year;
        Date.Month = SaveObj->WorldData.Month;
        Date.Day = SaveObj->WorldData.Day;
        GS->SetDate(Date);
    }

    // 恢复玩家数据
    if (AShanHeCharacter* Player = Cast<AShanHeCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
    {
        Player->Age = SaveObj->PlayerData.Age;
        Player->Money = SaveObj->PlayerData.Gold;
        if (float* Health = SaveObj->PlayerData.Attributes.Find(TEXT("Health")))
            Player->Health = *Health;
        if (float* Stamina = SaveObj->PlayerData.Attributes.Find(TEXT("Stamina")))
            Player->Stamina = *Stamina;
    }

    UE_LOG(LogShanHe, Log, TEXT("读档 %s 成功"), *SlotName);
    return true;
}
