#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InformationSystem.generated.h"

UENUM(BlueprintType)
enum class EInfoAccuracy : uint8
{
    Fact        UMETA(DisplayName="事实"),
    Exaggerated UMETA(DisplayName="夸大"),
    Rumor       UMETA(DisplayName="谣言"),
    FalseInfo   UMETA(DisplayName="虚假")
};

USTRUCT(BlueprintType)
struct FInformationPiece
{
    GENERATED_BODY()
    UPROPERTY() FGuid InfoID;
    UPROPERTY() FText Content;
    UPROPERTY() FName SourceCity;
    UPROPERTY() EInfoAccuracy Accuracy = EInfoAccuracy::Fact;
    UPROPERTY() int32 CreatedDay = 0;
    UPROPERTY() int32 SpreadSpeed = 1; // 每天传播多少个城市
    UPROPERTY() TArray<FName> KnownCities;
    UPROPERTY() float Credibility = 1.0f; // 可信度 0~1
    UPROPERTY() bool bIsOfficial = false;
};

/**
 * 信息传播系统 - 实现"信息差"核心设计
 * 信息从源头城市逐渐向外传播，可能被夸大、扭曲、变成谣言
 * 玩家根据所在位置和身份，获取不同准确度的信息
 */
UCLASS()
class SHANHEERA_API UInformationSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|信息")
    void BroadcastInformation(const FText& Content, FName SourceCity, bool bIsOfficial, EInfoAccuracy Accuracy);

    UFUNCTION(BlueprintCallable, Category="山河纪元|信息")
    TArray<FInformationPiece> GetInformationAtCity(FName CityID, int32 MaxCount = 10) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|信息")
    void SimulateSpread(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|信息")
    float GetPlayerInformationAccuracy(ESocialClass PlayerClass, FName PlayerCity) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|信息")
    TArray<FInformationPiece> AllInformation;

private:
    void DegradeInformation(FInformationPiece& Info, int32 Days);
    void SpreadToNeighbors(FInformationPiece& Info);
};
