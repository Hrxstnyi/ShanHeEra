#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/ShanHeStructs.h"
#include "ShanHeSaveSystem.generated.h"

class FArchive;

USTRUCT(BlueprintType)
struct FWorldSaveData
{
    GENERATED_BODY()
    // 元信息
    UPROPERTY() FString SaveName;
    UPROPERTY() FDateTime SaveTime;
    UPROPERTY() int32 PlayTimeSeconds = 0;
    UPROPERTY() int32 Version = 1;

    // 世界状态
    UPROPERTY() FDate CurrentDate;
    UPROPERTY() int32 TotalPopulation = 0;
    UPROPERTY() float WorldStability = 50.0f;
    UPROPERTY() int32 TotalSimulatedDays = 0;

    // 玩家状态
    UPROPERTY() FGuid PlayerNPCID;
    UPROPERTY() FPersonName PlayerName;
    UPROPERTY() FCharacterStats PlayerStats;
    UPROPERTY() ESocialClass PlayerClass = ESocialClass::Farmer;
    UPROPERTY() int32 PlayerAge = 16;
    UPROPERTY() FResourceStock PlayerWealth;
    UPROPERTY() FName PlayerLocation;

    // 经济状态
    UPROPERTY() float NationalTreasury = 100000.0f;
    UPROPERTY() float InflationRate = 0.0f;
    UPROPERTY() TArray<float> PriceHistory;

    // 政治状态
    UPROPERTY() float ImperialAuthority = 70.0f;
    UPROPERTY() float CourtStability = 60.0f;
    UPROPERTY() int32 ReignYears = 0;

    // 区域数据 (序列化时用TArray替代TMap)
    UPROPERTY() TArray<FName> RegionIDs;
    UPROPERTY() TArray<int32> RegionPopulations;
    UPROPERTY() TArray<float> RegionWealths;
    UPROPERTY() TArray<float> RegionStabilities;

    // 城市数据
    UPROPERTY() TArray<FName> CityIDs;
    UPROPERTY() TArray<int32> CityPopulations;
    UPROPERTY() TArray<float> CityProsperities;

    // 已触发事件
    UPROPERTY() TArray<FName> TriggeredEvents;

    // NPC数据 (关键NPC，普通NPC用种子生成)
    UPROPERTY() TArray<FGuid> ImportantNPCIDs;
    UPROPERTY() TArray<FPersonName> ImportantNPCNames;
    UPROPERTY() TArray<int32> ImportantNPCAges;
    UPROPERTY() TArray<uint8> ImportantNPCAlive; // bool序列化
};

/**
 * 存档系统 - 使用二进制序列化，不用JSON
 * 世界状态数据量大，二进制比JSON快10倍以上，体积小5倍
 */
UCLASS()
class SHANHEERA_API USaveSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    bool SaveGame(const FString& SlotName, const FWorldSaveData& Data);

    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    bool LoadGame(const FString& SlotName, FWorldSaveData& OutData);

    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    bool DeleteSave(const FString& SlotName);

    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    TArray<FString> GetAllSaveSlots() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    FString GetSaveDirectory() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    void QuickSave();

    UFUNCTION(BlueprintCallable, Category="山河纪元|存档")
    void QuickLoad();

private:
    FString GetSaveFilePath(const FString& SlotName) const;
    bool SerializeData(FArchive& Ar, FWorldSaveData& Data);
};
